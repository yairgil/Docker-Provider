#!/bin/bash

#test to exit non zero value if omsagent is not running
(ps -ef | grep omsagent- | grep -v "grep")
if [ $? -ne 0 ]
then
 echo " omsagent is not running" > /dev/termination-log
 exit 1
fi

#optionally test to exit non zero value if oneagent is not running
if [ -e "/opt/AZMON_CONTAINER_LOGS_EFFECTIVE_ROUTE_V2" ]; then
  (ps -ef | grep "mdsd -l" | grep -v "grep")
  if [ $? -ne 0 ]
  then
   echo "oneagent is not running" > /dev/termination-log
   exit 1
  fi
fi

#test to exit non zero value if fluentbit is not running
(ps -ef | grep td-agent-bit | grep -v "grep")
if [ $? -ne 0 ]
then
 echo "Fluentbit is not running" > /dev/termination-log
 exit 1
fi

#optionally test to exit non zero value if otelcollector is not running
if [ -e "/etc/config/kube.conf" ] && [ -e "/opt/OTELCOLLECTOR" ]; then
  (ps -ef | grep otelcollector | grep -v "grep")
  if [ $? -ne 0 ]
  then
    echo "OpenTelemetryCollector is not running" > /dev/termination-log
    exit 1
  fi
fi

if [ ! -s "inotifyoutput.txt" ]
then
  # inotifyoutput file is empty and the grep commands for omsagent and td-agent-bit succeeded
  exit 0
else
  if [ -s "inotifyoutput.txt" ]
  then
    # inotifyoutput file has data(config map was applied)
    echo "inotifyoutput.txt has been updated - config changed" > /dev/termination-log
    exit 1
  fi
fi
