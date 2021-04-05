# Deployment Instructions

#### Step 0 : Create a kubernetes cluster (AKS would be the quickest & easiest)

#### Step 1 : You would need to create a MDM account and have name of the MDM account along with its cert & key file(s)

#### Step 2 : In your Kubernetes cluster where you want to collect prometheus metrics from, create kubernetes secret (called ```metricstore-secret``` [see below for example] ) for MDM account to which you want to ship metrics to, from your kubernetes cluster

```kubectl create secret tls metricstore-secret --cert=<full_path_to_my_cert_file_including_file_name> --key=<full_path_to_my_key_file_including_file_name> -n=kube-system```

Example :
```kubectl create secret tls metricstore-secret --cert=/mnt/e/prometheusmetricswork/genevacert/CIGenevaCert.pem  --key=/mnt/e/prometheusmetricswork/genevacert/CIGenevaCert.pem -n=kube-system```

#### Step 3 : Provide the default MDM account name in the config map (prometheus-collector-configmap.yaml), and apply the configmap to your kubernetes cluster (see below). Also provide scrape config as needed in the config map [By default, configmap has scrape config to scrape our reference service (weather service), which is scrapping our reference app in the ['app'](../app/prometheus-reference-app.yaml) folder - which you need to deploy in case you want to use default scrape config (```kubectl apply -f prometheus-reference-app.yml``` from 'app' folder )]

Below are the 3 sub-steps to do for this step -

- 3.1) Ensure below line in the configmap has your MDM account name (which will be used as default MDM account to send metrics to)
```account_name = "mymetricsacountname"```
- 3.2) Change the scrape config as needed in the default configmap, save the file, and apply the configmap to the cluster
```kubectl apply -f prometheus-collector-configmap.yaml```
- 3.3) Deploy the sample/reference app (weather app), if you are using default scrape config, which will scraoe the reference (weather) app in the [app](../app/prometheus-reference-app.yaml) folder
```kubectl apply -f prometheus-reference-app.yaml```

#### Step 4 : Deploy the prometheus collector (prometheuscollector.yaml) [Prometheus-collector will run in kube-system namespace as a singleton replica]
```kubectl apply -f prometheuscollector.yaml```
