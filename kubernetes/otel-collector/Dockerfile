
FROM alpine:latest as certs
RUN apk --update add ca-certificates

ENV APPLICATIONINSIGHTS_AUTH NzAwZGM5OGYtYTdhZC00NThkLWI5NWMtMjA3ZjM3NmM3YmRi

FROM scratch
COPY --from=certs /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/ca-certificates.crt
COPY otelcontribcol_linux_amd64 /otelcontribcol 
COPY omsagent-otel-config.yaml /etc/omsagent-otel-config.yaml
ENTRYPOINT ["/otelcontribcol"]
EXPOSE 55680 55679 55678

