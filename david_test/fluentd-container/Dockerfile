FROM: ubuntu:18.04
WORKDIR /opt
RUN apt update && apt install nano less curl sudo -y
RUN useradd omsagent && usermod -aG sudo omsagent
RUN curl -L https://toolbelt.treasuredata.com/sh/install-ubuntu-bionic-td-agent4.sh | sh
RUN rmdir /etc/td-agent/plugin
RUN ln -s /opt/microsoft/omsagent/plugin /etc/td-agent/plugin
RUN rmdir /etc/td-agent/td-agent.conf
RUN ln -s /etc/opt/microsoft/omsagent/sysconf/omsagent.d/container.conf /etc/td-agent/td-agent.conf

# To start/stop TD agent
# sudo /etc/init.d/td-agent start
# sudo /etc/init.d/td-agent stop
# sudo /etc/init.d/td-agent restart
# sudo /etc/init.d/td-agent status