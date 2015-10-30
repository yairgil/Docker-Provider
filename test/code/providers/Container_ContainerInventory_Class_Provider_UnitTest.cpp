#include <limits.h>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <uuid/uuid.h>
#include <vector>
#include <wchar.h>

#include <scxcorelib/scxcmn.h>
#include <scxcorelib/scxprocess.h>
#include <scxcorelib/stringaid.h>
#include <testutils/scxunit.h>
#include <testutils/providertestutils.h>

#include "Container_ContainerInventory_Class_Provider.h"
#include "cjson/cJSON.h"
#include "TestScriptPath.h"

using namespace std;
using namespace SCXCoreLib;

class ContainerInventoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ContainerInventoryTest);
	CPPUNIT_TEST(TestEnumerateInstances);
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
	void TestEnumerateInstances()
	{
		wstring errMsg;
		TestableContext context;

		vector<wstring> m_keyNames;
		m_keyNames.push_back(L"InstanceID");

		// Run a container to ensure that there is at lease one result
		char containerName[65];
		strcpy(containerName, NewGuid().c_str());
		containers.push_back(string(containerName));
		char command[128];
		sprintf(command, "docker run --name=%s hello-world", containerName);

		CPPUNIT_ASSERT(!system(command));
		sleep(5);

		// Enumerate provider
		StandardTestEnumerateInstances<mi::Container_ContainerInventory_Class_Provider>(m_keyNames, context, CALL_LOCATION(errMsg));

		// Get images using command line
		CPPUNIT_ASSERT(!system("docker ps -aq --no-trunc > /tmp/docker_container_ids.txt"));

		FILE* idFile = fopen("/tmp/docker_container_ids.txt", "r");
		CPPUNIT_ASSERT(idFile);

		wchar_t id[13];
		set<wstring> allIds;

		// Full container IDs (one per line)
		while (fwscanf(idFile, L"%ls", id) != EOF)
		{
			allIds.insert(wstring(id));
		}

		fclose(idFile);
		remove("/tmp/docker_image_ids.txt");

		CPPUNIT_ASSERT_EQUAL(allIds.size(), context.Size());

		for (unsigned i = 0; i < context.Size(); ++i)
		{
			// Verify the InstanceID
			CPPUNIT_ASSERT(allIds.count(context[i].GetKey(L"InstanceID", CALL_LOCATION(errMsg))));
		}
	}
	
	void TestEnumerateVerifyAllValues()
	{
		wstring errMsg;
		TestableContext context;

		vector<wstring> m_keyNames;
		m_keyNames.push_back(L"InstanceID");

		// Run a container to ensure that there is at lease one result
		char containerName[65];
		strcpy(containerName, NewGuid().c_str());
		containers.push_back(string(containerName));
		char command[256];
		sprintf(command, "docker run --name=%s hello-world", containerName);

		CPPUNIT_ASSERT(!system(command));
		sleep(5);

		// Enumerate provider
		StandardTestEnumerateInstances<mi::Container_ContainerInventory_Class_Provider>(m_keyNames, context, CALL_LOCATION(errMsg));

		// Get container inventory using a script
		sprintf(command, "python %sContainerInventory.py > /tmp/docker_container_inventory.txt", TEST_SCRIPT_PATH);
		CPPUNIT_ASSERT(!system(command));

		FILE* containerFile = fopen("/tmp/docker_container_inventory.txt", "r");
		CPPUNIT_ASSERT(containerFile);

		char buffer[1024];
		vector<cJSON*> containersList;

		while (fgets(buffer, 1023, containerFile))
		{
			containersList.push_back(cJSON_Parse(buffer));
		}

		fclose(containerFile);
		remove("/tmp/docker_container_inventory.txt");

		// Should have same number of images
		CPPUNIT_ASSERT_EQUAL(containersList.size(), context.Size());

		wchar_t currentId[66];
		int containerCount = 0;

		for (unsigned i = 0; i < containersList.size(); ++i)
		{
			bool flag = false;
			mbstowcs(currentId, cJSON_GetObjectItem(containersList[i], "InstanceID")->valuestring, 65);

			for (unsigned j = 0; !flag && j < context.Size(); j++)
			{
				if (!context[j].GetProperty(L"InstanceID", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)).compare(wstring(currentId)))
				{
					wchar_t temp[512];
					unsigned count = 0;

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "CreatedTime")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"CreatedTime", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "ElementName")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"ElementName", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "State")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"State", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "StartedTime")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"StartedTime", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "FinishedTime")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"FinishedTime", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "ImageId")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"ImageId", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "Image")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"Image", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "Repository")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"Repository", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "ImageTag")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"ImageTag", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "ComposeGroup")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"ComposeGroup", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "ContainerHostname")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"ContainerHostname", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "Computer")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"Computer", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "Command")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"Command", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "EnvironmentVar")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"EnvironmentVar", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "Ports")->valuestring, 511);

					if (wcslen(temp) > 3)
					{
						CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"Ports", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));
					}

					mbstowcs(temp, cJSON_GetObjectItem(containersList[i], "Links")->valuestring, 511);
					CPPUNIT_ASSERT_EQUAL(wstring(temp), context[j].GetProperty(L"Links", CALL_LOCATION(errMsg)).GetValue_MIString(CALL_LOCATION(errMsg)));

					count = cJSON_GetObjectItem(containersList[i], "ExitCode")->valueint;
					unsigned providerExitCode = context[j].GetProperty(L"ExitCode", CALL_LOCATION(errMsg)).GetValue_MIUint32(CALL_LOCATION(errMsg));
					CPPUNIT_ASSERT_EQUAL(count, providerExitCode);
					CPPUNIT_ASSERT(providerExitCode <= INT_MAX);

					flag = true;
					containerCount += 1;
				}
			}

			cJSON_Delete(containersList[i]);
		}

		// Ensure all objects were validated
		CPPUNIT_ASSERT_EQUAL(containerCount, context.Size());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ContainerInventoryTest);