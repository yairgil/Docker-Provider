/* @migen@ */
#include <MI.h>
#include "Container_ImageInventory_Class_Provider.h"

#include <map>
#include <stdio.h>
#include <string>
#include <syslog.h>
#include <unistd.h>
#include <vector>

#include "../cjson/cJSON.h"
#include "../dockerapi/DockerRemoteApi.h"

#define NUMBYTESPERMB 1048576

using namespace std;

MI_BEGIN_NAMESPACE

class InventoryQuery
{
private:
	///
	/// Create the REST request to list images
	///
	/// \returns Request in string format
	///
	static string restDockerImages()
	{
		return "GET /images/json?all=0 HTTP/1.1\r\n\r\n";
	}

	///
	/// Create the REST request to list containers
	///
	/// \returns Request in string format
	///
	static string restDockerPs()
	{
		return "GET /containers/json?all=1 HTTP/1.1\r\n\r\n";
	}

	///
	/// Create the REST request to inspect a container
	///
	/// \param[in] id ID of the container to be inspected
	/// \returns Request in string format
	///
	static string restDockerInspect(string id)
	{
		return "GET /containers/" + id + "/json HTTP/1.1\r\n\r\n";
	}

	///
	/// Select the tag which contains :latest, if not present, select the last tag
	///
	/// \param[in] tags cJSON* representing an array of strings
	///
	static string SelectTag(cJSON* tags)
	{
		string result = "";

		if (tags)
		{
			bool flag = false;

			for (int j = cJSON_GetArraySize(tags) - 1; !flag && j > -1; j--)
			{
				// Get value of tag
				result = string(cJSON_GetArrayItem(tags, j)->valuestring);

				// Use the tag which contains latest
				if (result.find(":latest") != string::npos)
				{
					flag = true;
				}
			}
		}

		return result;
	}

	///
	/// Seperate the repository, image, and image tag strings
	///
	/// \param[in] instance Object representing the image
	/// \param[in] properties Raw string of form image:imagetag or repository/image:imagetag
	///
	static void SetImageRepositoryImageTag(Container_ImageInventory_Class& instance, string properties)
	{
		switch (properties.size())
		{
			default:
			{
				// Find delimiters in the string of format repository/image:imagetag
				int slashLocation = properties.find('/');
				int colonLocation = properties.find(':');

				if ((unsigned)colonLocation != string::npos)
				{
					if ((unsigned)slashLocation >= properties.size())
					{
						// image:imagetag
						instance.Image_value(properties.substr(0, colonLocation).c_str());
						instance.Repository_value("");
					}
					else
					{
						// repository/image:imagetag
						instance.Image_value(properties.substr(slashLocation + 1, colonLocation - slashLocation - 1).c_str());
						instance.Repository_value(properties.substr(0, slashLocation).c_str());
					}

					instance.ImageTag_value(properties.substr(colonLocation + 1).c_str());
					break;
				}
			}
			case 0:
			{
				// Do not crash the program
				syslog(LOG_WARNING, "Container image name (%s) is improperly formed and could not be parsed in SetRepositoryImageTag", properties.c_str());

				instance.Image_value("");
				instance.Repository_value("");
				instance.ImageTag_value("");
				break;
			}
		}
	}

	///
	/// Determine the state of a container and add it to the image count
	///
	/// \param[in] instances Collection of objects representing all images on the host
	/// \param[in] idTable Maps image IDs to indices in the vector
	/// \param[in] entry containing container information
	///
	static void ObtainContainerState(vector<Container_ImageInventory_Class>& instances, map<string, int>& idTable, cJSON* entry)
	{
		cJSON* state = cJSON_GetObjectItem(entry, "State");

		if (state)
		{
			string id = string(cJSON_GetObjectItem(entry, "Image")->valuestring);

			if (cJSON_GetObjectItem(state, "Running")->valueint)
			{
				// Running container
				instances[idTable[id]].Running_value(instances[idTable[id]].Running_value() + 1);
			}
			else
			{
				if (cJSON_GetObjectItem(state, "Paused")->valueint)
				{
					// Paused container
					instances[idTable[id]].Paused_value(instances[idTable[id]].Paused_value() + 1);
				}
				else
				{
					if (cJSON_GetObjectItem(state, "ExitCode")->valueint)
					{
						// Container exited nonzero
						instances[idTable[id]].Failed_value(instances[idTable[id]].Failed_value() + 1);
					}
					else
					{
						// Container exited normally
						instances[idTable[id]].Stopped_value(instances[idTable[id]].Stopped_value() + 1);
					}
				}
			}
		}
		else
		{
			syslog(LOG_WARNING, "Attempt in ObtainContainerState to get container %s state information returned null", cJSON_GetObjectItem(entry, "Id")->valuestring);
		}
	}

	///
	/// Count the running, paused, stopped, and failed containers associated with each image
	///
	/// \param[in] instances Collection of objects representing all images on the host
	/// \param[in] idTable Maps image IDs to indices in the vector
	///
	static void AggregateContainerStatus(vector<Container_ImageInventory_Class>& instances, map<string, int>& idTable)
	{
		// Request containers
		vector<string> request(1, restDockerPs());
		vector<cJSON*> response = getResponse(request);

		// See http://docs.docker.com/reference/api/Container_remote_api_v1.21/#list-containers for example output
		if (!response.empty() && response[0])
		{
			for (int i = 0; i < cJSON_GetArraySize(response[0]); i++)
			{
				cJSON* entry = cJSON_GetArrayItem(response[0], i);

				if (entry)
				{
					// Inspect container
					vector<string> subRequest(1, restDockerInspect(string(cJSON_GetObjectItem(entry, "Id")->valuestring)));
					vector<cJSON*> subResponse = getResponse(subRequest);

					// See http://docs.docker.com/reference/api/Container_remote_api_v1.21/#inspect-a-container for example output
					if (!subResponse.empty() && subResponse[0])
					{
						ObtainContainerState(instances, idTable, subResponse[0]);

						// Clean up object
						cJSON_Delete(subResponse[0]);
					}
					else
					{
						syslog(LOG_WARNING, "API call in AggregateContainerStatus to inspect container %s returned null", cJSON_GetObjectItem(entry, "Id")->valuestring);
					}
				}
				else
				{
					syslog(LOG_WARNING, "Attempt in AggregateContainerStatus to get element %d of container list returned null", i);
				}
			}

			// Clean up object
			cJSON_Delete(response[0]);
		}
		else
		{
			syslog(LOG_WARNING, "API call in AggregateContainerStatus to list containers returned null");
		}
	}

public:
	///
	/// Get information about all images on the host
	///
	/// \returns Vector containing objects representing each image
	///
	static vector<Container_ImageInventory_Class> QueryAll()
	{
		openlog("Container_ImageInventory", LOG_PID | LOG_NDELAY, LOG_LOCAL1);

		vector<Container_ImageInventory_Class> result;
		map<string, int> idTable;

		// Get computer name
		char name[256];
		string hostname = gethostname(name, 256) ? "" : string(name);

		// Request images
		vector<string> request(1, restDockerImages());
		vector<cJSON*> response = getResponse(request);

		// See http://docs.docker.com/reference/api/Container_remote_api_v1.21/#list-images for example output
		if (!response.empty() && response[0])
		{
			for (int i = 0; i < cJSON_GetArraySize(response[0]); i++)
			{
				cJSON* entry = cJSON_GetArrayItem(response[0], i);

				if (entry)
				{
					// New inventory entry
					Container_ImageInventory_Class instance;
					instance.Computer_value(hostname.c_str());

					// Get ID
					instance.InstanceID_value(cJSON_GetObjectItem(entry, "Id")->valuestring);

					// Get size
					char imageSize[128];
					char virtualSize[128];
					sprintf(imageSize, "%d MB", cJSON_GetObjectItem(entry, "Size")->valueint / NUMBYTESPERMB);
					sprintf(virtualSize, "%d MB", cJSON_GetObjectItem(entry, "VirtualSize")->valueint / NUMBYTESPERMB);
					instance.ImageSize_value(imageSize);
					instance.VirtualSize_value(virtualSize);

					// Get image
					SetImageRepositoryImageTag(instance, SelectTag(cJSON_GetObjectItem(entry, "RepoTags")));

					// Default container states
					instance.Running_value(0);
					instance.Paused_value(0);
					instance.Stopped_value(0);
					instance.Failed_value(0);

					idTable[string(cJSON_GetObjectItem(entry, "Id")->valuestring)] = result.size();
					result.push_back(instance);
				}
				else
				{
					syslog(LOG_WARNING, "Attempt in QueryAll to get element %d of image list returned null", i);
				}
			}

			// Clean up object
			cJSON_Delete(response[0]);

			// Get container status
			AggregateContainerStatus(result, idTable);
		}
		else
		{
			syslog(LOG_WARNING, "API call in QueryAll to list images returned null");
		}

		closelog();
		return result;
	}
};

#ifdef _MSC_VER
#pragma region
#endif

Container_ImageInventory_Class_Provider::Container_ImageInventory_Class_Provider(Module* module) : m_Module(module){}

Container_ImageInventory_Class_Provider::~Container_ImageInventory_Class_Provider(){}

void Container_ImageInventory_Class_Provider::Load(Context& context)
{
	context.Post(MI_RESULT_OK);
}

void Container_ImageInventory_Class_Provider::Unload(Context& context)
{
	context.Post(MI_RESULT_OK);
}

void Container_ImageInventory_Class_Provider::EnumerateInstances(Context& context, const String& nameSpace, const PropertySet& propertySet, bool keysOnly, const MI_Filter* filter)
{
	vector<Container_ImageInventory_Class> queryResult = InventoryQuery::QueryAll();

	for (unsigned i = 0; i < queryResult.size(); i++)
	{
		context.Post(queryResult[i]);
	}

	context.Post(MI_RESULT_OK);
}

void Container_ImageInventory_Class_Provider::GetInstance(Context& context, const String& nameSpace, const Container_ImageInventory_Class& instanceName, const PropertySet& propertySet)
{
	context.Post(MI_RESULT_OK);
}

void Container_ImageInventory_Class_Provider::CreateInstance(Context& context, const String& nameSpace, const Container_ImageInventory_Class& newInstance)
{
	context.Post(MI_RESULT_OK);
}

void Container_ImageInventory_Class_Provider::ModifyInstance(Context& context, const String& nameSpace, const Container_ImageInventory_Class& modifiedInstance, const PropertySet& propertySet)
{
	context.Post(MI_RESULT_OK);
}

void Container_ImageInventory_Class_Provider::DeleteInstance(Context& context, const String& nameSpace, const Container_ImageInventory_Class& instanceName)
{
	context.Post(MI_RESULT_OK);
}

#ifdef _MSC_VER
#pragma endregion
#endif

MI_END_NAMESPACE