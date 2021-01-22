#!/bin/bash


# generate logs for fluentd to pick up
while [[ 1 -le 2 ]]; do echo hi >> /var/log/constant_output/hi.txt; sleep 1; done &


# start fluentd
fluentd -c /opt/fluent/fluent.conf &


#start oneagent


echo "configuring mdsd..."
cat /opt/oneagent/envmdsd | while read line; do
        echo $line >> ~/.bashrc
done
source /opt/oneagent/envmdsd

echo "setting mdsd workspaceid & key for workspace:$CIWORKSPACE_id"
export CIWORKSPACE_id=$CIWORKSPACE_id
echo "export CIWORKSPACE_id=$CIWORKSPACE_id" >> ~/.bashrc
export CIWORKSPACE_key=$CIWORKSPACE_key
echo "export CIWORKSPACE_key=$CIWORKSPACE_key" >> ~/.bashrc

source ~/.bashrc

dpkg -l | grep mdsd | awk '{print $2 " " $3}'

echo "starting mdsd ..."
mdsd -l -e ${MDSD_LOG}/mdsd.err -w ${MDSD_LOG}/mdsd.warn -o ${MDSD_LOG}/mdsd.info -q ${MDSD_LOG}/mdsd.qos &


echo "************end oneagent log routing checks************"