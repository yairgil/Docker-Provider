/* @migen@ */
#include <MI.h>
#include "Docker_Server_Class_Provider.h"
#include "DockerRemoteApi.h"
#include "cJSON_Extend.h"
#define api_get_server_info string("GET /info HTTP/1.1\r\n\r\n")
#define api_get_server_version string("GET /version HTTP/1.1\r\n\r\n")

MI_BEGIN_NAMESPACE

Docker_Server_Class_Provider::Docker_Server_Class_Provider(
    Module* module) :
    m_Module(module)
{
}

Docker_Server_Class_Provider::~Docker_Server_Class_Provider()
{
}

void Docker_Server_Class_Provider::Load(
        Context& context)
{
    context.Post(MI_RESULT_OK);
}

void Docker_Server_Class_Provider::Unload(
        Context& context)
{
    context.Post(MI_RESULT_OK);
}

void Docker_Server_Class_set(Docker_Server_Class& inst, cJSON* version, cJSON* info)
{
    inst.ProductIdentifyingNumber_value("0");
    inst.ProductName_value("Docker");
    inst.ProductVendor_value("Docker");
    inst.SystemID_value("Docker");
    inst.CollectionID_value("Docker");
    inst.InstanceID_value(cJSON_Get(info, "Name")->valuestring);
    inst.ProductVersion_value(cJSON_Get(version, "Version")->valuestring);
    inst.KernelVersion_value(cJSON_Get(version, "KernelVersion")->valuestring);
    inst.Containers_value(cJSON_Get(info, "Containers")->valuedouble);
    inst.DockerRootDir_value(cJSON_Get(info, "DockerRootDir")->valuestring);
    inst.Driver_value(cJSON_Get(info, "Driver")->valuestring);
    inst.DriverStatus_value(4);
    inst.Images_value(cJSON_Get(info, "Images")->valuedouble);
    inst.InitPath_value(cJSON_Get(info, "InitPath")->valuestring);
    inst.MemTotal_value(cJSON_Get(info, "MemTotal")->valuedouble);
    inst.MemLimit_value(cJSON_Get(info, "MemoryLimit")->valueint);
    inst.SwapLimit_value(cJSON_Get(info, "SwapLimit")->valueint);
    inst.Name_value(cJSON_Get(info, "Name")->valuestring);
    inst.NCPU_value(cJSON_Get(info, "NCPU")->valuedouble);
    return;
}

void Docker_Server_Class_Provider::EnumerateInstances(
    Context& context,
    const String& nameSpace,
    const PropertySet& propertySet,
    bool keysOnly,
    const MI_Filter* filter)
{
    try {
        Docker_Server_Class inst;
        vector<string> request;
        request.push_back(api_get_server_version);
        request.push_back(api_get_server_info);
        vector<cJSON*> response = getResponse(request);
        Docker_Server_Class_set(inst, response[0], response[1]);
        cJSON_Delete(response[0]);
        cJSON_Delete(response[1]);
        context.Post(inst);
        context.Post(MI_RESULT_OK);
    }
    catch(string& e){
        context.Post(MI_RESULT_FAILED,e.c_str());
        return;
    }
}


void Docker_Server_Class_Provider::GetInstance(
    Context& context,
    const String& nameSpace,
    const Docker_Server_Class& instanceName,
    const PropertySet& propertySet)
{
    context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Docker_Server_Class_Provider::CreateInstance(
    Context& context,
    const String& nameSpace,
    const Docker_Server_Class& newInstance)
{
    context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Docker_Server_Class_Provider::ModifyInstance(
    Context& context,
    const String& nameSpace,
    const Docker_Server_Class& modifiedInstance,
    const PropertySet& propertySet)
{
    context.Post(MI_RESULT_NOT_SUPPORTED);
}

void Docker_Server_Class_Provider::DeleteInstance(
    Context& context,
    const String& nameSpace,
    const Docker_Server_Class& instanceName)
{
    context.Post(MI_RESULT_NOT_SUPPORTED);
}


MI_END_NAMESPACE
