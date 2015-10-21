#include <set>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unistd.h>
#include <uuid/uuid.h>
#include <vector>
#include <wchar.h>

#include <scxcorelib/scxcmn.h>
#include <scxcorelib/scxprocess.h>
#include <scxcorelib/stringaid.h>
#include <testutils/scxunit.h>
#include <testutils/providertestutils.h>

#include "Container_DaemonEvent_Class_Provider.h"
#include "cjson/cJSON.h"

#define LASTQUERYTIMEFILE "/var/opt/microsoft/docker-cimprov/state/LastEventQueryTime.txt"

using namespace std;
using namespace SCXCoreLib;

class DaemonEventTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(DaemonEventTest);
	CPPUNIT_TEST(TestEnumerateVerifyAllValues);
	CPPUNIT_TEST_SUITE_END();

private:
	vector<string> containers;

	static string NewGuid()
	{
		uuid_t uuid;
		uuid_generate_random(uuid);
		char s[37];
		uuid_unparse(uuid, s);
		return s;
	}

public:
	void setUp()
	{
		istringstream processInput;
		ostringstream processOutput;
		ostringstream processErr;

		// Get some images to use
		fputc('\n', stdout);
		SCXProcess::Run(L"docker pull hello-world", processInput, processOutput, processErr, 0);
	}

	void tearDown()
	{
		istringstream processInput;
		ostringstream processOutput;
		ostringstream processErr;

		wchar_t command[128];
		wchar_t temp[128];

		// Remove the containers that were started by the tests
		for (unsigned i = 0; i < containers.size(); i++)
		{
			mbstowcs(temp, containers[i].c_str(), 127);
			swprintf(command, 127, L"docker rm %s", temp);
			SCXProcess::Run(command, processInput, processOutput, processErr, 0);
		}

		containers.clear();
	}

protected:
	void TestEnumerateVerifyAllValues()
	{
		wstring errMsg;
		TestableContext context;

		istringstream processInput;
		ostringstream processOutput;
		ostringstream processErr;

		vector<wstring> m_keyNames;
		m_keyNames.push_back(L"InstanceID");

		// Read the time from disk
		FILE* file = fopen(LASTQUERYTIMEFILE, "r");
		int fileTime = time(NULL);

		if (file)
		{
			fscanf(file, "%d", &fileTime);
		}
		else
		{
			file = fopen(LASTQUERYTIMEFILE, "w");
			CPPUNIT_ASSERT(file);
			fprintf(file, "%d", fileTime);
		}

		fclose(file);

		// Run a container to ensure that there is at lease one result
		char containerName[65];
		strcpy(containerName, NewGuid().c_str());
		wchar_t wcontainerName[65];
		mbstowcs(wcontainerName, containerName, 64);
		containers.push_back(string(containerName));
		wchar_t command[128];
		swprintf(command, 127, L"docker run --name=%s hello-world", wcontainerName);

		SCXProcess::Run(command, processInput, processOutput, processErr, 0);
		sleep(5);

		// Enumerate provider
		StandardTestEnumerateInstances<mi::Container_DaemonEvent_Class_Provider>(m_keyNames, context, CALL_LOCATION(errMsg));

		wstring allowedCommandsList[] = { L"attach", L"commit", L"copy", L"create", L"destroy", L"die", L"exec_create", L"exec_start", L"export", L"kill", L"oom", L"pause", L"rename", L"resize", L"restart", L"start", L"stop", L"top", L"unpause", L"delete", L"import", L"pull", L"push", L"tag", L"untag" };
		set<wstring> allowedCommands(allowedCommandsList, allowedCommandsList + 25);

		// Check validity of every field of every object
		for (unsigned i = 0; i < context.Size(); i++)
		{
			// This field is a GUID and the value does not need verification
			CPPUNIT_ASSERT(context[i].GetProperty(L"InstanceID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());

			// These fields cannot be validated directly against Docker because events cannot be uniquely identified
			wstring temp = context[i].GetProperty(L"TimeOfCommand", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg));
			int t = -1;
			swscanf(temp.c_str(), L"%d", &t);
			CPPUNIT_ASSERT(t >= fileTime);

			CPPUNIT_ASSERT(allowedCommands.count(context[i].GetProperty(L"Command", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg))));
			CPPUNIT_ASSERT(context[i].GetProperty(L"ElementName", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).length());
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DaemonEventTest);