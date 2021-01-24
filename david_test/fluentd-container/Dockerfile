FROM davidmichelman/td-agent-base-base_image:latest
WORKDIR /opt
RUN useradd omsagent && usermod -aG sudo omsagent
# # for TD-agent
# RUN curl -L https://toolbelt.treasuredata.com/sh/install-ubuntu-bionic-td-agent4.sh | sh

# for fluentd (not td-agent)
RUN gem install fluentd --no-doc
RUN fluentd --setup ./fluent

# RUN rmdir /etc/td-agent/plugin
# RUN ln -s /opt/microsoft/omsagent/plugin /etc/td-agent/plugin
# RUN rmdir /etc/td-agent/td-agent.conf
# RUN ln -s /etc/opt/microsoft/omsagent/sysconf/omsagent.d/container.conf /etc/td-agent/td-agent.conf

COPY td-agent.conf /opt/fluent/fluent.conf
COPY envmdsd /opt/oneagent/envmdsd
COPY workspace_creds /opt/workspace_creds
COPY mdsd.xml /opt/mdsd.xml
COPY main.sh /opt/main.sh
RUN chmod +x /opt/main.sh

COPY certificategenerator/ /opt/certificategenerator/

RUN mkdir /var/log/constant_output

CMD ["/bin/bash"]


# to start fluentd
# fluentd -c /opt/fluent/fluent.conf &

# to start writing to files: while [[ 1 -le 2 ]]; do echo hi >> /var/log/constant_output/hi.txt; sleep 1; done &


# To start/stop TD agent
# sudo /etc/init.d/td-agent start
# sudo /etc/init.d/td-agent stop
# sudo /etc/init.d/td-agent restart
# sudo /etc/init.d/td-agent status