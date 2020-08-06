# OpenTelemetry Collector Demo

This demo consists of a sample Python application that sends trace data to the OpenTelemetry collector, which is sitting on the OMSagent. To build and run the demo, we will need to deploy the Python application and the collector, which will send the data to Azure Monitor. The data can then be viewed in Application Map and Log Analytics.

## Deploying the Python Application
#### 1. Create Docker image for Python app
Go to `source/opentelemetry-collector-contrib/examples/tracing/python` and run:
```
docker build -f Dockerfile.python . -t <repo>/<image>:<tag>
docker push <repo>/<image>:<tag>
```

#### 2. Deploy application
```
kubectl apply -f python-deployment.yaml
```

#### 3. Check deployment and related pods are running
Confirm the python deployment and related pod exist after running:
```
kubectl get deployments
kubectl get pods
```
Once that has been completed, it is time to deploy the collector. 

## Build collector binaries
Go to the correct directory and enable the build of the collector.
```
cd Docker-Provider/build/linux
make clean && make GOOS=linux GOARCH=amd64 OT_COLLECTOR_ENABLE=1
```

## Build & Deploy the Collector

#### 1. Create Docker image for collector
```
cd Docker-Provider/kubernetes/otel-collector
docker build -t <repo>/<image>:<tag>
docker push <repo>/<image>:<tag>
```

#### 2. Update & Deploy
Go to the directory that contains the `omsagent-otel.yaml`.
```
cd Docker-Provider/kubernetes
```
Update "image" for the otel-collector deployment to be the docker image you just built in `omsagent-otel.yaml`. Then, run:
```
kubectl apply -f omsagent-otel.yaml
```

#### 3. Check deployment and related pods are running
Confirm the deployment and related pods exist after running:
```
kubectl get deployments
kubectl get pods
```

#### 4. 
Finally, it is time to emit traces and see them in Application Insights.

## Run Python application and emit traces
#### 1. Execute commands from container
```
kubectl exec -it <name of pod> /bin/bash <br/>
```
#### 2. Generate spans
Note: if the probability sampler is enabled in the otel-collector, you might not see all the spans you send (adjust this number in collector's configuration, or disable it).
`curl http://localhost:5001`<br/>

Now, you should be able to go under the "Investigate" tab in Azure Portal and view your traces in Application Map or in Log Analytics. Take into account that it may take up to a few minutes before the traces show up.
