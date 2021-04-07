# Deployment Instructions

#### Step 0 : Create a kubernetes cluster (AKS would be the quickest & easiest)

#### Step 1 : Create a MDM account and have name of the MDM account along with its cert & key file(s)

#### Step 2 : In your Kubernetes cluster where you want to collect prometheus metrics from, create kubernetes secret (called ```metricstore-secret``` [see below for example] ) for MDM account to which you want to ship metrics to, from your kubernetes cluster

```kubectl create secret tls metricstore-secret --cert=<full_path_to_my_cert_file_including_file_name> --key=<full_path_to_my_key_file_including_file_name> -n=kube-system```

Example :
```kubectl create secret tls metricstore-secret --cert=/mnt/e/prometheusmetricswork/genevacert/CIGenevaCert.pem  --key=/mnt/e/prometheusmetricswork/genevacert/CIGenevaCert.pem -n=kube-system```

#### Step 3 : Provide the default MDM account name in the config map (prometheus-collector-configmap.yaml), optionally configure if you'd like scrape settings for kubelet, coredns, etc. included, and apply the configmap to your kubernetes cluster (see below).
Below are the sub-steps for this -
- 3.1) Ensure line below in the configmap has your MDM account name (which will be used as default MDM account to send metrics to):
  ``` 
  prometheus-collector-settings: |-
    [prometheus_collector_settings.default_metric_account]
      account_name = "mymetricaccountname"
  ```
- 3.2) Specify if you'd like kubelet or coredns scraping (no need to include in your prometheus config)
  ```
  default-scrape-settings: |-
    [default_scrape_settings]
      kubelet_enabled = true
      coredns_enabled = true
  ```
- 3.3) Change the scrape config as needed in the default configmap, save the file, and apply the configmap to the cluster
  ```
  kubectl apply -f prometheus-collector-configmap.yaml
  ```

#### Step 4 : Provide the prometheus scrape config as needed in the prometheus configmap. See [configuration.md](../configuration.md) for more info on the prometheus config There are two ways of doing so:
- Make changes as needed to the prometheus-config.yaml configmap and apply:
  ```
  kubectl apply -f prometheus-collector-configmap.yaml
  ```
  -  By default and for testing purposes, the configmap has a scrape config to scrape our reference service (weather service), which is located in the [app](../app/prometheus-reference-app.yaml) folder. If you'd like to use the default, you need to deploy by running the folowing command while in the [app](../app/prometheus-reference-app.yaml) folder:
      ```
      kubectl apply -f prometheus-reference-app.yaml
      ```
- If you have your own prometheus yaml and want to use that without having to paste into the configmap, rename the file to prometheus-config and run:
  ```
  kubectl create configmap prometheus-config --from-file=prometheus-config -n kube-system
  ```

#### Step 5 : Deploy the prometheus collector (prometheuscollector.yaml) [Prometheus-collector will run in kube-system namespace as a singleton replica]
```
kubectl apply -f prometheuscollector.yaml
```
