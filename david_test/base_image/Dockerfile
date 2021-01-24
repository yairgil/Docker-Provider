FROM ubuntu:18.04
WORKDIR /opt
# RUN apt-get update && apt-get install gnupg2 nano less curl sudo rubygems ruby-dev gcc make init-system-helpers  -y
RUN apt-get update && apt-get upgrade -y
RUN apt-get install gnupg2 nano less curl sudo init-system-helpers apt-utils rubygems ruby-dev gcc make wget ucf -y

#install oneagent - Official bits (10/18)
RUN wget https://github.com/microsoft/Docker-Provider/releases/download/10182020-oneagent/azure-mdsd_1.5.126-build.master.99_x86_64.deb 
RUN /usr/bin/dpkg -i azure-mdsd*.deb


# install dotnet core 3.1
RUN wget https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb && sudo dpkg -i packages-microsoft-prod.deb
RUN sudo apt-get update && sudo apt-get install -y apt-transport-https && sudo apt-get update && sudo apt-get install -y dotnet-sdk-3.1

