import sys
import yaml
import requests
import time
import copy

apiServerFilePath="/etc/kubernetes/manifests/kube-apiserver.yaml"
backUpFilePath="kube-apiserver.yaml.backup"
volumeMountsToBeAdded=[{
    "mountPath": "/etc/kubernetes",
    "name": "etc-kubernetes",
    "readOnly": True
},{
    "mountPath": "/var/log/kube-apiserver",
    "name": "var-log-kubeapi"
}]

hostPathsToBeAdded=[{
    "name": "etc-kubernetes",
    "hostPath": {
        "path": "/etc/kubernetes",
        "type": "Directory",
    }
},{
    "name": "var-log-kubeapi",
    "hostPath": {
        "path": "/var/log/kube-apiserver",
        "type": "DirectoryOrCreate",
    }
}]

def VerifyAuditLogsAreNotEnabled(commands, volumeMounts, hostPaths):
    commandsJoined = ', '.join(commands)
    AuditLogsCommands = [ "--audit-policy-file", "--audit-log-path", "--audit-log-format", "--audit-log-maxage", "--audit-log-maxbackup", "--audit-log-maxsize" ]
    for AuditLogsCommand in AuditLogsCommands:
        if AuditLogsCommand in commandsJoined:
            print ("Potential collision detected: command: "+ AuditLogsCommand)
            return False

    for currentVolumeMount in volumeMounts:
        for volumeMountToBeAdded in volumeMountsToBeAdded:
            if currentVolumeMount["name"] == volumeMountToBeAdded["name"] or currentVolumeMount["mountPath"] == volumeMountToBeAdded["mountPath"]:
                print ("Potential collision detected: volumeMount: "+ volumeMountToBeAdded)
                return False

    for currentHostPath in hostPaths:
        for hostPathToBeAdded in hostPathsToBeAdded:
            if currentHostPath["name"] == hostPathToBeAdded["name"] or currentHostPath["hostPath"]["path"] == hostPathToBeAdded["hostPath"]["path"]:
                print ("Potential collision detected: hostPath: "+ hostPathToBeAdded)
                return False

    return True

def EnableAuditLogs(commands, volumeMounts, hostPaths):
    commands.append("--audit-blabla-file=/etc/kubernetes/audit-policy.yaml")
    commands.append("--audit-log-path=/var/log/kube-apiserver/audit")
    commands.append("--audit-log-format=json")
    commands.append("--audit-log-maxage=1")
    commands.append("--audit-log-maxbackup=10")
    commands.append("--audit-log-maxsize=1")

    volumeMounts.extend(volumeMountsToBeAdded)
    hostPaths.extend(hostPathsToBeAdded)

def VerifyApiServerIsStillAlive():
    # Get the bearer token of the attached service account
    bearerTokenFile = open("/var/run/secrets/kubernetes.io/serviceaccount/token")
    bearerToken = bearerTokenFile.read()
    bearerTokenFile.close()

    header = {'Authorization': 'Bearer ' + bearerToken}
    url = 'https://kubernetes.default.svc.cluster.local/api/v1/namespaces/kube-system/pods'
        
    currentAttempt = 0
    maxAttempts = 10
    while currentAttempt < maxAttempts:
        time.sleep(10)
        try:
            response = requests.get(url, headers=header, verify="/var/run/secrets/kubernetes.io/serviceaccount/ca.crt")
            response = response.json()
            response = response["items"]
            # Get the API server item
            kubeApiServerItem = next(item for item in response if item["metadata"]["name"].startswith("kube-apiserver"))
            apiServerStatuses = kubeApiServerItem["status"]["containerStatuses"]
            apiServerStatuses = list(map(lambda x: x["ready"], apiServerStatuses))

            # Ensure all of the instances of the API server are up.
            if len(apiServerStatuses) != 0 and all(apiServerStatuses):
                print("temp1: " + all(apiServerStatuses))
                print("Api server is up and running")
                return True
        except:
            print("Error:", sys.exc_info()[0])
            print("Api server is not up yet, Attempt #" + str(currentAttempt))

        currentAttempt += 1
    #if len(apiServerStatuses) == 0 or False in apiServerStatuses or "False" in apiServerStatuses:
    #    return false

    print("Reached the maximum number of attempts, api server did not come up, reverting the change")
    return False

print("Starting...")
with open(apiServerFilePath, "r+") as file:
    yamlResult = yaml.full_load(file)
    file.seek(0)
    oldYaml = copy.deepcopy(yamlResult)
    for container in yamlResult["spec"]["containers"]:
        commands = container["command"]
        volumeMounts = container["volumeMounts"]
        hostPaths = yamlResult["spec"]["volumes"]
        if container["name"] == "kube-apiserver":
            if VerifyAuditLogsAreNotEnabled(commands, volumeMounts, hostPaths):
                EnableAuditLogs(commands, volumeMounts, hostPaths)
                yaml.dump(yamlResult, file)
                file.truncate()
                if VerifyApiServerIsStillAlive() == False:
                    file.seek(0)
                    yaml.dump(oldYaml, file)
                    file.truncate()
            else:
                print("Detected potential collision, aborting the altering of the yaml file to avoid damaging the cluster")
            exit(0)

exit(0)