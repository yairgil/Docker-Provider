/* @migen@ */
/*
**==============================================================================
**
** WARNING: THIS FILE WAS AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT.
**
**==============================================================================
*/
#include <MI.h>
#include "module.h"
#include "Docker_ContainerStatistics_Class_Provider.h"
#include "Docker_Server_Class_Provider.h"
#include "Docker_ContainerProcessorStatistics_Class_Provider.h"
#include "Docker_Container_Class_Provider.h"

using namespace mi;

MI_EXTERN_C void MI_CALL Docker_ContainerStatistics_Load(
    Docker_ContainerStatistics_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Docker_ContainerStatistics_Class_Provider* prov = new Docker_ContainerStatistics_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Docker_ContainerStatistics_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Docker_ContainerStatistics_Unload(
    Docker_ContainerStatistics_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Docker_ContainerStatistics_Class_Provider* prov = (Docker_ContainerStatistics_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Docker_ContainerStatistics_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Docker_ContainerStatistics_EnumerateInstances(
    Docker_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter)
{
    Docker_ContainerStatistics_Class_Provider* cxxSelf =((Docker_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);

    cxxSelf->EnumerateInstances(
        cxxContext,
        nameSpace,
        __PropertySet(propertySet),
        __bool(keysOnly),
        filter);
}

MI_EXTERN_C void MI_CALL Docker_ContainerStatistics_GetInstance(
    Docker_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_ContainerStatistics* instanceName,
    const MI_PropertySet* propertySet)
{
    Docker_ContainerStatistics_Class_Provider* cxxSelf =((Docker_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_ContainerStatistics_Class cxxInstanceName(instanceName, true);

    cxxSelf->GetInstance(
        cxxContext,
        nameSpace,
        cxxInstanceName,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Docker_ContainerStatistics_CreateInstance(
    Docker_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_ContainerStatistics* newInstance)
{
    Docker_ContainerStatistics_Class_Provider* cxxSelf =((Docker_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_ContainerStatistics_Class cxxNewInstance(newInstance, false);

    cxxSelf->CreateInstance(cxxContext, nameSpace, cxxNewInstance);
}

MI_EXTERN_C void MI_CALL Docker_ContainerStatistics_ModifyInstance(
    Docker_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_ContainerStatistics* modifiedInstance,
    const MI_PropertySet* propertySet)
{
    Docker_ContainerStatistics_Class_Provider* cxxSelf =((Docker_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_ContainerStatistics_Class cxxModifiedInstance(modifiedInstance, false);

    cxxSelf->ModifyInstance(
        cxxContext,
        nameSpace,
        cxxModifiedInstance,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Docker_ContainerStatistics_DeleteInstance(
    Docker_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_ContainerStatistics* instanceName)
{
    Docker_ContainerStatistics_Class_Provider* cxxSelf =((Docker_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_ContainerStatistics_Class cxxInstanceName(instanceName, true);

    cxxSelf->DeleteInstance(cxxContext, nameSpace, cxxInstanceName);
}

MI_EXTERN_C void MI_CALL Docker_ContainerStatistics_Invoke_ResetSelectedStats(
    Docker_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Docker_ContainerStatistics* instanceName,
    const Docker_ContainerStatistics_ResetSelectedStats* in)
{
    Docker_ContainerStatistics_Class_Provider* cxxSelf =((Docker_ContainerStatistics_Class_Provider*)self);
    Docker_ContainerStatistics_Class instance(instanceName, false);
    Context  cxxContext(context);
    Docker_ContainerStatistics_ResetSelectedStats_Class param(in, false);

    cxxSelf->Invoke_ResetSelectedStats(cxxContext, nameSpace, instance, param);
}

MI_EXTERN_C void MI_CALL Docker_Server_Load(
    Docker_Server_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Docker_Server_Class_Provider* prov = new Docker_Server_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Docker_Server_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Docker_Server_Unload(
    Docker_Server_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Docker_Server_Class_Provider* prov = (Docker_Server_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Docker_Server_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Docker_Server_EnumerateInstances(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter)
{
    Docker_Server_Class_Provider* cxxSelf =((Docker_Server_Class_Provider*)self);
    Context  cxxContext(context);

    cxxSelf->EnumerateInstances(
        cxxContext,
        nameSpace,
        __PropertySet(propertySet),
        __bool(keysOnly),
        filter);
}

MI_EXTERN_C void MI_CALL Docker_Server_GetInstance(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Server* instanceName,
    const MI_PropertySet* propertySet)
{
    Docker_Server_Class_Provider* cxxSelf =((Docker_Server_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_Server_Class cxxInstanceName(instanceName, true);

    cxxSelf->GetInstance(
        cxxContext,
        nameSpace,
        cxxInstanceName,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Docker_Server_CreateInstance(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Server* newInstance)
{
    Docker_Server_Class_Provider* cxxSelf =((Docker_Server_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_Server_Class cxxNewInstance(newInstance, false);

    cxxSelf->CreateInstance(cxxContext, nameSpace, cxxNewInstance);
}

MI_EXTERN_C void MI_CALL Docker_Server_ModifyInstance(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Server* modifiedInstance,
    const MI_PropertySet* propertySet)
{
    Docker_Server_Class_Provider* cxxSelf =((Docker_Server_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_Server_Class cxxModifiedInstance(modifiedInstance, false);

    cxxSelf->ModifyInstance(
        cxxContext,
        nameSpace,
        cxxModifiedInstance,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Docker_Server_DeleteInstance(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Server* instanceName)
{
    Docker_Server_Class_Provider* cxxSelf =((Docker_Server_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_Server_Class cxxInstanceName(instanceName, true);

    cxxSelf->DeleteInstance(cxxContext, nameSpace, cxxInstanceName);
}

MI_EXTERN_C void MI_CALL Docker_ContainerProcessorStatistics_Load(
    Docker_ContainerProcessorStatistics_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Docker_ContainerProcessorStatistics_Class_Provider* prov = new Docker_ContainerProcessorStatistics_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Docker_ContainerProcessorStatistics_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Docker_ContainerProcessorStatistics_Unload(
    Docker_ContainerProcessorStatistics_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Docker_ContainerProcessorStatistics_Class_Provider* prov = (Docker_ContainerProcessorStatistics_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Docker_ContainerProcessorStatistics_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Docker_ContainerProcessorStatistics_EnumerateInstances(
    Docker_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter)
{
    Docker_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Docker_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);

    cxxSelf->EnumerateInstances(
        cxxContext,
        nameSpace,
        __PropertySet(propertySet),
        __bool(keysOnly),
        filter);
}

MI_EXTERN_C void MI_CALL Docker_ContainerProcessorStatistics_GetInstance(
    Docker_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_ContainerProcessorStatistics* instanceName,
    const MI_PropertySet* propertySet)
{
    Docker_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Docker_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_ContainerProcessorStatistics_Class cxxInstanceName(instanceName, true);

    cxxSelf->GetInstance(
        cxxContext,
        nameSpace,
        cxxInstanceName,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Docker_ContainerProcessorStatistics_CreateInstance(
    Docker_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_ContainerProcessorStatistics* newInstance)
{
    Docker_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Docker_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_ContainerProcessorStatistics_Class cxxNewInstance(newInstance, false);

    cxxSelf->CreateInstance(cxxContext, nameSpace, cxxNewInstance);
}

MI_EXTERN_C void MI_CALL Docker_ContainerProcessorStatistics_ModifyInstance(
    Docker_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_ContainerProcessorStatistics* modifiedInstance,
    const MI_PropertySet* propertySet)
{
    Docker_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Docker_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_ContainerProcessorStatistics_Class cxxModifiedInstance(modifiedInstance, false);

    cxxSelf->ModifyInstance(
        cxxContext,
        nameSpace,
        cxxModifiedInstance,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Docker_ContainerProcessorStatistics_DeleteInstance(
    Docker_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_ContainerProcessorStatistics* instanceName)
{
    Docker_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Docker_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_ContainerProcessorStatistics_Class cxxInstanceName(instanceName, true);

    cxxSelf->DeleteInstance(cxxContext, nameSpace, cxxInstanceName);
}

MI_EXTERN_C void MI_CALL Docker_ContainerProcessorStatistics_Invoke_ResetSelectedStats(
    Docker_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Docker_ContainerProcessorStatistics* instanceName,
    const Docker_ContainerProcessorStatistics_ResetSelectedStats* in)
{
    Docker_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Docker_ContainerProcessorStatistics_Class_Provider*)self);
    Docker_ContainerProcessorStatistics_Class instance(instanceName, false);
    Context  cxxContext(context);
    Docker_ContainerProcessorStatistics_ResetSelectedStats_Class param(in, false);

    cxxSelf->Invoke_ResetSelectedStats(cxxContext, nameSpace, instance, param);
}

MI_EXTERN_C void MI_CALL Docker_Container_Load(
    Docker_Container_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Docker_Container_Class_Provider* prov = new Docker_Container_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Docker_Container_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Docker_Container_Unload(
    Docker_Container_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Docker_Container_Class_Provider* prov = (Docker_Container_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Docker_Container_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Docker_Container_EnumerateInstances(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter)
{
    Docker_Container_Class_Provider* cxxSelf =((Docker_Container_Class_Provider*)self);
    Context  cxxContext(context);

    cxxSelf->EnumerateInstances(
        cxxContext,
        nameSpace,
        __PropertySet(propertySet),
        __bool(keysOnly),
        filter);
}

MI_EXTERN_C void MI_CALL Docker_Container_GetInstance(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Container* instanceName,
    const MI_PropertySet* propertySet)
{
    Docker_Container_Class_Provider* cxxSelf =((Docker_Container_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_Container_Class cxxInstanceName(instanceName, true);

    cxxSelf->GetInstance(
        cxxContext,
        nameSpace,
        cxxInstanceName,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Docker_Container_CreateInstance(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Container* newInstance)
{
    Docker_Container_Class_Provider* cxxSelf =((Docker_Container_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_Container_Class cxxNewInstance(newInstance, false);

    cxxSelf->CreateInstance(cxxContext, nameSpace, cxxNewInstance);
}

MI_EXTERN_C void MI_CALL Docker_Container_ModifyInstance(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Container* modifiedInstance,
    const MI_PropertySet* propertySet)
{
    Docker_Container_Class_Provider* cxxSelf =((Docker_Container_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_Container_Class cxxModifiedInstance(modifiedInstance, false);

    cxxSelf->ModifyInstance(
        cxxContext,
        nameSpace,
        cxxModifiedInstance,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Docker_Container_DeleteInstance(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Container* instanceName)
{
    Docker_Container_Class_Provider* cxxSelf =((Docker_Container_Class_Provider*)self);
    Context  cxxContext(context);
    Docker_Container_Class cxxInstanceName(instanceName, true);

    cxxSelf->DeleteInstance(cxxContext, nameSpace, cxxInstanceName);
}

MI_EXTERN_C void MI_CALL Docker_Container_Invoke_RequestStateChange(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Docker_Container* instanceName,
    const Docker_Container_RequestStateChange* in)
{
    Docker_Container_Class_Provider* cxxSelf =((Docker_Container_Class_Provider*)self);
    Docker_Container_Class instance(instanceName, false);
    Context  cxxContext(context);
    Docker_Container_RequestStateChange_Class param(in, false);

    cxxSelf->Invoke_RequestStateChange(cxxContext, nameSpace, instance, param);
}

MI_EXTERN_C void MI_CALL Docker_Container_Invoke_SetPowerState(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Docker_Container* instanceName,
    const Docker_Container_SetPowerState* in)
{
    Docker_Container_Class_Provider* cxxSelf =((Docker_Container_Class_Provider*)self);
    Docker_Container_Class instance(instanceName, false);
    Context  cxxContext(context);
    Docker_Container_SetPowerState_Class param(in, false);

    cxxSelf->Invoke_SetPowerState(cxxContext, nameSpace, instance, param);
}


MI_EXTERN_C MI_SchemaDecl schemaDecl;

void MI_CALL Load(MI_Module_Self** self, struct _MI_Context* context)
{
    *self = (MI_Module_Self*)new Module;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

void MI_CALL Unload(MI_Module_Self* self, struct _MI_Context* context)
{
    Module* module = (Module*)self;
    delete module;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C MI_EXPORT MI_Module* MI_MAIN_CALL MI_Main(MI_Server* server)
{
    /* WARNING: THIS FUNCTION AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT. */
    extern MI_Server* __mi_server;
    static MI_Module module;
    __mi_server = server;
    module.flags |= MI_MODULE_FLAG_STANDARD_QUALIFIERS;
    module.flags |= MI_MODULE_FLAG_CPLUSPLUS;
    module.charSize = sizeof(MI_Char);
    module.version = MI_VERSION;
    module.generatorVersion = MI_MAKE_VERSION(1,0,8);
    module.schemaDecl = &schemaDecl;
    module.Load = Load;
    module.Unload = Unload;
    return &module;
}

