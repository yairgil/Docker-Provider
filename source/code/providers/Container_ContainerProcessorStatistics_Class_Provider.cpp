/* @migen@ */
#include <MI.h>
#include "Container_ContainerProcessorStatistics_Class_Provider.h"

#include <stdlib.h>
#include <string>
#include <vector>

#include "cJSON_Extend.h"
#include "DockerRemoteApi.h"

using namespace std;

string api_get_top(string id)
{
	return string("GET /containers/" + id + "/top HTTP/1.1\r\n\r\n");
}

MI_BEGIN_NAMESPACE

vector<Container_ContainerProcessorStatistics_Class> Container_ContainerProcessorStatistics_Class_get(cJSON* r, string id)
{
	cJSON* title = cJSON_Get(r, "Titles");
	cJSON* data = cJSON_Get(r, "Processes");

	vector<Container_ContainerProcessorStatistics_Class> insts;

	for (int j = 0; j < cJSON_GetArraySize(data); j++)
	{
		cJSON* pdata = cJSON_GetArrayItem(data, j);

		Container_ContainerProcessorStatistics_Class inst;

		for (int k = 0; k < cJSON_GetArraySize(title); k++)
		{
			if (string("PID") == cJSON_GetArrayItem(title, k)->valuestring)
			{
				inst.ProcessorID_value(atoi(cJSON_GetArrayItem(pdata, k)->valuestring));
				inst.InstanceID_value((id + "_" + cJSON_GetArrayItem(pdata, k)->valuestring).c_str());
			}
			else if (string("%CPU") == cJSON_GetArrayItem(title, k)->valuestring)
			{
				inst.CPUTotalPct_value(atof(cJSON_GetArrayItem(pdata, k)->valuestring) * 100);
			}
			else if (string("%COMMAND") == cJSON_GetArrayItem(title, k)->valuestring)
			{
				inst.ElementName_value(cJSON_GetArrayItem(pdata, k)->valuestring);
			}
			else if (string("CMD") == cJSON_GetArrayItem(title, k)->valuestring)
			{
				inst.ElementName_value(cJSON_GetArrayItem(pdata, k)->valuestring);
			}
		}

		insts.push_back(inst);
	}

	return insts;
}

Container_ContainerProcessorStatistics_Class_Provider::Container_ContainerProcessorStatistics_Class_Provider(Module* module) : m_Module(module){}

Container_ContainerProcessorStatistics_Class_Provider::~Container_ContainerProcessorStatistics_Class_Provider(){}

void Container_ContainerProcessorStatistics_Class_Provider::Load(Context& context)
{
	context.Post(MI_RESULT_OK);
}

void Container_ContainerProcessorStatistics_Class_Provider::Unload(Context& context)
{
	context.Post(MI_RESULT_OK);
}

void Container_ContainerProcessorStatistics_Class_Provider::EnumerateInstances(Context& context, const String& nameSpace, const PropertySet& propertySet, bool keysOnly, const MI_Filter* filter)
{
	try
	{
		vector<string> containerIds = listContainer();
		vector<string> request;

		for (unsigned int i = 0; i < containerIds.size(); i++)
		{
			request.push_back(api_get_top(containerIds[i]));
		}

		vector<cJSON*> response = getResponse(request);

		for (unsigned int i = 0; i < response.size(); i++)
		{
			vector<Container_ContainerProcessorStatistics_Class> insts = Container_ContainerProcessorStatistics_Class_get(response[i], containerIds[i]);

			for (unsigned int k = 0; k < insts.size(); k++)
			{
				context.Post(insts[k]);
			}

			cJSON_Delete(response[i]);
		}

		context.Post(MI_RESULT_OK);
	}
	catch (string& e)
	{
		context.Post(MI_RESULT_FAILED, e.c_str());
	}
}

void Container_ContainerProcessorStatistics_Class_Provider::GetInstance(Context& context, const String& nameSpace, const Container_ContainerProcessorStatistics_Class& instanceName, const PropertySet& propertySet)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_ContainerProcessorStatistics_Class_Provider::CreateInstance(Context& context, const String& nameSpace, const Container_ContainerProcessorStatistics_Class& newInstance)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_ContainerProcessorStatistics_Class_Provider::ModifyInstance(Context& context, const String& nameSpace, const Container_ContainerProcessorStatistics_Class& modifiedInstance, const PropertySet& propertySet)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_ContainerProcessorStatistics_Class_Provider::DeleteInstance(Context& context, const String& nameSpace, const Container_ContainerProcessorStatistics_Class& instanceName)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_ContainerProcessorStatistics_Class_Provider::Invoke_ResetSelectedStats(Context& context, const String& nameSpace, const Container_ContainerProcessorStatistics_Class& instanceName, const Container_ContainerProcessorStatistics_ResetSelectedStats_Class& in)
{
	context.Post(MI_RESULT_NOT_SUPPORTED);
}

MI_END_NAMESPACE
