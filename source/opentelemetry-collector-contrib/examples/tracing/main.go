// real one is in go/src/otel-test-app

package main

import (
	"context"
	"log"

	"go.opentelemetry.io/otel/api/correlation"
	"go.opentelemetry.io/otel/api/global"
	"go.opentelemetry.io/otel/api/kv"
	"go.opentelemetry.io/otel/api/metric"
	"go.opentelemetry.io/otel/api/trace"
	metricstdout "go.opentelemetry.io/otel/exporters/metric/stdout"
	"go.opentelemetry.io/otel/exporters/trace/jaeger"
	"go.opentelemetry.io/otel/sdk/metric/controller/push"
	"go.opentelemetry.io/otel/sdk/resource/resourcekeys"
	sdktrace "go.opentelemetry.io/otel/sdk/trace"
)

var (
	fooKey          = kv.Key("ex.com/foo")
	barKey          = kv.Key("ex.com/bar")
	lemonsKey       = kv.Key("ex.com/lemons")
	anotherKey      = kv.Key("ex.com/another")
	httpStatus      = kv.Key("http.status_code")
	httpSchemeKey   = kv.Key("http.scheme")
	httpHostKey     = kv.Key("http.host")
	httpTargetKey   = kv.Key("http.target")
	peerServiceKey  = kv.Key("peer.service")
	peerhostnameKey = kv.Key("peer.hostname")
	peerportKey     = kv.Key("peer.port")
	componentKey    = kv.Key("component")
)

// initTracer creates and registers trace provider instance. OTLP
func initTracer() {
	exp, _ := otlp.NewExporter(otlp.WithInsecure(), otlp.WithAddress("localhost:9090"))
	tp, _ := sdktrace.NewProvider(
		sdktrace.WithSyncer(exp),
		sdktrace.WithConfig(sdktrace.Config{DefaultSampler: sdktrace.AlwaysSample()}),
		sdktrace.WithResourceAttributes(
			key.String(resourcekeys.ServiceKeyName, "otlp"),
			key.String(resourcekeys.ServiceKeyNamespace, "namespace"),
			key.String(resourcekeys.HostKeyHostName, "nana"),
		),
	)
	global.SetTraceProvider(tp)
}

func initTracer_jager() func() {
	// Create and install Jaeger export pipeline
	_, flush, err := jaeger.NewExportPipeline(
		jaeger.WithCollectorEndpoint("http://localhost:14268/api/traces"),
		jaeger.WithProcess(jaeger.Process{
			ServiceName: "trace-demo",
			Tags: []kv.KeyValue{
				kv.String("exporter", "jaeger"),
				kv.Float64("float", 312.23),
				kv.String(resourcekeys.ServiceKeyNamespace, "namespace"),
				kv.String(resourcekeys.HostKeyHostName, "nana"),
			},
		}),
		jaeger.RegisterAsGlobal(),
		jaeger.WithSDK(&sdktrace.Config{DefaultSampler: sdktrace.AlwaysSample()}),
	)
	if err != nil {
		log.Fatal(err)
	}

	return func() {
		flush()
	}
}

func initMeter() *push.Controller {
	pusher, err := metricstdout.InstallNewPipeline(metricstdout.Config{
		Quantiles:   []float64{0.5, 0.9, 0.99},
		PrettyPrint: false,
	})
	if err != nil {
		log.Panicf("failed to initialize metric stdout exporter %v", err)
	}
	return pusher
}

func main() {
	defer initMeter().Stop()
	fn := initTracer()
	// fn := initTracer_jager()
	defer fn()

	tracer := global.Tracer("ex.com/basic")
	meter := global.Meter("ex.com/basic")

	commonLabels := []kv.KeyValue{lemonsKey.Int(10), kv.String("A", "1"), kv.String("B", "2"), kv.String("C", "3")}

	// oneMetricCB := func(result metric.Float64ObserverResult) {
	// 	result.Observe(1, commonLabels...)
	// // }
	// _ = metric.Must(meter).RegisterFloat64Observer("ex.com.one", oneMetricCB,
	// 	metric.WithDescription("An observer set to 1.0"),
	// )

	measureTwo := metric.Must(meter).NewFloat64Measure("ex.com.two")

	ctx := context.Background()

	ctx = correlation.NewContext(ctx,
		fooKey.String("foo1"),
		barKey.String("bar1"),
	)

	measure := measureTwo.Bind(commonLabels...)
	defer measure.Unbind()

	for i := 0; i < 10000; i++ {
		err := tracer.WithSpan(ctx, "operation1", func(ctx context.Context) error {

			trace.SpanFromContext(ctx).AddEvent(ctx, "Nice operation!", kv.Key("bogons").Int(100))
			trace.SpanFromContext(ctx).SetAttributes(anotherKey.String("yes"))
			trace.SpanFromContext(ctx).SetAttributes(componentKey.String("http"))
			trace.SpanFromContext(ctx).SetAttributes(httpSchemeKey.String("http"))
			trace.SpanFromContext(ctx).SetAttributes(httpHostKey.String("localhost"))
			trace.SpanFromContext(ctx).SetAttributes(httpTargetKey.String("statestore"))
			trace.SpanFromContext(ctx).SetAttributes(httpStatus.Int(200))
			return tracer.WithSpan(
				ctx,
				"Sub operation1...",
				func(ctx context.Context) error {
					trace.SpanFromContext(ctx).SetAttributes(lemonsKey.String("five"))
					trace.SpanFromContext(ctx).AddEvent(ctx, "Sub span event")
					trace.SpanFromContext(ctx).SetAttributes(componentKey.String("http"))
					trace.SpanFromContext(ctx).SetAttributes(httpStatus.Int(404))
					return nil
				}, trace.WithSpanKind(trace.SpanKindServer),
			)
		}, trace.WithSpanKind(trace.SpanKindClient))

		tracer.WithSpan(ctx, "operation3", func(ctx context.Context) error {
			trace.SpanFromContext(ctx).SetAttributes(anotherKey.String("yes"))
			trace.SpanFromContext(ctx).SetAttributes(componentKey.String("grpc"))
			trace.SpanFromContext(ctx).SetAttributes(peerServiceKey.String("dapr"))
			trace.SpanFromContext(ctx).SetAttributes(peerhostnameKey.String("statestore4"))
			trace.SpanFromContext(ctx).SetAttributes(peerportKey.String("state3"))
			return nil
		}, trace.WithSpanKind(trace.SpanKindClient))
		if err != nil {
			panic(err)
		}
	}
}
