import logging
import sys
import yaml
import requests
import time
import copy
from dataclasses import dataclass
from shutil import copy2
from io import StringIO

@dataclass
class ApiServerComponents:
    commands: list
    hostPaths: list
    volumeMounts: list

class AuditLogsEnabler():
    auditPolicyFilePath = "/etc/kubernetes/audit-policy.yaml"
    backUpFilePath = "/var/log/kube-apiserver.yaml.backup"
    apiServerFilePath = "/etc/kubernetes/manifests/kube-apiserver.yaml"
    logger = ""

    volumeMountsToBeAdded = [{
        "mountPath": "/etc/kubernetes",
        "name": "etc-kubernetes",
        "readOnly": True
    }, {
        "mountPath": "/var/log/kube-apiserver",
        "name": "var-log-kubeapi"
    }]

    hostPathsToBeAdded = [{
        "name": "etc-kubernetes",
        "hostPath": {
            "path": "/etc/kubernetes",
            "type": "Directory",
        }
    }, {
        "name": "var-log-kubeapi",
        "hostPath": {
            "path": "/var/log/kube-apiserver",
            "type": "DirectoryOrCreate",
        }
    }]

    commandsToBeAdded = [
        "--audit-policy-file=" + auditPolicyFilePath,
        "--audit-log-path=/var/log/kube-apiserver/audit",
        "--audit-log-format=json",
        "--audit-log-maxage=1",
        "--audit-log-maxbackup=10",
        "--audit-log-maxsize=1"
    ]

    def __init__(self):
        logger = logging.getLogger()
        logger.setLevel(logging.DEBUG)

        formatter = logging.Formatter(
            '%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        handler = logging.StreamHandler(sys.stdout)
        self.InitHandler(logger, handler, formatter)

        try:
            handler = logging.FileHandler("/var/log/InitContainerLogs.log")
            InitHandler(logger, handler, formatter)
        except PermissionError:
            logger.warning(
                "failed to add file handler for logging due to insufficient permissions, only using stdout for now")

        logger.info("Finished Initializing")
        self.logger = logger

    def InitHandler(self, logger, handler, formatter):
        handler.setLevel(logging.DEBUG)
        handler.setFormatter(formatter)
        logger.addHandler(handler)

    def VerifyAuditLogsAreNotEnabled(self, commands, volumeMounts, hostPaths):
        self.logger.info("Checking to see whether there's any sign for audit logs configuration in the api server yaml")
        commandsJoined = ', '.join(commands)
        commandsTobeAdded = self.commandsToBeAdded
        commandsKeysToBeAdded = list(
            map(lambda x: x[:x.find('=')], commandsTobeAdded))

        for AuditLogsCommand in commandsKeysToBeAdded:
            if AuditLogsCommand in commandsJoined:
                self.logger.warning("Potential collision detected: command: " + AuditLogsCommand)
                return False

        for currentVolumeMount in volumeMounts:
            for volumeMountToBeAdded in self.volumeMountsToBeAdded:
                if currentVolumeMount["name"] == volumeMountToBeAdded["name"] or currentVolumeMount["mountPath"] == volumeMountToBeAdded["mountPath"]:
                    self.logger.warning("Potential collision detected: volumeMount: " +
                          volumeMountToBeAdded["name"])
                    return False

        for currentHostPath in hostPaths:
            for hostPathToBeAdded in self.hostPathsToBeAdded:
                if currentHostPath["name"] == hostPathToBeAdded["name"] or currentHostPath["hostPath"]["path"] == hostPathToBeAdded["hostPath"]["path"]:
                    self.logger.warning("Potential collision detected: hostPath: " +
                          hostPathToBeAdded["name"])
                    return False
        self.logger.info("no sign audit logs configuration sign was found in the api server yaml")
        return True

    def EnableAuditLogs(self, commands, volumeMounts, hostPaths):
        commands.extend(self.commandsToBeAdded)
        volumeMounts.extend(self.volumeMountsToBeAdded)
        hostPaths.extend(self.hostPathsToBeAdded)

    def VerifyApiServerIsStillAlive(self):
        self.logger.info("Verifying that the API server is alive")

        apiServerUrl = "https://kubernetes.default.svc.cluster.local/api/v1/namespaces/kube-system/pods"
        bearerTokenFilePath = "/var/run/secrets/kubernetes.io/serviceaccount/token"
        caCertVerificationPath = "/var/run/secrets/kubernetes.io/serviceaccount/ca.crt"
        # Get the bearer token of the attached service account
        bearerTokenFile = open(bearerTokenFilePath)
        bearerToken = bearerTokenFile.read()
        bearerTokenFile.close()

        header = {'Authorization': 'Bearer ' + bearerToken}

        currentAttempt = 0
        maxAttempts = 10
        while currentAttempt < maxAttempts:
            time.sleep(10)
            response = ""
            try:
                response = requests.get(
                    apiServerUrl, headers=header, verify=caCertVerificationPath)
                response = response.json()
                response = response["items"]
                # Get the API server item
                kubeApiServerItem = next(
                    item for item in response if item["metadata"]["name"].startswith("kube-apiserver"))
                apiServerStatuses = kubeApiServerItem["status"]["containerStatuses"]
                apiServerStatuses = list(
                    map(lambda x: x["ready"], apiServerStatuses))

                # Ensure all of the instances of the API server are up.
                if len(apiServerStatuses) != 0 and all(apiServerStatuses):
                    self.logger.info("Api server is up and running")
                    return True
            except:
                self.logger.warning(sys.exc_info()[0])
                self.logger.warning(
                    "Api server is not up yet, Attempt #" + str(currentAttempt))

            currentAttempt += 1

        self.logger.error("Reached the maximum number of attempts")
        return False

    def InitializeFileSystem(self):
        try:
            self.logger.info("backing up yaml into " + str(self.backUpFilePath) + "in case the api server becomes corrupted and auto fix does not resolve it after 10 minutes, please copy the file file back to " + str(self.apiServerFilePath))
            copy2(self.apiServerFilePath, self.backUpFilePath)
            self.logger.info("finished backing up the api server yaml")
        
            self.logger.info("copying the audit policy file to " + self.auditPolicyFilePath)
            copy2("audit-policy.yaml", self.auditPolicyFilePath)
        except:
            self.logger.error("Error encountered: ")
            self.logger.error(sys.exc_info()[0])
            return False
        return True
        
    def ExtractYamlSections(self, yamlResult):
        for container in yamlResult["spec"]["containers"]:
            if container["name"] == "kube-apiserver":
                commands = container["command"]
                volumeMounts = container["volumeMounts"]
                hostPaths = yamlResult["spec"]["volumes"]
                return ApiServerComponents(commands, volumeMounts, hostPaths)
        return None

    def Run(self, file):
        if self.VerifyApiServerIsStillAlive() == False:
            self.logger.error(
                "Could not verify the api server status, exiting without making any changes...")
            return 0
        
        if self.InitializeFileSystem() == False:
            self.logger.error("failed to initialize files dependencies, aborting without making any changes...")
            return 0

        file.seek(0)
        self.logger.info("reading the api server yaml file")
        yamlResult = yaml.full_load(file)
        file.seek(0)
        oldYaml = copy.deepcopy(yamlResult)
        x = self.ExtractYamlSections(yamlResult)
        for container in yamlResult["spec"]["containers"]:
            commands = container["command"]
            volumeMounts = container["volumeMounts"]
            hostPaths = yamlResult["spec"]["volumes"]
            if container["name"] == "kube-apiserver":
                if self.VerifyAuditLogsAreNotEnabled(commands, volumeMounts, hostPaths):
                    self.EnableAuditLogs(commands, volumeMounts, hostPaths)
                    yaml.dump(yamlResult, file)
                    if self.VerifyApiServerIsStillAlive() == False:
                        self.logger.info(
                            "Api server did not come up, reverting the changes")
                        file.seek(0)
                        yaml.dump(oldYaml, file)
                        self.logger.info("Done reverting the changes")
                else:
                    self.logger.info(
                        "Detected potential collision, aborting the altering of the yaml file to avoid damaging the cluster")
                return 0
        return 0


if __name__ == '__main__':
    print("Starting...")
    x = AuditLogsEnabler()
    with open(x.apiServerFilePath, "r+") as file:
        x.Run(file)
