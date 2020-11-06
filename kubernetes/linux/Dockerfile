FROM ubuntu:18.04
MAINTAINER OMSContainers@microsoft.com
LABEL vendor=Microsoft\ Corp \
    com.microsoft.product="Azure Monitor for containers"
ARG IMAGE_TAG=ciprod10272020
ENV AGENT_VERSION ${IMAGE_TAG}
ENV tmpdir /opt
ENV APPLICATIONINSIGHTS_AUTH NzAwZGM5OGYtYTdhZC00NThkLWI5NWMtMjA3ZjM3NmM3YmRi
ENV MALLOC_ARENA_MAX 2
ENV HOST_MOUNT_PREFIX /hostfs
ENV HOST_PROC /hostfs/proc
ENV HOST_SYS /hostfs/sys
ENV HOST_ETC /hostfs/etc
ENV HOST_VAR /hostfs/var
ENV AZMON_COLLECT_ENV False
ENV KUBE_CLIENT_BACKOFF_BASE 1
ENV KUBE_CLIENT_BACKOFF_DURATION 0
RUN /usr/bin/apt-get update && /usr/bin/apt-get install -y build-essential libc-bin wget openssl curl sudo python-ctypes init-system-helpers  net-tools rsyslog cron vim dmidecode apt-transport-https gnupg && rm -rf /var/lib/apt/lists/*
COPY setup.sh main.sh defaultpromenvvariables defaultpromenvvariables-rs mdsd.xml envmdsd $tmpdir/
WORKDIR ${tmpdir}

# jemalloc
RUN wget -O /tmp/jemalloc-5.2.1.tar.bz2 https://github.com/jemalloc/jemalloc/releases/download/5.2.1/jemalloc-5.2.1.tar.bz2 \
 && cd /tmp && tar -xjf jemalloc-5.2.1.tar.bz2 && cd jemalloc-5.2.1/ \
 && ./configure && make \
 && mv lib/libjemalloc.so.2 /usr/lib \
 && rm -rf /tmp/* \
 && /usr/bin/apt-get remove -y build-essential \
 && /usr/bin/apt-get autoremove -y

ENV LD_PRELOAD="/usr/lib/libjemalloc.so.2"

# copy docker provider shell bundle to use the agent image
COPY ./Linux_ULINUX_1.0_x64_64_Release/docker-cimprov-*.*.*-*.x86_64.sh .
# Note: If you prefer remote destination, uncomment below line and comment above line
# wget https://github.com/microsoft/Docker-Provider/releases/download/10.0.0-1/docker-cimprov-10.0.0-1.universal.x86_64.sh

RUN chmod 775 $tmpdir/*.sh; sync; $tmpdir/setup.sh
CMD [ "/opt/main.sh" ]
