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
#include "Container_ImageInventory_Class_Provider.h"
#include "Container_DaemonEvent_Class_Provider.h"
#include "Container_Server_Class_Provider.h"
#include "Container_Container_Class_Provider.h"
#include "Container_ContainerStatistics_Class_Provider.h"
#include "Container_ContainerProcessorStatistics_Class_Provider.h"

using namespace mi;

MI_EXTERN_C void MI_CALL Container_ImageInventory_Load(
    Container_ImageInventory_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_ImageInventory_Class_Provider* prov = new Container_ImageInventory_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Container_ImageInventory_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Container_ImageInventory_Unload(
    Container_ImageInventory_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_ImageInventory_Class_Provider* prov = (Container_ImageInventory_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Container_ImageInventory_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Container_DaemonEvent_Load(
    Container_DaemonEvent_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_DaemonEvent_Class_Provider* prov = new Container_DaemonEvent_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Container_DaemonEvent_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Container_DaemonEvent_Unload(
    Container_DaemonEvent_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_DaemonEvent_Class_Provider* prov = (Container_DaemonEvent_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Container_DaemonEvent_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Container_Server_Load(
    Container_Server_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_Server_Class_Provider* prov = new Container_Server_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Container_Server_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Container_Server_Unload(
    Container_Server_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_Server_Class_Provider* prov = (Container_Server_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Container_Server_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Container_Server_EnumerateInstances(
    Container_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter)
{
    Container_Server_Class_Provider* cxxSelf =((Container_Server_Class_Provider*)self);
    Context  cxxContext(context);

    cxxSelf->EnumerateInstances(
        cxxContext,
        nameSpace,
        __PropertySet(propertySet),
        __bool(keysOnly),
        filter);
}

MI_EXTERN_C void MI_CALL Container_Server_GetInstance(
    Container_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_Server* instanceName,
    const MI_PropertySet* propertySet)
{
    Container_Server_Class_Provider* cxxSelf =((Container_Server_Class_Provider*)self);
    Context  cxxContext(context);
    Container_Server_Class cxxInstanceName(instanceName, true);

    cxxSelf->GetInstance(
        cxxContext,
        nameSpace,
        cxxInstanceName,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Container_Server_CreateInstance(
    Container_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_Server* newInstance)
{
    Container_Server_Class_Provider* cxxSelf =((Container_Server_Class_Provider*)self);
    Context  cxxContext(context);
    Container_Server_Class cxxNewInstance(newInstance, false);

    cxxSelf->CreateInstance(cxxContext, nameSpace, cxxNewInstance);
}

MI_EXTERN_C void MI_CALL Container_Server_ModifyInstance(
    Container_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_Server* modifiedInstance,
    const MI_PropertySet* propertySet)
{
    Container_Server_Class_Provider* cxxSelf =((Container_Server_Class_Provider*)self);
    Context  cxxContext(context);
    Container_Server_Class cxxModifiedInstance(modifiedInstance, false);

    cxxSelf->ModifyInstance(
        cxxContext,
        nameSpace,
        cxxModifiedInstance,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Container_Server_DeleteInstance(
    Container_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_Server* instanceName)
{
    Container_Server_Class_Provider* cxxSelf =((Container_Server_Class_Provider*)self);
    Context  cxxContext(context);
    Container_Server_Class cxxInstanceName(instanceName, true);

    cxxSelf->DeleteInstance(cxxContext, nameSpace, cxxInstanceName);
}

MI_EXTERN_C void MI_CALL Container_Container_Load(
    Container_Container_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_Container_Class_Provider* prov = new Container_Container_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Container_Container_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Container_Container_Unload(
    Container_Container_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_Container_Class_Provider* prov = (Container_Container_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Container_Container_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Container_Container_EnumerateInstances(
    Container_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter)
{
    Container_Container_Class_Provider* cxxSelf =((Container_Container_Class_Provider*)self);
    Context  cxxContext(context);

    cxxSelf->EnumerateInstances(
        cxxContext,
        nameSpace,
        __PropertySet(propertySet),
        __bool(keysOnly),
        filter);
}

MI_EXTERN_C void MI_CALL Container_Container_GetInstance(
    Container_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_Container* instanceName,
    const MI_PropertySet* propertySet)
{
    Container_Container_Class_Provider* cxxSelf =((Container_Container_Class_Provider*)self);
    Context  cxxContext(context);
    Container_Container_Class cxxInstanceName(instanceName, true);

    cxxSelf->GetInstance(
        cxxContext,
        nameSpace,
        cxxInstanceName,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Container_Container_CreateInstance(
    Container_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_Container* newInstance)
{
    Container_Container_Class_Provider* cxxSelf =((Container_Container_Class_Provider*)self);
    Context  cxxContext(context);
    Container_Container_Class cxxNewInstance(newInstance, false);

    cxxSelf->CreateInstance(cxxContext, nameSpace, cxxNewInstance);
}

MI_EXTERN_C void MI_CALL Container_Container_ModifyInstance(
    Container_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_Container* modifiedInstance,
    const MI_PropertySet* propertySet)
{
    Container_Container_Class_Provider* cxxSelf =((Container_Container_Class_Provider*)self);
    Context  cxxContext(context);
    Container_Container_Class cxxModifiedInstance(modifiedInstance, false);

    cxxSelf->ModifyInstance(
        cxxContext,
        nameSpace,
        cxxModifiedInstance,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Container_Container_DeleteInstance(
    Container_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_Container* instanceName)
{
    Container_Container_Class_Provider* cxxSelf =((Container_Container_Class_Provider*)self);
    Context  cxxContext(context);
    Container_Container_Class cxxInstanceName(instanceName, true);

    cxxSelf->DeleteInstance(cxxContext, nameSpace, cxxInstanceName);
}

MI_EXTERN_C void MI_CALL Container_Container_Invoke_RequestStateChange(
    Container_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Container_Container* instanceName,
    const Container_Container_RequestStateChange* in)
{
    Container_Container_Class_Provider* cxxSelf =((Container_Container_Class_Provider*)self);
    Container_Container_Class instance(instanceName, false);
    Context  cxxContext(context);
    Container_Container_RequestStateChange_Class param(in, false);

    cxxSelf->Invoke_RequestStateChange(cxxContext, nameSpace, instance, param);
}

MI_EXTERN_C void MI_CALL Container_Container_Invoke_SetPowerState(
    Container_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Container_Container* instanceName,
    const Container_Container_SetPowerState* in)
{
    Container_Container_Class_Provider* cxxSelf =((Container_Container_Class_Provider*)self);
    Container_Container_Class instance(instanceName, false);
    Context  cxxContext(context);
    Container_Container_SetPowerState_Class param(in, false);

    cxxSelf->Invoke_SetPowerState(cxxContext, nameSpace, instance, param);
}

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_Load(
    Container_ContainerStatistics_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_ContainerStatistics_Class_Provider* prov = new Container_ContainerStatistics_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Container_ContainerStatistics_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_Unload(
    Container_ContainerStatistics_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_ContainerStatistics_Class_Provider* prov = (Container_ContainerStatistics_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Container_ContainerStatistics_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_EnumerateInstances(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter)
{
    Container_ContainerStatistics_Class_Provider* cxxSelf =((Container_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);

    cxxSelf->EnumerateInstances(
        cxxContext,
        nameSpace,
        __PropertySet(propertySet),
        __bool(keysOnly),
        filter);
}

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_GetInstance(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerStatistics* instanceName,
    const MI_PropertySet* propertySet)
{
    Container_ContainerStatistics_Class_Provider* cxxSelf =((Container_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Container_ContainerStatistics_Class cxxInstanceName(instanceName, true);

    cxxSelf->GetInstance(
        cxxContext,
        nameSpace,
        cxxInstanceName,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_CreateInstance(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerStatistics* newInstance)
{
    Container_ContainerStatistics_Class_Provider* cxxSelf =((Container_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Container_ContainerStatistics_Class cxxNewInstance(newInstance, false);

    cxxSelf->CreateInstance(cxxContext, nameSpace, cxxNewInstance);
}

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_ModifyInstance(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerStatistics* modifiedInstance,
    const MI_PropertySet* propertySet)
{
    Container_ContainerStatistics_Class_Provider* cxxSelf =((Container_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Container_ContainerStatistics_Class cxxModifiedInstance(modifiedInstance, false);

    cxxSelf->ModifyInstance(
        cxxContext,
        nameSpace,
        cxxModifiedInstance,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_DeleteInstance(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerStatistics* instanceName)
{
    Container_ContainerStatistics_Class_Provider* cxxSelf =((Container_ContainerStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Container_ContainerStatistics_Class cxxInstanceName(instanceName, true);

    cxxSelf->DeleteInstance(cxxContext, nameSpace, cxxInstanceName);
}

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_Invoke_ResetSelectedStats(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Container_ContainerStatistics* instanceName,
    const Container_ContainerStatistics_ResetSelectedStats* in)
{
    Container_ContainerStatistics_Class_Provider* cxxSelf =((Container_ContainerStatistics_Class_Provider*)self);
    Container_ContainerStatistics_Class instance(instanceName, false);
    Context  cxxContext(context);
    Container_ContainerStatistics_ResetSelectedStats_Class param(in, false);

    cxxSelf->Invoke_ResetSelectedStats(cxxContext, nameSpace, instance, param);
}

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_Load(
    Container_ContainerProcessorStatistics_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_ContainerProcessorStatistics_Class_Provider* prov = new Container_ContainerProcessorStatistics_Class_Provider((Module*)selfModule);

    prov->Load(ctx);
    if (MI_RESULT_OK != r)
    {
        delete prov;
        MI_Context_PostResult(context, r);
        return;
    }
    *self = (Container_ContainerProcessorStatistics_Self*)prov;
    MI_Context_PostResult(context, MI_RESULT_OK);
}

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_Unload(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context)
{
    MI_Result r = MI_RESULT_OK;
    Context ctx(context, &r);
    Container_ContainerProcessorStatistics_Class_Provider* prov = (Container_ContainerProcessorStatistics_Class_Provider*)self;

    prov->Unload(ctx);
    delete ((Container_ContainerProcessorStatistics_Class_Provider*)self);
    MI_Context_PostResult(context, r);
}

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_EnumerateInstances(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter)
{
    Container_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Container_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);

    cxxSelf->EnumerateInstances(
        cxxContext,
        nameSpace,
        __PropertySet(propertySet),
        __bool(keysOnly),
        filter);
}

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_GetInstance(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerProcessorStatistics* instanceName,
    const MI_PropertySet* propertySet)
{
    Container_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Container_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Container_ContainerProcessorStatistics_Class cxxInstanceName(instanceName, true);

    cxxSelf->GetInstance(
        cxxContext,
        nameSpace,
        cxxInstanceName,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_CreateInstance(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerProcessorStatistics* newInstance)
{
    Container_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Container_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Container_ContainerProcessorStatistics_Class cxxNewInstance(newInstance, false);

    cxxSelf->CreateInstance(cxxContext, nameSpace, cxxNewInstance);
}

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_ModifyInstance(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerProcessorStatistics* modifiedInstance,
    const MI_PropertySet* propertySet)
{
    Container_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Container_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Container_ContainerProcessorStatistics_Class cxxModifiedInstance(modifiedInstance, false);

    cxxSelf->ModifyInstance(
        cxxContext,
        nameSpace,
        cxxModifiedInstance,
        __PropertySet(propertySet));
}

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_DeleteInstance(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerProcessorStatistics* instanceName)
{
    Container_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Container_ContainerProcessorStatistics_Class_Provider*)self);
    Context  cxxContext(context);
    Container_ContainerProcessorStatistics_Class cxxInstanceName(instanceName, true);

    cxxSelf->DeleteInstance(cxxContext, nameSpace, cxxInstanceName);
}

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_Invoke_ResetSelectedStats(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Container_ContainerProcessorStatistics* instanceName,
    const Container_ContainerProcessorStatistics_ResetSelectedStats* in)
{
    Container_ContainerProcessorStatistics_Class_Provider* cxxSelf =((Container_ContainerProcessorStatistics_Class_Provider*)self);
    Container_ContainerProcessorStatistics_Class instance(instanceName, false);
    Context  cxxContext(context);
    Container_ContainerProcessorStatistics_ResetSelectedStats_Class param(in, false);

    cxxSelf->Invoke_ResetSelectedStats(cxxContext, nameSpace, instance, param);
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

