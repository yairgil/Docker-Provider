FROM ubuntu:18.04
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
RUN /usr/bin/apt-get update && /usr/bin/apt-get install -y libc-bin wget openssl curl sudo python-ctypes init-system-helpers  net-tools rsyslog cron vim dmidecode apt-transport-https gnupg ca-certificates locales && rm -rf /var/lib/apt/lists/*

WORKDIR ${tmpdir}

# set up apt repositories for fluent bit and ruby 2.6
RUN wget -qO - https://packages.fluentbit.io/fluentbit.key | sudo apt-key add -  && sudo echo "deb https://packages.fluentbit.io/ubuntu/xenial xenial main" >> /etc/apt/sources.list
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys F5DA5F09C3173AA6  &&  echo "deb http://ppa.launchpad.net/brightbox/ruby-ng/ubuntu bionic main" >> /etc/apt/sources.list

RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && dpkg-reconfigure --frontend=noninteractive locales  &&  update-locale LANG=en_US.UTF-8

# libcap2 is used to setcaps for ruby process to read /proc/env
RUN apt-get update  &&  apt-get install -y apt && DEBIAN_FRONTEND=noninteractive apt-get install -y inotify-tools  jq=1.5+dfsg-2  libcap2-bin ruby2.6 ruby2.6-dev gcc make  td-agent-bit=1.6.8  nano less

RUN gem install fluentd -v "1.12.2" --no-document  &&  fluentd --setup ./fluent  &&  gem install gyoku iso8601 --no-doc

# install oneagent - Official bits (10/7/2021)
RUN wget -q https://github.com/microsoft/Docker-Provider/releases/download/1.14/azure-mdsd_1.14.2-build.master.284_x86_64.deb  &&  /usr/bin/dpkg -i azure-mdsd*.deb

# install telegraf
RUN wget https://dl.influxdata.com/telegraf/releases/telegraf-1.18.0_linux_amd64.tar.gz  &&  tar -zxvf telegraf-1.18.0_linux_amd64.tar.gz  &&  rm -f telegraf-*.tar.gz  &&  mv /opt/telegraf-1.18.0/usr/bin/telegraf /opt/telegraf  &&  chmod 777 /opt/telegraf


ARG IMAGE_TAG=ciprod10132021
ENV AGENT_VERSION ${IMAGE_TAG}

COPY setup.sh main.sh defaultpromenvvariables defaultpromenvvariables-rs defaultpromenvvariables-sidecar mdsd.xml envmdsd logrotate.conf $tmpdir/

# log rotate conf for mdsd and can be extended for other log files as well
RUN cp -f logrotate.conf /etc/logrotate.d/ci-agent
RUN  cp -f mdsd.xml /etc/mdsd.d  &&  cp -f envmdsd /etc/mdsd.d  &&  rm -f azure-mdsd*.deb


# copy docker provider shell bundle to use the agent image
COPY ./Linux_ULINUX_1.0_x64_64_Release/docker-cimprov-*.*.*-*.x86_64.sh .
# Note: If you prefer remote destination, uncomment below line and comment above line
# wget https://github.com/microsoft/Docker-Provider/releases/download/10.0.0-1/docker-cimprov-10.0.0-1.universal.x86_64.sh

RUN chmod 775 $tmpdir/*.sh; sync; $tmpdir/setup.sh
CMD [ "/opt/main.sh" ]

