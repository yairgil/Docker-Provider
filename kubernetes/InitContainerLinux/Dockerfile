FROM python:3.8
MAINTAINER ascdetectionnadav@microsoft.com
LABEL vendor=Microsoft\ Corp \
    com.microsoft.product="Azure Defender"
ENV DEBIAN_FRONTEND noninteractive

WORKDIR /code

RUN apt-get update && \
    apt-get install -y \
    python3 \
    python3-pip

RUN pip install --no-cache-dir pyyaml
RUN pip install --no-cache-dir requests
RUN pip install --no-cache-dir mock

COPY EnableAuditLogs.py audit-policy.yaml .

CMD [ "python", "./EnableAuditLogs.py" ]
