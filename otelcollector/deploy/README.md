# Deployment Instructions

#### Step 1 : You would need to create a MDM account and have name of the MDM account along with its cert & key file(s)
#### Step 2 : In your Kubernetes cluster where you want to collect prometheus metrics from, create kubernetes secret (called ```metricstore-secret``` [see below for example] ) for MDM account 
```kubectl create secret tls metricstore-secret --cert=<full_path_to_my_cert_file_including_file_name> --key=<full_path_to_my_key_file_including_file_name> -n=kube-system```

Example :
```kubectl create secret tls metricstore-secret --cert=/mnt/e/prometheusmetricswork/genevacert/CIGenevaCert.pem  --key=/mnt/e/prometheusmetricswork/genevacert/CIGenevaCert.pem -n=kube-system```
#### Step 3 : Provide the default MDM account name in the config map (prometheus-collector-configmap.yaml), and apply the configmap to your kubernetes cluster. Also provide scrape config as needed [There is a default which is scrapping our reference app in the 'app' folder - which you need to deploy in case you want to use default scrape config]
Example :
```account_name = "mymetricsacountname"```

```kubectl apply -f prometheus-collector-configmap.yaml```
#### Step 4 : Deploy the prometheus collector (prometheuscollector.yaml) [Prometheus-collector will run in kube-system namespace as a singleton replica]
```kubectl apply -f prometheuscollector.yaml```
