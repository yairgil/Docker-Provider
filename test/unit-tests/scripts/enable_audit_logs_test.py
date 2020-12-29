import sys
sys.path.insert(0, 'kubernetes/InitContainerLinux')
from EnableAuditLogs import *
import unittest
import json
from mock import mock_open, patch, call

import tempfile

defaultApiServerFilePath = "test/unit-tests/scripts/default-kube-apiserver.yaml"

class EnableAuditLogsTests(unittest.TestCase):
    auditEnabler = AuditLogsEnabler()
    yamlContent = ""

    def setUp(self):
        
        with open(defaultApiServerFilePath, "r+") as file:
            self.yamlContent = yaml.full_load(file)

    def test_enable_audit_logs(self):
        yamlSpec = self.yamlContent["spec"]
        commands = yamlSpec["containers"][0]["command"]
        volumeMounts = yamlSpec["containers"][0]["volumeMounts"]
        hostPaths = yamlSpec["volumes"]

        for auditLogCommand in self.auditEnabler.commandsToBeAdded:
            commandsExtended = copy.deepcopy(commands)
            commandsExtended.insert(len(commandsExtended), auditLogCommand)
            
            self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commandsExtended, volumeMounts, hostPaths)))


class VerifyAuditLogsAreNotEnabledTests(unittest.TestCase):

    auditEnabler = AuditLogsEnabler()
    yamlContent = ""

    def setUp(self):
        with open(defaultApiServerFilePath, "r+") as file:
            self.yamlContent = yaml.full_load(file)

    def test_audit_logs_enabled_commands_exist(self):
        yamlSpec = self.yamlContent["spec"]
        commands = yamlSpec["containers"][0]["command"]
        volumeMounts = yamlSpec["containers"][0]["volumeMounts"]
        hostPaths = yamlSpec["volumes"]

        for auditLogCommand in self.auditEnabler.commandsToBeAdded:
            commandsExtended = copy.deepcopy(commands)
            commandsExtended.insert(len(commandsExtended), auditLogCommand)
            self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commandsExtended, volumeMounts, hostPaths)))

    def test_audit_logs_enabled_host_paths_exist(self):
        yamlSpec = self.yamlContent["spec"]
        commands = yamlSpec["containers"][0]["command"]
        volumeMounts = yamlSpec["containers"][0]["volumeMounts"]
        hostPaths = yamlSpec["volumes"]

        # add 1 host path element twice
        hostPathsToBeAdded = copy.deepcopy(
            self.auditEnabler.hostPathsToBeAdded)
        hostPathsExtended = copy.deepcopy(hostPaths)
        hostPathsExtended.insert(len(hostPathsExtended), hostPathsToBeAdded[0])
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commands, volumeMounts, hostPathsExtended)))

        # add 1 host path element with the same name but different path
        hostPathsToBeAdded = copy.deepcopy(
            self.auditEnabler.hostPathsToBeAdded)
        hostPathsToBeAdded[0]["name"] = "ca-certs"
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commands, volumeMounts, hostPathsExtended)))

        # add 1 host path element with the same path but different name
        hostPathsToBeAdded = copy.deepcopy(
            self.auditEnabler.hostPathsToBeAdded)
        hostPathsToBeAdded[0]["hostPath"]["path"] = "/etc/ssl/certs"
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commands, volumeMounts, hostPathsExtended)))

    def test_audit_logs_enabled_volume_mounts_exist(self):
        yamlSpec = self.yamlContent["spec"]
        commands = yamlSpec["containers"][0]["command"]
        volumeMounts = yamlSpec["containers"][0]["volumeMounts"]
        hostPaths = yamlSpec["volumes"]

        # add 1 volume mount element twice
        volumeMountsToBeAdded = copy.deepcopy(
            self.auditEnabler.volumeMountsToBeAdded)
        volumeMountsExtended = copy.deepcopy(volumeMounts)
        volumeMountsExtended.insert(
            len(volumeMountsExtended), volumeMountsToBeAdded[0])
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commands, volumeMountsExtended, hostPaths)))

        # add 1 volume mount element with the same name but different path
        volumeMountsToBeAdded = copy.deepcopy(
            self.auditEnabler.volumeMountsToBeAdded)
        volumeMountsToBeAdded[0]["name"] = "ca-certs"
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commands, volumeMountsExtended, hostPaths)))

        # add 1 volume mount element with the same path but different name
        volumeMountsToBeAdded = copy.deepcopy(
            self.auditEnabler.volumeMountsToBeAdded)
        volumeMountsToBeAdded[0]["mountPath"] = "/etc/ssl/certs"
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commands, volumeMountsExtended, hostPaths)))

    def test_audit_logs_audit_logs_not_enabled(self):
        yamlSpec = self.yamlContent["spec"]
        commands = yamlSpec["containers"][0]["command"]
        volumeMounts = yamlSpec["containers"][0]["volumeMounts"]
        hostPaths = yamlSpec["volumes"]

        self.assertTrue(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commands, volumeMounts, hostPaths)))

    def test_audit_logs_audit_logs_not_enabled_multiple_containers(self):
        yamlSpec = self.yamlContent["spec"]
        containers = yamlSpec["containers"]
        secondContainer = copy.deepcopy(containers[0])
        secondContainer["name"] = "randomcontainername"
        containers.insert(len(containers), secondContainer)

        commands = yamlSpec["containers"][0]["command"]
        volumeMounts = yamlSpec["containers"][0]["volumeMounts"]
        hostPaths = yamlSpec["volumes"]

        self.assertTrue(self.auditEnabler.VerifyAuditLogsAreNotEnabled(ApiServerComponents(commands, volumeMounts, hostPaths)))

class VerifyApiServerIsStillAliveTests(unittest.TestCase):
    auditEnabler = AuditLogsEnabler()
    successOnIterationNumber = -1
    apiServerGetPodsResponse = "test/unit-tests/scripts/apiServerGetPodsResponse.json"
    apiServerGetPodsResponseContent = ""
    def setUp(self):
        with open(self.apiServerGetPodsResponse, "r+") as file:
            self.apiServerGetPodsResponseContent = json.load(file)

    # This method will be used by the mock to replace requests.get
    def mocked_requests_get(*args, **kwargs):
        class MockResponse:
            def __init__(self, json_data, status_code):
                self.json_data = json_data
                self.status_code = status_code

            def json(self):
                return self.json_data
        
        self = args[0]
        if self.successOnIterationNumber > 0:
            self.successOnIterationNumber = self.successOnIterationNumber - 1
            return MockResponse(None, 404)
        
        if self.successOnIterationNumber == 0:
            return MockResponse(self.apiServerGetPodsResponseContent, 200)
        
        return MockResponse(None, 404)

    def test_verify_api_server_valid_connection(self):
        with patch('builtins.open', mock_open(read_data="a")) as m, patch('requests.get', side_effect=self.mocked_requests_get):
            self.successOnIterationNumber = 0
            self.assertTrue(self.auditEnabler.VerifyApiServerIsStillAlive(sleepInterval=0.1))

    def test_verify_api_server_no_connection(self):
        with patch('builtins.open', mock_open(read_data="a")) as m, patch('requests.get', side_effect=self.mocked_requests_get):
            self.successOnIterationNumber = 500
            self.assertFalse(self.auditEnabler.VerifyApiServerIsStillAlive(sleepInterval=0.1))

    def test_verify_api_server_connection_starts_after_few_iterations(self):
        with patch('builtins.open', mock_open(read_data="a")) as m, patch('requests.get', side_effect=self.mocked_requests_get):
            self.successOnIterationNumber = 3
            self.assertTrue(self.auditEnabler.VerifyApiServerIsStillAlive(sleepInterval=0.1))

    def test_verify_api_server_not_all_master_nodes_are_up(self):
        response = self.apiServerGetPodsResponseContent["items"]
        # Get the API server item
        kubeApiServerItem = next(
            item for item in response if item["metadata"]["name"].startswith("kube-apiserver"))
        apiServerStatuses = kubeApiServerItem["status"]["containerStatuses"]
        firstNodeStatus = apiServerStatuses[0]
        secondNodeStatus = copy.deepcopy(firstNodeStatus)
        thirdNodeStatus = copy.deepcopy(firstNodeStatus)
        secondNodeStatus["ready"] = False
        apiServerStatuses.append(secondNodeStatus)
        apiServerStatuses.append(thirdNodeStatus)
        with patch('builtins.open', mock_open(read_data="a")) as m, patch('requests.get', side_effect=self.mocked_requests_get):
            self.successOnIterationNumber = 0
            self.assertFalse(self.auditEnabler.VerifyApiServerIsStillAlive(sleepInterval=0.1))

    def test_verify_api_server_invalid_response(self):
        response = self.apiServerGetPodsResponseContent["items"]
        # Get the API server item
        kubeApiServerItem = next(
            item for item in response if item["metadata"]["name"].startswith("kube-apiserver"))
        apiServerStatuses = kubeApiServerItem["status"]["containerStatuses"]
        apiServerStatuses.clear()
        with patch('builtins.open', mock_open(read_data="a")) as m, patch('requests.get', side_effect=self.mocked_requests_get):
            self.successOnIterationNumber = 0
            self.assertFalse(self.auditEnabler.VerifyApiServerIsStillAlive(sleepInterval=0.1))

@dataclass
class ApiServerComponentsLengths:
    commands: int
    volumeMounts: int
    hostPaths: int

class GeneralFlowTests(unittest.TestCase):

    auditEnabler = AuditLogsEnabler()
    yamlContent = ""

    def setUp(self):
        with open(defaultApiServerFilePath, "r+") as file:
            self.yamlContent = yaml.full_load(file)

    def get_api_Server_components_lengths(self, apiServerYaml):
        commandsLength = len(apiServerYaml["spec"]["containers"][0]["command"])
        volumeMountsLength = len(apiServerYaml["spec"]["containers"][0]["volumeMounts"])
        hostPathsLength = len(apiServerYaml["spec"]["volumes"])

        return ApiServerComponentsLengths(commandsLength, volumeMountsLength, hostPathsLength)

    def check_if_all_audit_logs_fields_exist(self, updatedYamlContent):
        updatedComponentsLengths = self.get_api_Server_components_lengths(updatedYamlContent)
        previousComponentsLengths = self.get_api_Server_components_lengths(self.yamlContent)
        self.assertEqual(updatedComponentsLengths.commands, previousComponentsLengths.commands + 6)
        self.assertIn('--audit-log-path=/var/log/kube-apiserver/audit',
                    updatedYamlContent["spec"]["containers"][0]["command"])
        self.assertEqual(updatedComponentsLengths.volumeMounts, previousComponentsLengths.volumeMounts + 2)
        self.assertEqual(updatedComponentsLengths.hostPaths, previousComponentsLengths.hostPaths + 2)


    def test_run_default_scenario(self):
        yamlContentStr = yaml.dump(self.yamlContent)
        # with patch('builtins.open', mock_open(read_data=self.document)) as m:
        with patch("EnableAuditLogs.AuditLogsEnabler.VerifyApiServerIsStillAlive") as apiServerMock, patch("EnableAuditLogs.AuditLogsEnabler.InitializeFileSystem") as initFSMock, tempfile.TemporaryFile(mode='w+') as file:
            apiServerMock.return_value = True
            initFSMock.return_value = True
            file.write(yamlContentStr)
            file.seek(0)

            self.auditEnabler.Run(file)

            file.seek(0)
            updatedYamlContent = yaml.full_load(file)
            self.check_if_all_audit_logs_fields_exist(updatedYamlContent)

    def test_run_enabling_audit_logs_api_server_fails(self):
        yamlContentStr = yaml.dump(self.yamlContent)
        with patch("EnableAuditLogs.AuditLogsEnabler.VerifyApiServerIsStillAlive") as apiServerMock, patch("EnableAuditLogs.AuditLogsEnabler.InitializeFileSystem") as initFSMock, tempfile.TemporaryFile(mode='w+') as file:
            apiServerMock.return_value = False
            initFSMock.return_value = True
            file.write(yamlContentStr)
            file.seek(0)

            self.auditEnabler.Run(file)

            # Ensure changes were reverted and the old yaml equals to the current one
            file.seek(0)
            updatedYamlContentStr = file.read()
            self.assertEqual(updatedYamlContentStr, yamlContentStr)

    def test_run_enabling_audit_logs_fsinit_fails(self):
        yamlContentStr = yaml.dump(self.yamlContent)
        with patch("EnableAuditLogs.AuditLogsEnabler.VerifyApiServerIsStillAlive") as apiServerMock, patch("EnableAuditLogs.AuditLogsEnabler.InitializeFileSystem") as initFSMock, tempfile.TemporaryFile(mode='w+') as file:
            apiServerMock.return_value = True
            initFSMock.return_value = False
            file.write(yamlContentStr)
            file.seek(0)

            self.auditEnabler.Run(file)

            # Ensure changes were reverted and the old yaml equals to the current one
            file.seek(0)
            updatedYamlContentStr = file.read()
            self.assertEqual(updatedYamlContentStr, yamlContentStr)

    def test_run_enabling_audit_logs_twice(self):
        yamlContentStr = yaml.dump(self.yamlContent)
        with patch("EnableAuditLogs.AuditLogsEnabler.VerifyApiServerIsStillAlive") as apiServerMock, patch("EnableAuditLogs.AuditLogsEnabler.InitializeFileSystem") as initFSMock, tempfile.TemporaryFile(mode='w+') as file:
            apiServerMock.return_value = True
            initFSMock.return_value = True
            file.write(yamlContentStr)

            # ensure audit logs configurations were added
            self.auditEnabler.Run(file)
            file.seek(0)
            updatedYamlContent = yaml.full_load(file)
            self.check_if_all_audit_logs_fields_exist(updatedYamlContent)

            # ensure no additional data was added
            file.seek(0)
            self.auditEnabler.Run(file)
            file.seek(0)
            updatedYamlContent = yaml.full_load(file)
            self.check_if_all_audit_logs_fields_exist(updatedYamlContent)

def suite():
    suite = unittest.TestSuite()
    suite.addTest(VerifyAuditLogsAreNotEnabledTests(
        'test_audit_logs_enabled_commands_exist'))
    suite.addTest(VerifyAuditLogsAreNotEnabledTests(
        'test_audit_logs_enabled_volume_mounts_exist'))
    suite.addTest(VerifyAuditLogsAreNotEnabledTests(
        'test_audit_logs_enabled_host_paths_exist'))
    suite.addTest(VerifyAuditLogsAreNotEnabledTests(
        'test_audit_logs_audit_logs_not_enabled_multiple_containers'))
    suite.addTest(VerifyAuditLogsAreNotEnabledTests(
        'test_audit_logs_audit_logs_not_enabled'))
    suite.addTest(VerifyApiServerIsStillAliveTests('test_verify_api_server_valid_connection'))
    suite.addTest(VerifyApiServerIsStillAliveTests('test_verify_api_server_no_connection'))
    suite.addTest(VerifyApiServerIsStillAliveTests('test_verify_api_server_connection_starts_after_few_iterations'))
    suite.addTest(VerifyApiServerIsStillAliveTests('test_verify_api_server_not_all_master_nodes_are_up'))
    suite.addTest(VerifyApiServerIsStillAliveTests('test_verify_api_server_invalid_response'))
    suite.addTest(GeneralFlowTests('test_run_default_scenario'))
    suite.addTest(GeneralFlowTests('test_run_enabling_audit_logs_api_server_fails'))
    suite.addTest(GeneralFlowTests('test_run_enabling_audit_logs_fsinit_fails'))
    suite.addTest(GeneralFlowTests('test_run_enabling_audit_logs_twice'))
    return suite


if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    runner.run(suite())
