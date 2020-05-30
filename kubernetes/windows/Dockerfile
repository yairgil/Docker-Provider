FROM mcr.microsoft.com/azuremonitor/containerinsights/ciprod:winakslogbase-04212020

SHELL ["powershell"]

ENV tmpdir /opt/omsagentwindows/scripts/powershell

WORKDIR /opt/omsagentwindows/scripts/powershell

# copy certificate generator binaries zip
COPY ./omsagentwindows/*.zip /opt/omsagentwindows/

COPY setup.ps1 /opt/omsagentwindows/scripts/powershell
RUN ./setup.ps1

COPY main.ps1 /opt/omsagentwindows/scripts/powershell
COPY ./omsagentwindows/scripts/filesystemwatcher.ps1 /opt/omsagentwindows/scripts/powershell
COPY ./omsagentwindows/scripts/livenessprobe.cmd /opt/omsagentwindows/scripts/cmd/

# copy ruby scripts to /opt folder
COPY ./omsagentwindows/scripts/*.rb /opt/omsagentwindows/scripts/ruby/

# copy out_oms.so file
COPY ./omsagentwindows/out_oms.so /opt/omsagentwindows/out_oms.so

# copy fluent, fluent-bit and out_oms conf files
COPY ./omsagentwindows/conf/fluent.conf /etc/fluent/
COPY ./omsagentwindows/conf/fluent-bit.conf /etc/fluent-bit
COPY ./omsagentwindows/conf/out_oms.conf /etc/omsagentwindows

# copy keepcert alive ruby scripts
COPY ./omsagentwindows/scripts/rubyKeepCertificateAlive/*.rb /etc/fluent/plugin/

ENV AGENT_VERSION "win-ciprod05262020-2"
ENV OS_TYPE "windows"
ENV APPLICATIONINSIGHTS_AUTH "NzAwZGM5OGYtYTdhZC00NThkLWI5NWMtMjA3ZjM3NmM3YmRi"
ENV AZMON_COLLECT_ENV False
ENV CI_CERT_LOCATION "C://oms.crt"
ENV CI_KEY_LOCATION "C://oms.key"

ENTRYPOINT ["powershell", "C:\\opt\\omsagentwindows\\scripts\\powershell\\main.ps1"]
