# OpenTelemetry Collector Demo

This demo is a sample Python app to build the collector and exercise its tracing functionality. To build and run the demo, we will need to deploy the Python app and the collector and send traces to Azure Monitor. 

## Deploying the Python Application
#### 1. Create Docker image for Python app
`docker build -f Dockerfile.python . -t <name:tag>` <br/>
`docker push <name:tag>` <br/>

#### 2. Delete previous deployments
`kubectl delete -f python-deployment.yaml`<br/>

#### 3. Check deployment and related pods are deleted
`kubectl get deployments`<br/>
`kubectl get pods`<br/>

#### 4. Deploy
`kubectl apply -f python-deployment.yaml`<br/>

Once that has been completed, it is time to deploy the collector.

## Deploying the Collector

#### 1. Create Docker image for collector
`docker build -t <name:tag> .` <br/>
`docker push <name:tag>` <br/>

#### 2. Delete previous deployments
`kubectl delete -f omsagent-otel.yaml`<br/>

#### 3. Check deployment and related pods are deleted
`kubectl get deployments`
`kubectl get pods`<br/>

#### 4. Update & Deploy
Go to omsagent-otel.yaml (in kubernetes folder), and update "image" for the otel-collector deployment to be the docker image you just built. Then, run:
`kubectl apply -f omsagent-otel.yaml`<br/>

Finally, it is time to emit traces and see them in Application Insights.

## Run Python application and emit traces
#### 1. Execute commands from container
`kubectl exec -it <name of pod> /bin/bash`<br/>

#### 2. Generate spans
Note: if the probability sampler is enabled in the otel-collector, you might not see all the spans you send (adjust this number in collector's configuration, or disable it).
`curl http://localhost:5001`<br/>

Now, you should be able to go under the "Investigate" tab in Azure Portal and view your traces in Application Map or in Search. Take into account that it may take up to a few minutes before the traces show up.
