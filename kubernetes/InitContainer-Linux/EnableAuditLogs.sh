#!/bin/bash

YamlFile="/etc/kubernetes/manifests/kube-apiserver.yaml"

# Ensure the yaml file exists
if [ ! -f $YamlFile ]; then
    echo "Could not find $YamlFile"
    exit 1
fi

# Check if audit logs is already enabled
if grep -q -- "--audit-policy-file=" "$YamlFile"; then
    echo "Found --audit-policy-file= flag in the kube-apiserver.yaml file"
    echo "Looks like you already have audit policy enabled on your cluster, exiting..."
    exit 0
fi

echo "Backing up the $YamlFile into $YamlFile.backup , in case problems will occur, switch between the backup and the api server yaml file"
cp $YamlFile "$YamlFile.backup"

# We can't use the below to update a root file even if running as sudo since yq is running strict mode:
# https://github.com/mikefarah/yq/issues/148 
# yq w -i -s "AuditLogsUpdateRules.yaml" $YamlFile
# Workaround for now:
TempYamlFile="kube-apiserver.temp.yaml"
cp $YamlFile $TempYamlFile
yq w -i -s "AuditLogsUpdateRules.yaml" $TempYamlFile
cp $TempYamlFile $YamlFile # Not using mv to preserve target's permissions

# Cleanup
echo "cleaning up, removing temp files"
rm $TempYamlFile
echo "temp files removed"

echo "Script finished"