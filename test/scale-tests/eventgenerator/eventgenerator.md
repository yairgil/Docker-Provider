# K8s Events generator

This generator 50 events per minute.

```
kubectl create namespace  eventgenerator
kubectl apply -f https://raw.githubusercontent.com/falcosecurity/event-generator/master/deployment/role-rolebinding-serviceaccount.yaml -n eventgenerator
kubectl apply -f https://raw.githubusercontent.com/falcosecurity/event-generator/master/deployment/event-generator.yaml -n eventgenerator
```
Reference - Reference - https://github.com/falcosecurity/event-generator