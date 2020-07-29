
FROM alpine:latest as certs
RUN apk --update add ca-certificates

ENV APPLICATIONINSIGHTS_AUTH YzdiZDM4OTQtNTZjMS00N2Q3LWE4ZTYtOTg0NDBlMjAxOGI2

FROM scratch
COPY --from=certs /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/ca-certificates.crt
COPY otelcontribcol_linux_amd64 /otelcontribcol 
COPY otel-collector-config.yml /etc/otel-collector-config.yml
ENTRYPOINT ["/otelcontribcol"]
EXPOSE 55680 55679 55678

