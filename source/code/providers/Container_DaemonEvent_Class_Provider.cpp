/* @migen@ */
#include <MI.h>
#include "Container_DaemonEvent_Class_Provider.h"

#include <stdio.h>
#include <string>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <uuid/uuid.h>
#include <vector>

#include "../cjson/cJSON.h"
#include "../dockerapi/DockerRemoteApi.h"

#define LASTQUERYTIMEFILE "/var/opt/microsoft/docker-cimprov/state/LastEventQueryTime.txt"

using namespace std;

class Guid
{
public:
	///
	/// Create a guid and represent it as string
	///
	static string NewToString()
	{
		uuid_t uuid;
		uuid_generate_random(uuid);
		char s[37];
		uuid_unparse(uuid, s);
		return string(s);
	}
};

MI_BEGIN_NAMESPACE

class EventQuery
{
private:
	///
	/// Create the REST request to list events
	///
	/// \returns Request in string format
	///
	static string restDockerEvents(int start, int end)
	{
		char result[70];
		sprintf(result, "GET /events?since=%d&until=%d HTTP/1.1\r\n\r\n", start, end);
		return string(result);
	}

	///
	/// Get the previous time if it was stored, otherwise get the current time
	///
	/// \returns Time of previous query if available
	///
	static int GetPreviousTime()
	{
		int fileTime = time(NULL);
		int currentTime = fileTime;
		FILE* file = fopen(LASTQUERYTIMEFILE, "r");

		if (file)
		{
			fscanf(file, "%d", &fileTime);
			fclose(file);

			if (fileTime > currentTime)
			{
				syslog(LOG_WARNING, "The time stored in %s is more recent than the current time", LASTQUERYTIMEFILE);
			}
		}
		else
		{
			syslog(LOG_WARNING, "Attempt in GetPreviousTime to open %s for reading failed", LASTQUERYTIMEFILE);
		}

		// Discard stored times that are more recent than the current time
		return fileTime > currentTime ? currentTime : fileTime;
	}

	///
	/// Write the previous time to disk
	///
	/// \param[in] t The time
	///
	static void SetPreviousTime(int t)
	{
		FILE* file = fopen(LASTQUERYTIMEFILE, "w");

		if (file)
		{
			fprintf(file, "%d", t);
			fclose(file);
		}
		else
		{
			syslog(LOG_WARNING, "Attempt in SetPreviousTime to open %s for writing failed", LASTQUERYTIMEFILE);
		}
	}

public:
	///
	/// Get information about Docker events since the last query
	///
	/// \returns Vector containing objects representing each event
	///
	static vector<Container_DaemonEvent_Class> QueryAll()
	{
		openlog("Container_DaemonEvent", LOG_PID | LOG_NDELAY, LOG_LOCAL1);

		vector<Container_DaemonEvent_Class> result;
		int previousTime = GetPreviousTime();
		int currentTime = time(NULL);

		if (currentTime > previousTime)
		{
			// Get computer name
			char name[256];
			string hostname = gethostname(name, 256) ? "" : string(name);

			// Request events
			vector<string> request(1, restDockerEvents(previousTime, currentTime));
			vector<cJSON*> response = getResponse(request, true);

			// See https://docs.docker.com/reference/api/Container_remote_api_v1.21/#monitor-docker-s-events for example output
			if (!response.empty() && response[0])
			{
				for (int i = 0; i < cJSON_GetArraySize(response[0]); i++)
				{
					cJSON* entry = cJSON_GetArrayItem(response[0], i);

					if (entry)
					{
						// New inventory entry
						Container_DaemonEvent_Class instance;
						instance.Computer_value(hostname.c_str());
						instance.InstanceID_value(Guid::NewToString().c_str());
						instance.Command_value(cJSON_GetObjectItem(entry, "status")->valuestring);

						char buffer[33];
						sprintf(buffer, "%d", cJSON_GetObjectItem(entry, "time")->valueint);
						instance.TimeOfCommand_value(buffer);

						char* tempImageName = cJSON_GetObjectItem(entry, "from")->valuestring;

						if (tempImageName)
						{
							// Container event
							instance.ElementName_value(tempImageName);
							instance.Id_value(cJSON_GetObjectItem(entry, "id")->valuestring);
						}
						else
						{
							// Image event
							instance.ElementName_value(cJSON_GetObjectItem(entry, "id")->valuestring);
							instance.Id_value("");
						}

						result.push_back(instance);
					}
					else
					{
						syslog(LOG_WARNING, "Attempt in QueryAll to get element %d of event list returned null", i);
					}
				}

				// Clean up object
				cJSON_Delete(response[0]);
			}
		}

		SetPreviousTime(currentTime);
		closelog();
		return result;
	}
};

#ifdef _MSC_VER
#pragma region
#endif

Container_DaemonEvent_Class_Provider::Container_DaemonEvent_Class_Provider(Module* module) : m_Module(module){}

Container_DaemonEvent_Class_Provider::~Container_DaemonEvent_Class_Provider(){}

void Container_DaemonEvent_Class_Provider::Load(Context& context)
{
	context.Post(MI_RESULT_OK);
}

void Container_DaemonEvent_Class_Provider::Unload(Context& context)
{
	context.Post(MI_RESULT_OK);
}

void Container_DaemonEvent_Class_Provider::EnumerateInstances(Context& context, const String& nameSpace, const PropertySet& propertySet, bool keysOnly, const MI_Filter* filter)
{
	vector<Container_DaemonEvent_Class> queryResult = EventQuery::QueryAll();

	for (unsigned i = 0; i < queryResult.size(); i++)
	{
		context.Post(queryResult[i]);
	}

	context.Post(MI_RESULT_OK);
}

void Container_DaemonEvent_Class_Provider::GetInstance(Context& context, const String& nameSpace, const Container_DaemonEvent_Class& instanceName, const PropertySet& propertySet)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_DaemonEvent_Class_Provider::CreateInstance(Context& context, const String& nameSpace, const Container_DaemonEvent_Class& newInstance)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_DaemonEvent_Class_Provider::ModifyInstance(Context& context, const String& nameSpace, const Container_DaemonEvent_Class& modifiedInstance, const PropertySet& propertySet)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_DaemonEvent_Class_Provider::DeleteInstance(Context& context, const String& nameSpace, const Container_DaemonEvent_Class& instanceName)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

#ifdef _MSC_VER
#pragma endregion
#endif

MI_END_NAMESPACE