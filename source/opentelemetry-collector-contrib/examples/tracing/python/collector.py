import flask
import logging
import requests
import sys
import threading

from opentelemetry import trace
from opentelemetry.ext.flask import FlaskInstrumentor
from opentelemetry.ext.requests import RequestsInstrumentor
from opentelemetry.sdk.resources import Resource
from opentelemetry.sdk.trace import TracerProvider
from opentelemetry.sdk.trace.export import BatchExportSpanProcessor
from opentelemetry.ext.otlp.trace_exporter import OTLPSpanExporter

my_port = 0
if len(sys.argv) > 1:
    my_port = int(sys.argv[1])
else:
    print("need to specify port number as first parameter")

exporter = OTLPSpanExporter(endpoint="omsagent-otel:55680")

trace.set_tracer_provider(TracerProvider(resource=Resource(labels={ "service.name": "basic-service-%s" % my_port})))
span_processor = BatchExportSpanProcessor(exporter)
trace.get_tracer_provider().add_span_processor(span_processor)

app = flask.Flask(__name__)
FlaskInstrumentor().instrument_app(app)
RequestsInstrumentor().instrument()

forward_port = 0
if len(sys.argv) > 2:
    forward_port = int(sys.argv[2])

@app.route("/")
def pull_requests():
    global forward_port

    # Fetch a list of pull requests on the opentracing repository
    github_url = "https://api.github.com/repos/opentracing/opentracing-python/pulls"
    r = requests.get(github_url)

    json = r.json()
    pull_request_titles = map(lambda item: item['title'], json)

    if forward_port:
        requests.get("http://localhost:%s" % forward_port)

    return 'Success!'

app.run(debug=True, port=my_port)