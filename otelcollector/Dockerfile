FROM ubuntu:20.04
LABEL description="Azure Monitor Prometheus metrics collector"
LABEL maintainer="omscontainers@microsoft.com"
ENV DEBIAN_FRONTEND=noninteractive
ENV tmpdir /opt
ENV APPLICATIONINSIGHTS_AUTH NzAwZGM5OGYtYTdhZC00NThkLWI5NWMtMjA3ZjM3NmM3YmRi
RUN apt-get update && apt-get install -y libc-bin wget openssl curl sudo init-system-helpers net-tools cron vim apt-transport-https locales ruby-full
COPY ./scripts/livenessprobe.sh $tmpdir/microsoft/liveness/livenessprobe.sh
COPY ./configmapparser/*.rb $tmpdir/microsoft/configmapparser/
COPY ./opentelemetry-collector-builder/otelcollector ./opentelemetry-collector-builder/collector-config-default.yml ./opentelemetry-collector-builder/collector-config-template.yml $tmpdir/microsoft/otelcollector/
COPY ./scripts/setup.sh ./scripts/main.sh $tmpdir/
COPY ./metricextension/me.config /usr/sbin/
RUN chmod 775 $tmpdir/*.sh; sync; $tmpdir/setup.sh
CMD [ "/opt/main.sh" ]
