FROM ubuntu:21.04
MAINTAINER ascdetectionnadav@microsoft.com
LABEL vendor=Microsoft\ Corp \
    com.microsoft.product="Azure Defender"
ENV tmpdir /opt

RUN /usr/bin/apt-get update && /usr/bin/apt-get install -y wget curl

RUN wget -O /usr/bin/yq "https://github.com/mikefarah/yq/releases/download/3.3.2/yq_linux_amd64"
RUN chmod +x /usr/bin/yq
#RUN /usr/bin/snap install YQ
#RUN apk add yq
#RUN wget -O /$tmpdir/EnableAuditLogs.sh https://arcsecuritydev.blob.core.windows.net/extension/EnableAuditLogs.sh
COPY AuditLogsUpdateRules.yaml EnableAuditLogs.sh $tmpdir/
WORKDIR ${tmpdir}

RUN chmod 775 $tmpdir/*.sh; sync
CMD [ "/opt/EnableAuditLogs.sh" ]
