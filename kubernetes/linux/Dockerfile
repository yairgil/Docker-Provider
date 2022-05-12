FROM cblmariner2preview.azurecr.io/base/core:2.0 as builder
MAINTAINER OMSContainers@microsoft.com
LABEL vendor=Microsoft\ Corp \
    com.microsoft.product="Azure Monitor for containers"
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
ENV RUBY_GC_HEAP_OLDOBJECT_LIMIT_FACTOR 0.9

COPY logrotate /etc/cron.daily/logrotate
COPY crontab /etc/crontab
# RUN /usr/bin/apt-get update && /usr/bin/apt-get install -y libc-bin wget openssl curl sudo python-ctypes init-system-helpers  net-tools rsyslog cron vim dmidecode apt-transport-https gnupg && rm -rf /var/lib/apt/lists/*
RUN tdnf install -y dnf && dnf clean expire-cache && dnf check-update
RUN tdnf install -y wget openssl curl sudo net-tools cronie rsyslog vim dmidecode gnupg logrotate busybox && rm -rf /var/lib/apt/lists/*
RUN mkdir /busybin && busybox --install /busybin

COPY setup.sh main.sh defaultpromenvvariables defaultpromenvvariables-rs defaultpromenvvariables-sidecar mdsd.xml envmdsd logrotate.conf $tmpdir/
COPY azurecore.repo /etc/yum.repos.d/

RUN chmod 775 /etc/cron.daily/logrotate

ARG IMAGE_TAG=ciprod03172022
ENV AGENT_VERSION ${IMAGE_TAG}

WORKDIR ${tmpdir}

# copy docker provider shell bundle to use the agent image
COPY ./Linux_ULINUX_1.0_*_64_Release/docker-cimprov-*.*.*-*.*.*.sh .
# Note: If you prefer remote destination, uncomment below line and comment above line
# wget https://github.com/microsoft/Docker-Provider/releases/download/10.0.0-1/docker-cimprov-10.0.0-1.universal.x86_64.sh

RUN chmod 775 /etc/cron.daily/logrotate
RUN chmod 775 $tmpdir/*.sh; sync; $tmpdir/setup.sh

FROM cblmariner2preview.azurecr.io/distroless/base:2.0
ENV tmpdir /opt
ENV PATH="/busybin:${PATH}"
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
ENV RUBY_GC_HEAP_OLDOBJECT_LIMIT_FACTOR 0.9
WORKDIR ${tmpdir}


# files
COPY --from=builder /opt /opt
COPY --from=builder /etc /etc
COPY --from=builder /busybin /busybin
COPY --from=builder /var/opt/microsoft /var/opt/microsoft
COPY --from=builder /var/lib/logrotate /var/lib/logrotate

# executables
COPY --from=builder /usr/bin/bash /usr/bin/bash
COPY --from=builder /usr/bin/ruby /usr/bin/ruby
COPY --from=builder /usr/lib/ruby /usr/lib/ruby
COPY --from=builder /usr/bin/inotifywait /usr/bin/inotifywait
COPY --from=builder /usr/sbin/busybox /usr/sbin/busybox
COPY --from=builder /usr/bin/fluent-bit /usr/bin/fluent-bit
COPY --from=builder /usr/bin/telegraf /usr/bin/telegraf
COPY --from=builder /usr/sbin/crond /usr/sbin/crond
COPY --from=builder /usr/bin/vim /usr/bin/vim
COPY --from=builder /usr/share/vim /usr/share/vim
COPY --from=builder /usr/sbin/mdsd /usr/sbin/mdsd
COPY --from=builder /usr/sbin/telegraf /usr/sbin/telegraf
COPY --from=builder /usr/sbin/logrotate /usr/sbin/logrotate
COPY --from=builder /usr/bin/logger /usr/bin/logger
COPY --from=builder /usr/sbin/setcap /usr/sbin/setcap
COPY --from=builder /usr/bin/curl /usr/bin/curl
COPY --from=builder /usr/bin/jq /usr/bin/jq
COPY --from=builder /usr/bin/base64 /usr/bin/base64

# bash dependencies
COPY --from=builder /lib/libreadline.so.8 /lib/
COPY --from=builder /lib/libc.so.6 /lib/
COPY --from=builder /usr/lib/libncursesw.so.6 /usr/lib/libtinfo.so.6 /usr/lib/
#ruby dependencies
COPY --from=builder /usr/lib/libruby.so.2.7 /usr/lib/libc.so.6  /usr/lib/libgmp.so.10 /usr/lib/libcrypt.so.1 /usr/lib/libm.so.6 /usr/lib/
# inotifywait dependencies
COPY --from=builder /lib/libinotifytools.so.0 /lib/
# fluent-bit dependencies
COPY --from=builder /lib/libssl.so.1.1 /lib/libcrypto.so.1.1 /lib/libsystemd.so.0 /lib/libm.so.6 /lib/libgcc_s.so.1 /lib/liblzma.so.5 /lib/liblz4.so.1 /lib/libcap.so.2 /lib/libgcrypt.so.20 /lib/libgpg-error.so.0 /lib/
# crond dependencies
COPY --from=builder /lib/libselinux.so.1 /lib/libpam.so.0 /lib/libpcre.so.1 /lib/libaudit.so.1 /lib/
# vim dependencies
COPY --from=builder /lib/libtinfo.so.6 /lib/
# mdsd dependencies
COPY --from=builder /usr/lib/libdl.so.2 /usr/lib/librt.so.1 /usr/lib/libpthread.so.0 /usr/lib/libm.so.6 /usr/lib/libstdc++.so.6 /usr/lib/libgcc_s.so.1 /usr/lib/
COPY --from=builder /usr/local/lib/libtcmalloc_minimal.so.4 /usr/local/lib/
# logrotate dependencies
COPY --from=builder /lib/libpopt.so.0  /lib/
# jq dependencies
COPY --from=builder /lib/libjq.so.1 /lib/libonig.so.5 /lib/
# curl dependencies
COPY --from=builder /lib/libcurl.so.4 /lib/libz.so.1 /lib/libssh2.so.1 /lib/libgssapi_krb5.so.2 /lib/libzstd.so.1 /lib/
COPY --from=builder /usr/lib/libkrb5.so.3 /usr/lib/libk5crypto.so.3 /usr/lib/libcom_err.so.2 /usr/lib/libkrb5support.so.0 /usr/lib/libresolv.so.2 /usr/lib/

RUN [ "/bin/bash", "-c", "/etc/cron.daily/logrotate" ]
CMD [ "/opt/main.sh" ]

