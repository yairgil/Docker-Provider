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
    CPPUNIT_TEST(Testk8EnumerateInstances);
    CPPUNIT_TEST(TestNonk8EnumerateInstances);
    CPPUNIT_TEST_SUITE_END();

private:
    char hostname[128];
    wstring processCmdArr = [L"/bin/sh -c sleep inf;", L"sleep inf"];

public:
    void setUp()
    {        
        gethostname(hostname, sizeof hostname);        
    }

protected:
    void Testk8EnumerateInstances()
    {
        wstring errMsg;
        TestableContext context;
        vector<wstring> m_keyNames;
        m_keyNames.push_back(L"InstanceID");

        RunCommand("docker run -d --name=\"k8_cpt.sandboxname_cptpodname_cptnamepsace_cptid\" ubuntu /bin/sh -c \"sleep inf;\"");

        // Enumerate provider. This just tests if the provider is able to pick up properties from a docker info call on the machine
        StandardTestEnumerateInstances<mi::Container_Process_Class_Provider>(m_keyNames, context, CALL_LOCATION(errMsg));
        CPPUNIT_ASSERT_EQUAL(2, context.Size());        
		
        // Only check that the values are present and within the valid range because it is not possible to create a controlled environment
        for (unsigned i = 0; i < context.Size(); ++i)
        {
            wstring instanceId = context[i].GetProperty(L"InstanceID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg));
            CPPUNIT_ASSERT(instanceId.length());
            CPPUNIT_ASSERT_EQUAL(std::wstring(hostname), context[i].GetProperty(L"Computer", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"cptpodname"), context[i].GetProperty(L"Pod", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"cptnamespace"), context[i].GetProperty(L"Namespace", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"root"), context[i].GetProperty(L"Uid", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT(context[i].GetProperty(L"PID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"PPID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"C", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"STIME", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"?"),context[i].GetProperty(L"Tty", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(processCmdArr[i], context[i].GetProperty(L"Cmd", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT(context[i].GetProperty(L"Id", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"k8_cpt.sandboxname_cptpodname_cptnamepsace_cptid"), context[i].GetProperty(L"Name", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
        }
        RunCommand("docker rm -f $(docker ps --filter \"name=k8_cpt.sandboxname_cptpodname_cptnamepsace_cptid\" -a -q)");
    }

    void TestNonk8EnumerateInstances()
    {
        wstring errMsg;
        TestableContext context;
        vector<wstring> m_keyNames;
        m_keyNames.push_back(L"InstanceID");

        RunCommand("docker run -d --name=\"ContainerProcessTest\" ubuntu /bin/sh -c \"sleep inf;\"");

        // Enumerate provider. This just tests if the provider is able to pick up properties from a docker info call on the machine
        StandardTestEnumerateInstances<mi::Container_Process_Class_Provider>(m_keyNames, context, CALL_LOCATION(errMsg));
        CPPUNIT_ASSERT_EQUAL(2, context.Size());        
		
        // Only check that the values are present and within the valid range because it is not possible to create a controlled environment
        for (unsigned i = 0; i < context.Size(); ++i)
        {
            wstring instanceId = context[i].GetProperty(L"InstanceID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg));
            CPPUNIT_ASSERT(instanceId.length());
            CPPUNIT_ASSERT_EQUAL(std::wstring(hostname), context[i].GetProperty(L"Computer", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"None"), context[i].GetProperty(L"Cmd", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"None"), context[i].GetProperty(L"Cmd", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"root"),context[i].GetProperty(L"Uid", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT(context[i].GetProperty(L"PID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"PPID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"C", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT(context[i].GetProperty(L"STIME", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"?"),context[i].GetProperty(L"Tty", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT_EQUAL(processCmdArr[i], context[i].GetProperty(L"Cmd", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
            CPPUNIT_ASSERT(context[i].GetProperty(L"Id", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
            CPPUNIT_ASSERT_EQUAL(std::wstring(L"ContainerProcessTest"),context[i].GetProperty(L"Name", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
        }
        RunCommand("docker rm -f $(docker ps --filter \"name=ContainerProcessTest\" -a -q)");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(ContainerProcessTest);
