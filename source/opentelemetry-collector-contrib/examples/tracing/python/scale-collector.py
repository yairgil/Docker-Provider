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
# from opentelemetry.ext.opencensusexporter.trace_exporter import (
#   OpenCensusSpanExporter,
# )

my_port = 5001

# exporter = OpenCensusSpanExporter(
#     service_name="basic-service-%s" % my_port, endpoint="otel-collector:55678"
# )

exporter = OTLPSpanExporter(endpoint="otel-collector:55680")

# trace.set_tracer_provider(TracerProvider())
trace.set_tracer_provider(TracerProvider(resource=Resource(labels={ "service.name": "basic-service" })))
span_processor = BatchExportSpanProcessor(exporter)
trace.get_tracer_provider().add_span_processor(span_processor)

app = flask.Flask(__name__)
FlaskInstrumentor().instrument_app(app)
RequestsInstrumentor().instrument()

logging.basicConfig(level=logging.INFO)
counter = 0

@app.route("/")
def thread_function(name):
    global counter
    while(1):
        url = "http://localhost:5001"
        r = requests.get(url)
        counter += 1
        if counter % 1000 == 0:
            app.logger.info("sent %d" % counter)
    return 'Success! Sent ' + str(counter) + ' traces\n'

result = threading.Thread(target=thread_function, args=(1,))
result.start()

app.run(debug=True, port=my_port)
result.join()
