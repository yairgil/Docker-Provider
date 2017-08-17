#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <uuid/uuid.h>
#include <vector>

#include <scxcorelib/scxcmn.h>
#include <scxcorelib/scxprocess.h>
#include <scxcorelib/stringaid.h>
#include <testutils/scxunit.h>
#include <testutils/providertestutils.h>

#include "Container_Process_Class_Provider.h"
#include "cjson/cJSON.h"

using namespace std;
using namespace SCXCoreLib;

class ContainerProcessTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ContainerProcessTest);
    CPPUNIT_TEST(TestPsParsing);
    //Running this test at the end to test that the unset of DOCKER_TESTRUNNER_STRING is successful
    CPPUNIT_TEST(TestEnumerateInstances);
    CPPUNIT_TEST_SUITE_END();
private:

public:
    
protected:
    void TestEnumerateInstances()
    {
        wstring errMsg;
        TestableContext context;
        vector<wstring> m_keyNames;
        m_keyNames.push_back(L"InstanceID");

        // Enumerate provider. This just tests if the provider is able to pick up properties from a docker info call on the machine
        StandardTestEnumerateInstances<mi::Container_Process_Class_Provider>(m_keyNames, context, CALL_LOCATION(errMsg));
        CPPUNIT_ASSERT_EQUAL(1, context.Size());        
		
        // Only check that the values are present and within the valid range because it is not possible to create a controlled environment
        for (unsigned i = 0; i < context.Size(); ++i)
        {
            wstring instanceId = context[i].GetProperty(L"InstanceID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg));
            CPPUNIT_ASSERT(instanceId.length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"Computer", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"Pod", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"Namespace", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"Uid", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"PID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"PPID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"C", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"STIME", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"Tty", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"Cmd", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"Id", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"Name", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
        }
    }

    void TestPsParsing()
    {
        wstring errMsg;
        TestableContext context;
        vector<wstring> m_keyNames;
        m_keyNames.push_back(L"InstanceID");
        string k8NodeInfoString = "\r\n{\"ID\":\"4H4P:Z5O5:MMPA:BSDD:44TG:3FRY:AJ5Z:SPU4:LIEV:2XIF:SDJ3:FWLL\",\"Containers\":23,\"ContainersRunning\":13,\"ContainersPaused\":0,\"ContainersStopped\":10,\"Images\":25,\"Driver\":\"overlay\",\"DriverStatus\":[[\"Backing Filesystem\",\"extfs\"]],\"SystemStatus\":null,\"Plugins\":{\"Volume\":[\"local\"],\"Network\":[\"host\",\"bridge\",\"null\",\"overlay\"],\"Authorization\":null},\"MemoryLimit\":true,\"SwapLimit\":false,\"KernelMemory\":true,\"CpuCfsPeriod\":true,\"CpuCfsQuota\":true,\"CPUShares\":true,\"CPUSet\":true,\"IPv4Forwarding\":true,\"BridgeNfIptables\":true,\"BridgeNfIp6tables\":true,\"Debug\":false,\"NFd\":89,\"OomKillDisable\":true,\"NGoroutines\":89,\"SystemTime\":\"2017-04-25T22:06:56.371719716Z\",\"ExecutionDriver\":\"\",\"LoggingDriver\":\"json-file\",\"CgroupDriver\":\"cgroupfs\",\"NEventsListener\":0,\"KernelVersion\":\"4.4.0-72-generic\",\"OperatingSystem\":\"Ubuntu 16.04 LTS\",\"OSType\":\"linux\",\"Architecture\":\"x86_64\",\"IndexServerAddress\":\"https://index.docker.io/v1/\",\"RegistryConfig\":{\"InsecureRegistryCIDRs\":[\"127.0.0.0/8\"],\"IndexConfigs\":{\"docker.io\":{\"Name\":\"docker.io\",\"Mirrors\":null,\"Secure\":true,\"Official\":true}},\"Mirrors\":null},\"NCPU\":2,\"MemTotal\":7305641984,\"DockerRootDir\":\"/var/lib/docker\",\"HttpProxy\":\"\",\"HttpsProxy\":\"\",\"NoProxy\":\"\",\"Name\":\"k8-master-71E8D996-0\",\"Labels\":null,\"ExperimentalBuild\":false,\"ServerVersion\":\"1.12.6\",\"ClusterStore\":\"\",\"ClusterAdvertise\":\"\",\"SecurityOptions\":[\"apparmor\",\"seccomp\"],\"Runtimes\":{\"runc\":{\"path\":\"docker-runc\"}},\"DefaultRuntime\":\"runc\",\"Swarm\":{\"NodeID\":\"\",\"NodeAddr\":\"\",\"LocalNodeState\":\"inactive\",\"ControlAvailable\":false,\"Error\":\"\",\"RemoteManagers\":null,\"Nodes\":0,\"Managers\":0,\"Cluster\":{\"ID\":\"\",\"Version\":{},\"CreatedAt\":\"0001-01-01T00:00:00Z\",\"UpdatedAt\":\"0001-01-01T00:00:00Z\",\"Spec\":{\"Orchestration\":{},\"Raft\":{},\"Dispatcher\":{},\"CAConfig\":{},\"TaskDefaults\":{}}}},\"LiveRestoreEnabled\":false}";
        setenv(DOCKER_TESTRUNNER_STRING,k8NodeInfoString.c_str(),1);

        // Enumerate provider. Use k8 specific response instead of the standard docker info response
        StandardTestEnumerateInstances<mi::Container_Process_Class_Provider>(m_keyNames, context, CALL_LOCATION(errMsg));
        CPPUNIT_ASSERT(context.Size() > 0);

        for (unsigned i = 0; i < context.Size(); ++i)
        {
            wstring instanceId = context[i].GetProperty(L"InstanceID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg));
            CPPUNIT_ASSERT(instanceId.length());
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"k8-master-71E8D996-0"), context[i].GetProperty(L"Computer", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg))) ;
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"1.12.6"), context[i].GetProperty(L"DockerVersion", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"Ubuntu 16.04 LTS"), context[i].GetProperty(L"OperatingSystem", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"local"), context[i].GetProperty(L"Volume", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"Master"), context[i].GetProperty(L"NodeRole", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"Kubernetes"), context[i].GetProperty(L"OrchestratorType", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
        }

        unsetenv(DOCKER_TESTRUNNER_STRING);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(ContainerProcessTest);
