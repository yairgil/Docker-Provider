/* @migen@ */
#include <MI.h>
#include "Container_ContainerStatistics_Class_Provider.h"

#include <map>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>

#include "../cjson/cJSON_Extend.h"
#include "../dockerapi/DockerRemoteApi.h"

using namespace std;

string api_get_container_info(string id)
{
	return string("GET /containers/" + id + "/json HTTP/1.1\r\n\r\n");
}

string api_get_container_stats(string id)
{
	return string("GET /containers/" + id + "/stats?stream=false HTTP/1.1\r\n\r\n");
}

long getreadtimeofdockerapi(char* time)
{
	struct tm tm;
	memset(&tm, 0, sizeof(struct tm));
	string t(time);
	strptime(t.substr(0, 19).c_str(), "%Y-%m-%dT%H:%M:%S", &tm);
	return mktime(&tm) * 10000 + atoi(t.substr(20, 4).c_str());
}

MI_BEGIN_NAMESPACE

void TrySetContainerDiskData(Container_ContainerStatistics_Class& instance, string id)
{
	// Request stats
	vector<string> request(1, api_get_container_stats(id));
	vector<cJSON*> response = getResponse(request);

	instance.DiskBytesRead_value(0);
	instance.DiskBytesWritten_value(0);

	if (response.size() && response[0])
	{
		cJSON* blkio_stats = cJSON_GetObjectItem(response[0], "blkio_stats");

		if (blkio_stats)
		{
			cJSON* values = cJSON_GetObjectItem(blkio_stats, "io_service_bytes_recursive");

			bool readFlag = false;
			bool writeFlag = false;

			for (int i = 0; values && !readFlag && !writeFlag && i < cJSON_GetArraySize(values); i++)
			{
				cJSON* entry = cJSON_GetArrayItem(values, i);

				if (entry)
				{
					cJSON* op = cJSON_GetObjectItem(entry, "op");
					cJSON* rawValue = cJSON_GetObjectItem(entry, "value");

					if (op && rawValue)
					{
						if (!strcmp(op->valuestring, "Read"))
						{
							instance.DiskBytesRead_value((long)rawValue);
							readFlag = true;
						}
						else if (!strcmp(op->valuestring, "Write"))
						{
							instance.DiskBytesWritten_value((long)rawValue);
							writeFlag = true;
						}
					}
				}
			}
		}
	}
}

void Container_ContainerStatistics_Class_set(Container_ContainerStatistics_Class& stats, cJSON* data, map<string, Container_ContainerStatistics_Class>& map)
{
	Container_ContainerStatistics_Class& lstats = map[string(stats.InstanceID().value.Str())];
	long readtime = getreadtimeofdockerapi(cJSON_Get(data, "read")->valuestring);
	stats.updatetime_value(readtime);
	stats.ElementName_value(lstats.ElementName().value);
	// double interval = (readtime - lstats.updatetime().value) / 10000.0;
	stats.NetRXBytes_value(cJSON_Get(data, "network.rx_bytes")->valueint);
	// stats.NetBytes_value(cJSON_Get(data, "network.tx_bytes")->valuedouble + cJSON_Get(data, "network.rx_bytes")->valuedouble);
	stats.NetTXBytes_value(cJSON_Get(data, "network.tx_bytes")->valueint);
	// stats.NetRXKBytesPerSec_value((stats.NetRXBytes().value - lstats.NetRXBytes().value) / 1024 / interval);
	// stats.NetTXKBytesPerSec_value((stats.NetTXBytes().value - lstats.NetTXBytes().value) / 1024 / interval);
	// stats.MemCacheMB_value(cJSON_Get(data, "memory_stats.stats.cache")->valuedouble / 1024 / 1024);
	// stats.MemRSSMB_value(cJSON_Get(data, "memory_stats.stats.total_rss")->valuedouble / 1024 / 1024);
	// stats.MemPGFault_value(cJSON_Get(data, "memory_stats.stats.pgfault")->valuedouble);
	// stats.MemPGFaultPerSec_value((stats.MemPGFault().value - lstats.MemPGFault().value) / interval);
	// stats.MemPGMajFault_value(cJSON_Get(data, "memory_stats.stats.pgmajfault")->valuedouble);
	// stats.MemPGMajFaultPerSec_value((stats.MemPGMajFault().value - lstats.MemPGMajFault().value) / interval);
	// stats.MemUnevictableMB_value(cJSON_Get(data, "memory_stats.stats.unevictable")->valuedouble / 1024 / 1024);
	// stats.MemLimitMB_value(cJSON_Get(data, "memory_stats.limit")->valuedouble / 1024 / 1024);
	stats.MemUsedPct_value(cJSON_Get(data, "memory_stats.usage")->valueint / 1024 / 1024);
	stats.CPUTotal_value(cJSON_Get(data, "cpu_stats.cpu_usage.total_usage")->valueint / 1000000000);
	// stats.CPUHost_value(cJSON_Get(data, "cpu_stats.system_cpu_usage")->valuedouble);
	// stats.CPUSystem_value(cJSON_Get(data, "cpu_stats.cpu_usage.usage_in_kernelmode")->valuedouble);
	int cpu_number = cJSON_GetArraySize(cJSON_Get(data, "cpu_stats.cpu_usage.percpu_usage"));
	
	if ((stats.CPUHost().value - lstats.CPUHost().value))
	{
		stats.CPUTotalPct_value((stats.CPUTotal().value - lstats.CPUTotal().value)*cpu_number * 100 / (stats.CPUHost().value - lstats.CPUHost().value));
	}
	else
	{
		stats.CPUTotalPct_value(0);
	}

	// stats.CPUSystemPct_value((stats.CPUSystem().value - lstats.CPUSystem().value)*cpu_number * 100 / (stats.CPUHost().value - lstats.CPUHost().value));
	map[string(stats.InstanceID().value.Str())] = stats;
}

static map<string, Container_ContainerStatistics_Class> map_data;

Container_ContainerStatistics_Class_Provider::Container_ContainerStatistics_Class_Provider(Module* module) : m_Module(module){}

Container_ContainerStatistics_Class_Provider::~Container_ContainerStatistics_Class_Provider(){}

void Container_ContainerStatistics_Class_Provider::Load(Context& context)
{
	context.Post(MI_RESULT_OK);
}

void Container_ContainerStatistics_Class_Provider::Unload(Context& context)
{
	context.Post(MI_RESULT_OK);
}

void Container_ContainerStatistics_Class_Provider::EnumerateInstances(Context& context, const String& nameSpace, const PropertySet& propertySet, bool keysOnly, const MI_Filter* filter)
{
	try
	{
		vector<string> containers = listContainer();
		vector<string> request;

		for (unsigned int i = 0; i < containers.size(); i++)
		{
			request.push_back(api_get_container_stats(containers[i]));
		}

		for (unsigned int i = 0; i < containers.size(); i++)
		{
			if (map_data.find(containers[i]) == map_data.end())
			{
				request.push_back(api_get_container_info(containers[i]));
			}
		}

		vector<cJSON*> response = getResponse(request);

		for (unsigned int i = containers.size(); i < response.size(); i++)
		{
			string id = cJSON_Get(response[i], "ID")->valuestring;
			Container_ContainerStatistics_Class data;
			data.InstanceID_value(id.c_str());
			data.ElementName_value(cJSON_Get(response[i], "Name")->valuestring);
			map_data[id] = data;
			cJSON_Delete(response[i]);
		}

		for (unsigned int i = 0; i < containers.size(); i++)
		{
			Container_ContainerStatistics_Class inst;
			inst.InstanceID_value(containers[i].c_str());
			Container_ContainerStatistics_Class_set(inst, response[i], map_data);
			TrySetContainerDiskData(inst, containers[i]);

			if (strlen(inst.ElementName().value.Str()) < 192)
			{
				char longName[256];
				sprintf(longName, "%s\\%s", containers[i].c_str(), inst.ElementName().value.Str() + 1);
				inst.InstanceID_value(longName);
			}

			context.Post(inst);
			cJSON_Delete(response[i]);
		}

		context.Post(MI_RESULT_OK);
	}
	catch (string& e)
	{
		context.Post(MI_RESULT_FAILED, e.c_str());
	}
}

void Container_ContainerStatistics_Class_Provider::GetInstance(Context& context, const String& nameSpace, const Container_ContainerStatistics_Class& instanceName, const PropertySet& propertySet)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_ContainerStatistics_Class_Provider::CreateInstance(Context& context, const String& nameSpace, const Container_ContainerStatistics_Class& newInstance)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_ContainerStatistics_Class_Provider::ModifyInstance(Context& context, const String& nameSpace, const Container_ContainerStatistics_Class& modifiedInstance, const PropertySet& propertySet)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_ContainerStatistics_Class_Provider::DeleteInstance(Context& context, const String& nameSpace, const Container_ContainerStatistics_Class& instanceName)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_ContainerStatistics_Class_Provider::Invoke_ResetSelectedStats(Context& context, const String& nameSpace, const Container_ContainerStatistics_Class& instanceName, const Container_ContainerStatistics_ResetSelectedStats_Class& in)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

MI_END_NAMESPACE