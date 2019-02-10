#!/bin/sh
count403=$(grep -iF  "[azure_monitor]: failed to write batch: [403] 403 Forbidden" /var/opt/microsoft/docker-cimprov/log/telegraf.log | wc -l | tr -d '\n')
echo "telegraf,AKS_RESOURCE_ID=${AKS_RESOURCE_ID}, 403countTotal=${count403}i"