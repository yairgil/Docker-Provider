#!/bin/bash


# generate logs for fluentd to pick up
# while [[ 1 -le 2 ]]; do echo hi >> /var/log/constant_output/hi.txt; sleep 1; done &


# start fluentd
# fluentd -c /opt/fluent/fluentd4.conf &


cat /opt/workspace_creds | while read line; do
        echo $line >> ~/.bashrc
done
source /opt/workspace_creds



# generate the ODS certificate
cd /opt/certificategenerator
export WSID=$CIWORKSPACE_id
mkdir -p C:/etc/omsagent-secret
echo $CIWORKSPACE_key >> C:/etc/omsagent-secret/KEY
dotnet run
cp C:/oms.crt /etc/opt/microsoft/omsagent/certs/oms.crt
cp C:/oms.key /etc/opt/microsoft/omsagent/certs/oms.key
cd ..




export MDSD_ROLE_PREFIX="/var/run/mdsd/default"
#export MDSD_OPTIONS="-d -A -r ${MDSD_ROLE_PREFIX}"
mkdir /var/opt/microsoft/linuxmonagent/log
export OMS_CERT_PATH="/etc/opt/microsoft/omsagent/certs/oms.crt"
export OMS_CERT_KEY_PATH="/etc/opt/microsoft/omsagent/certs/oms.key"
export MDSD_OPTIONS="-A -c /etc/mdsd.d/mdsd.xml -r ${MDSD_ROLE_PREFIX} -S ${MDSD_SPOOL_DIRECTORY}/eh -e ${MDSD_LOG}/mdsd.err -w ${MDSD_LOG}/mdsd.warn -o ${MDSD_LOG}/mdsd.info -q ${MDSD_LOG}/mdsd.qos"
export ENABLE_ODS_TELEMETRY_FILE="true"
export HOSTNAME_OVERRIDE="${NODE_NAME}"
export MDSD_TCMALLOC_RELEASE_FREQ_SEC=1
export MDSD_COMPRESSION_ALGORITHM=LZ4
export SSL_CERT_DIR="/etc/ssl/certs"





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
mdsd -l -c /opt/mdsd.xml -e ${MDSD_LOG}/mdsd.err -w ${MDSD_LOG}/mdsd.warn -o ${MDSD_LOG}/mdsd.info -q ${MDSD_LOG}/mdsd.qos &


echo "************end oneagent log routing checks************"




