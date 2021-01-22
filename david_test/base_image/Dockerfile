FROM ubuntu:18.04
WORKDIR /opt
# RUN apt-get update && apt-get install gnupg2 nano less curl sudo rubygems ruby-dev gcc make init-system-helpers  -y
RUN apt-get update && apt-get upgrade -y
RUN apt-get install gnupg2 nano less curl sudo init-system-helpers apt-utils rubygems ruby-dev gcc make wget ucf -y

#install oneagent - Official bits (10/18)
RUN wget https://github.com/microsoft/Docker-Provider/releases/download/10182020-oneagent/azure-mdsd_1.5.126-build.master.99_x86_64.deb 
RUN /usr/bin/dpkg -i azure-mdsd*.deb

