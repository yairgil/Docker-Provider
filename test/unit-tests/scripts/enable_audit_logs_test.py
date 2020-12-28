import sys
sys.path.insert(0, 'kubernetes/InitContainerLinux')
from EnableAuditLogs import *
import unittest
#from unittest.mock import Mock
from mock import mock_open, patch, call

#from io import StringIO
import tempfile
#
#import EnableAuditLogs.py


class EnableAuditLogsTests(unittest.TestCase):
    auditEnabler = AuditLogsEnabler()
    yamlContent = ""

    def setUp(self):
        defaultApiServerFilePath = "test/unit-tests/scripts/default-kube-apiserver.yaml"
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
            self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
                commandsExtended, volumeMounts, hostPaths))


class VerifyAuditLogsAreNotEnabledTests(unittest.TestCase):

    auditEnabler = AuditLogsEnabler()
    yamlContent = ""

    def setUp(self):
        defaultApiServerFilePath = "test/unit-tests/scripts/default-kube-apiserver.yaml"
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
            self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
                commandsExtended, volumeMounts, hostPaths))

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
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
            commands, volumeMounts, hostPathsExtended))

        # add 1 host path element with the same name but different path
        hostPathsToBeAdded = copy.deepcopy(
            self.auditEnabler.hostPathsToBeAdded)
        hostPathsToBeAdded[0]["name"] = "ca-certs"
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
            commands, volumeMounts, hostPathsExtended))

        # add 1 host path element with the same path but different name
        hostPathsToBeAdded = copy.deepcopy(
            self.auditEnabler.hostPathsToBeAdded)
        hostPathsToBeAdded[0]["hostPath"]["path"] = "/etc/ssl/certs"
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
            commands, volumeMounts, hostPathsExtended))

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
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
            commands, volumeMountsExtended, hostPaths))

        # add 1 volume mount element with the same name but different path
        volumeMountsToBeAdded = copy.deepcopy(
            self.auditEnabler.volumeMountsToBeAdded)
        volumeMountsToBeAdded[0]["name"] = "ca-certs"
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
            commands, volumeMountsExtended, hostPaths))

        # add 1 volume mount element with the same path but different name
        volumeMountsToBeAdded = copy.deepcopy(
            self.auditEnabler.volumeMountsToBeAdded)
        volumeMountsToBeAdded[0]["mountPath"] = "/etc/ssl/certs"
        self.assertFalse(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
            commands, volumeMountsExtended, hostPaths))

    def test_audit_logs_audit_logs_not_enabled(self):
        yamlSpec = self.yamlContent["spec"]
        commands = yamlSpec["containers"][0]["command"]
        volumeMounts = yamlSpec["containers"][0]["volumeMounts"]
        hostPaths = yamlSpec["volumes"]

        self.assertTrue(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
            commands, volumeMounts, hostPaths))

    def test_audit_logs_audit_logs_not_enabled_multiple_containers(self):
        yamlSpec = self.yamlContent["spec"]
        containers = yamlSpec["containers"]
        secondContainer = copy.deepcopy(containers[0])
        secondContainer["name"] = "randomcontainername"
        containers.insert(len(containers), secondContainer)

        commands = yamlSpec["containers"][0]["command"]
        volumeMounts = yamlSpec["containers"][0]["volumeMounts"]
        hostPaths = yamlSpec["volumes"]

        self.assertTrue(self.auditEnabler.VerifyAuditLogsAreNotEnabled(
            commands, volumeMounts, hostPaths))


class GeneralFlowTests(unittest.TestCase):

    auditEnabler = AuditLogsEnabler()
    yamlContent = ""

    def setUp(self):
        defaultApiServerFilePath = "test/unit-tests/scripts/default-kube-apiserver.yaml"
        with open(defaultApiServerFilePath, "r+") as file:
            self.yamlContent = yaml.full_load(file)

    def test_run_default_scenario(self):
        yamlContentStr = yaml.dump(self.yamlContent)
        # with patch('builtins.open', mock_open(read_data=self.document)) as m:
        with patch("EnableAuditLogs.AuditLogsEnabler.VerifyApiServerIsStillAlive") as apiServerMock, patch("EnableAuditLogs.AuditLogsEnabler.InitializeFileSystem") as initFSMock, tempfile.TemporaryFile(mode='w+') as file:
            apiServerMock.return_value = True
            initFSMock.return_value = True
            file.write(yamlContentStr)  # yaml.dump(yamlContent))
            file.seek(0)

            self.auditEnabler.Run(file)

            file.seek(0)
            yamlContent = yaml.full_load(file)
            self.assertEqual(len(yamlContent["spec"]["containers"][0]["command"]), len(
                self.yamlContent["spec"]["containers"][0]["command"]) + 6)
            self.assertIn('--audit-log-path=/var/log/kube-apiserver/audit',
                        yamlContent["spec"]["containers"][0]["command"])
            self.assertEqual(len(yamlContent["spec"]["containers"][0]["volumeMounts"]), len(
                self.yamlContent["spec"]["containers"][0]["volumeMounts"]) + 2)
            self.assertEqual(len(yamlContent["spec"]["volumes"]), len(
                self.yamlContent["spec"]["volumes"]) + 2)

    def test_run_doube_enabling_audit_logs(self):
        yamlContentStr = yaml.dump(self.yamlContent)
        with patch("EnableAuditLogs.AuditLogsEnabler.VerifyApiServerIsStillAlive") as apiServerMock, patch("EnableAuditLogs.AuditLogsEnabler.InitializeFileSystem") as initFSMock, tempfile.TemporaryFile(mode='w+') as file:
            apiServerMock.return_value = True
            initFSMock.return_value = True
            file.write(yamlContentStr)

            self.auditEnabler.Run(file)
            file.seek(0)
            yamlContent = yaml.full_load(file)
            self.assertEqual(len(yamlContent["spec"]["containers"][0]["command"]), len(
                self.yamlContent["spec"]["containers"][0]["command"]) + 6)
            self.assertIn('--audit-log-path=/var/log/kube-apiserver/audit',
                            yamlContent["spec"]["containers"][0]["command"])
            self.assertEqual(len(yamlContent["spec"]["containers"][0]["volumeMounts"]), len(
                self.yamlContent["spec"]["containers"][0]["volumeMounts"]) + 2)
            self.assertEqual(len(yamlContent["spec"]["volumes"]), len(
                self.yamlContent["spec"]["volumes"]) + 2)

            file.seek(0)
            self.auditEnabler.Run(file)
            file.seek(0)
            yamlContent = yaml.full_load(file)
            self.assertEqual(len(yamlContent["spec"]["containers"][0]["command"]), len(
                self.yamlContent["spec"]["containers"][0]["command"]) + 6)
            self.assertIn('--audit-log-path=/var/log/kube-apiserver/audit',
                            yamlContent["spec"]["containers"][0]["command"])
            self.assertEqual(len(yamlContent["spec"]["containers"][0]["volumeMounts"]), len(
                self.yamlContent["spec"]["containers"][0]["volumeMounts"]) + 2)
            self.assertEqual(len(yamlContent["spec"]["volumes"]), len(
                self.yamlContent["spec"]["volumes"]) + 2)

    # def tearDown(self):
        # self.widget.dispose()


def getAllWriteCalls(mockCalls):
    ''.join(map(lambda x: x.args[0], handle.write.mock_calls))


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
    suite.addTest(GeneralFlowTests('test_run_default_scenario'))
    suite.addTest(GeneralFlowTests('test_run_doube_enabling_audit_logs'))
    # suite.addTest(WidgetTestCase('test_widget_resize'))
    return suite


if __name__ == '__main__':
    runner = unittest.TextTestRunner()
    runner.run(suite())
