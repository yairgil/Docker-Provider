package main

import (
	"go.opentelemetry.io/collector/component"
	"go.opentelemetry.io/collector/component/componenterror"
	"go.opentelemetry.io/collector/service/defaultcomponents"
	"github.com/vishiy/influxexporter"
)

func components() (component.Factories, error) {
	var errs []error
	factories, err := defaultcomponents.Components()
	if err != nil {
		return component.Factories{}, err
	}

	exporters := []component.ExporterFactory{
		influxexporter.NewFactory(),
	}
	for _, pr := range factories.Exporters {
		exporters = append(exporters, pr)
	}
	factories.Exporters, err = component.MakeExporterFactoryMap(exporters...)
	if err != nil {
		errs = append(errs, err)
	}

	return factories, componenterror.CombineErrors(errs)
}