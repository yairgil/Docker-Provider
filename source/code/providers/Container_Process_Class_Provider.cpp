/* @migen@ */
#include <MI.h>
#include "Container_Process_Class_Provider.h"

#include <vector>
#include <string>
#include <cstdlib>
#include <syslog.h>
#include "../cjson/cJSON.h"
#include "../dockerapi/DockerRemoteApi.h"
#include "../dockerapi/DockerRestHelper.h"

MI_BEGIN_NAMESPACE

class ContainerProcessQuery
{
public:
    ///
    /// \returns Object representing process info for each container
    ///
    static vector<Container_Process_Class> GetProcessInfoPerContainer()
    {
        vector<Container_Process_Class> runningProcessListInstance;

        // Get computer name
        string hostname = getDockerHostName();

        // Request containers
        vector<string> dockerPsRequest(1, DockerRestHelper::restDockerPsRunning());
        vector<cJSON*> dockerPsResponse = getResponse(dockerPsRequest);

        if (!dockerPsResponse.empty() && dockerPsResponse[0])
        {
            for (int i = 0; i < cJSON_GetArraySize(dockerPsResponse[0]); i++)
            {
                cJSON* containerEntry = cJSON_GetArrayItem(dockerPsResponse[0], i);
                if (containerEntry)
                {
                    string containerId = string(cJSON_GetObjectItem(containerEntry, "Id")->valuestring);
                    string containerName;
                    // Get container name
                    cJSON* names = cJSON_GetObjectItem(containerEntry, "Names");
                    if (cJSON_GetArraySize(names))
                    {
                        containerName = string(cJSON_GetArrayItem(names, 0)->valuestring + 1);
                    }

                    // Request container process info
                    vector<string> dockerTopRequest(1, DockerRestHelper::restDockerTop(containerId));
                    vector<cJSON*> dockerTopResponse = getResponse(dockerTopRequest);

                    if (!dockerTopResponse.empty() && dockerTopResponse[0])
                    {
                        //Get process entry
                        cJSON* processArrEntry = cJSON_GetObjectItem(dockerTopResponse[0], "Process");
                        if(processArrEntry != NULL)
                        {
                            for(int j =0; j < cJSON_GetArraySize(processArrEntry); j++)
                            {
                                Container_Process_Class processInstance;

                                cJSON* processEntry = processArrEntry[j];
                                //process scpecific values
                                processInstance.InstanceID_value(containerId.c_str());
                                processInstance.Uid_value(processEntry[0]);
                                processInstance.PID_value(processEntry[1]);
                                processInstance.PPID_value(processEntry[2]);
                                processInstance.C_value(processEntry[3]);
                                processInstance.STIME_value(processEntry[4]);
                                processInstance.Tty_value(processEntry[5]);
                                processInstance.StartTime_value(processEntry[6]);
                                processInstance.Cmd_value(processEntry[7]);
                                //container specific values
                                processInstance.Id_value(containerId.c_str());
                                processInstance.Name_value(containerName.c_str());
                                processInstance.Computer_value(hostname.c_str());
                                runningProcessListInstance.push_front(processInstance);
                            }
                        }
                    }
                    dockerTopResponse.clear();
                }
            }
        }
        dockerPsResponse.clear();
    }
};

Container_Process_Class_Provider::Container_Process_Class_Provider(
    Module* module) :
    m_Module(module)
{
}

Container_Process_Class_Provider::~Container_Process_Class_Provider()
{
}

void Container_Process_Class_Provider::Load(
        Context& context)
{
    context.Post(MI_RESULT_OK);
}

void Container_Process_Class_Provider::Unload(
        Context& context)
{
    context.Post(MI_RESULT_OK);
}

void Container_Process_Class_Provider::EnumerateInstances(
    Context& context,
    const String& nameSpace,
    const PropertySet& propertySet,
    bool keysOnly,
    const MI_Filter* filter)
{
    try
    {
        vector<Container_Process_Class> queryResult = ContainerProcessQuery::GetProcessInfoPerContainer();
        for (unsigned i = 0; i < queryResult.size(); i++)
        {
            context.Post(queryResult[i]);
        }
        context.Post(MI_RESULT_OK);
    }
    catch (std::exception &e)
    {
        syslog(LOG_ERR, "Container_Process %s", e.what());
        context.Post(MI_RESULT_FAILED);
    }
    catch (...)
    {
        syslog(LOG_ERR, "Container_Process Unknown exception");
        context.Post(MI_RESULT_FAILED);
    }
}

void Container_Process_Class_Provider::GetInstance(
    Context& context,
    const String& nameSpace,
    const Container_Process_Class& instanceName,
    const PropertySet& propertySet)
{
    context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_Process_Class_Provider::CreateInstance(
    Context& context,
    const String& nameSpace,
    const Container_Process_Class& newInstance)
{
    context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_Process_Class_Provider::ModifyInstance(
    Context& context,
    const String& nameSpace,
    const Container_Process_Class& modifiedInstance,
    const PropertySet& propertySet)
{
    context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Container_Process_Class_Provider::DeleteInstance(
    Context& context,
    const String& nameSpace,
    const Container_Process_Class& instanceName)
{
    context.Post(MI_RESULT_NOT_SUPPORTED);
}


MI_END_NAMESPACE
