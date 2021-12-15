FROM ubuntu:18.04 AS base_image
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
RUN apt-get update  &&  apt-get install -y apt && DEBIAN_FRONTEND=noninteractive apt-get install -y inotify-tools jq=1.5+dfsg-2 libcap2-bin ruby2.6 ruby2.6-dev gcc make td-agent-bit=1.6.8 nano less rpm librpm-dev sysstat

RUN gem install fluentd -v "1.12.2" --no-document  &&  fluentd --setup ./fluent  &&  gem install gyoku iso8601 --no-doc

# install oneagent - Official bits (10/7/2021)
RUN wget -q https://github.com/microsoft/Docker-Provider/releases/download/1.14/azure-mdsd_1.14.2-build.master.284_x86_64.deb  &&  /usr/bin/dpkg -i azure-mdsd*.deb

# install telegraf
RUN wget -q https://dl.influxdata.com/telegraf/releases/telegraf-1.18.0_linux_amd64.tar.gz  &&  tar -zxvf telegraf-1.18.0_linux_amd64.tar.gz  &&  rm -f telegraf-*.tar.gz  &&  mv /opt/telegraf-1.18.0/usr/bin/telegraf /opt/telegraf  &&  chmod 777 /opt/telegraf

# install golang and a go debugger
#TODO: don't include this in production image
RUN wget -q https://go.dev/dl/go1.17.3.linux-amd64.tar.gz && rm -rf /usr/local/go && tar -C /usr/local -xzf go1.17.3.linux-amd64.tar.gz && rm go1.17.3.linux-amd64.tar.gz
# changing the path for all shells (interractive and non-interractive) is insanely hard. Creating links in places already in the path is much easier
RUN ln -s /usr/local/go/bin/go /usr/bin/go && ln -s /usr/local/go/bin/gofmt /usr/bin/gofmt
RUN /usr/local/go/bin/go install github.com/go-delve/delve/cmd/dlv@latest && echo alias dlv=/root/go/bin/dlv >> ~/.bashrc


# do the go build in a separate container
# Go downloads a lot of dependencies for the build (the build container is >1gb). If we build in a separate
# container then only copy the executable into the final output container we can keep all that out of the final image
# (which saves a lot of time when pushing/pulling the image)
FROM base_image AS go_build_env

# COPY kubernetes/linux/get_go_deps.sh /
# RUN /get_go_deps.sh
ADD source/plugins/go/src/go.mod /src/source/plugins/go/src/go.mod
ADD source/plugins/go/src/go.sum /src/source/plugins/go/src/go.sum
RUN cd /src/source/plugins/go/src/ && go mod download -x
RUN cd /src/source/plugins/go/src/ && go list -f '{{.Path}}/...' -m all | tail -n +2 | CGO_ENABLED=0 GOOS=linux xargs go build -v -installsuffix cgo -i; echo done
 
COPY build /src/build
COPY source /src/source
# COPY kubernetes /src/kubernetes

RUN cd /src/build/linux && make fluentbitplugin
# RUN cd /src/build/linux && go build -ldflags "-X main.revision=16.0.0.0 -X main.builddate=$(date +%Y-%M-%dT%H%M%SZ)" -buildmode=c-shared -o out_oms.so .



FROM base_image

ARG IMAGE_TAG=ciprod10132021
ENV AGENT_VERSION ${IMAGE_TAG}

COPY kubernetes/linux/setup.sh kubernetes/linux/get_go_deps.sh kubernetes/linux/main.sh kubernetes/linux/defaultpromenvvariables kubernetes/linux/defaultpromenvvariables-rs kubernetes/linux/defaultpromenvvariables-sidecar kubernetes/linux/mdsd.xml kubernetes/linux/envmdsd kubernetes/linux/logrotate.conf $tmpdir/

# log rotate conf for mdsd and can be extended for other log files as well
RUN cp -f logrotate.conf /etc/logrotate.d/ci-agent
RUN  cp -f mdsd.xml /etc/mdsd.d  &&  cp -f envmdsd /etc/mdsd.d  &&  rm -f azure-mdsd*.deb

# build the docker provider
RUN mkdir /src
COPY build /src/build
COPY source /src/source
COPY kubernetes /src/kubernetes

RUN mkdir /src/intermediate/ && mkdir /src/intermediate/Linux_ULINUX_1.0_x64_64_Release
COPY --from=go_build_env /src/intermediate/Linux_ULINUX_1.0_x64_64_Release/out_oms.so /src/intermediate/Linux_ULINUX_1.0_x64_64_Release/out_oms.so

# this is now done in setup.sh
# copy docker provider shell bundle to use the agent image
# COPY ./Linux_ULINUX_1.0_x64_64_Release/docker-cimprov-*.*.*-*.x86_64.sh .
# Note: If you prefer remote destination, uncomment below line and comment above line
# wget https://github.com/microsoft/Docker-Provider/releases/download/10.0.0-1/docker-cimprov-10.0.0-1.universal.x86_64.sh

RUN chmod 775 $tmpdir/*.sh; sync; $tmpdir/setup.sh
CMD [ "/opt/main.sh" ]

