#!/bin/bash
set -e
TEMP_DIR=temp-$RANDOM
DEFAULT_ONPREM_K8S_CLUSTER="onprem-k8s-cluster-test"

install-kind()
{
sudo curl -Lo ./kind https://kind.sigs.k8s.io/dl/v0.8.1/kind-linux-amd64
sudo chmod +x ./kind
sudo mv ./kind /usr/local/bin/kind
}

create_cluster()
{
sudo touch ~/${TEMP_DIR}/kind-config.yaml
sudo chmod 777~/${TEMP_DIR}/kind-config.yaml
cat >> kind-config.yaml <<EOL
kind: Cluster
apiVersion: kind.sigs.k8s.io/v1alpha3
nodes:
 - role: control-plane
 - role: worker
EOL
sudo kind create cluster --config ~/${TEMP_DIR}/kind-config.yaml  --name $ClusterName
}


for ARGUMENT in "$@"
do
   KEY=$(echo $ARGUMENT | cut -f1 -d=)
   VALUE=$(echo $ARGUMENT | cut -f2 -d=)

   case "$KEY" in
           ClusterName) ClusterName=$VALUE ;;
           *)
    esac
done

if [ -z $ClusterName ]; then
  ClusterName=$DEFAULT_ONPREM_K8S_CLUSTER
fi

echo "creating kind k8 cluster ..."
cd ~
echo "creating temp directory":$TEMP_DIR
sudo mkdir $TEMP_DIR && cd $TEMP_DIR

echo "download and install kind"
install-kind

echo "creating cluster: ${ClusterName}"
create_cluster

echo "creating kind k8 cluster completed."