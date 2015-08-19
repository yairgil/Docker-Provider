/* @migen@ */
#ifndef _Docker_Container_Class_Provider_h
#define _Docker_Container_Class_Provider_h

#include "Docker_Container.h"
#ifdef __cplusplus
# include <micxx/micxx.h>
# include "module.h"

MI_BEGIN_NAMESPACE

/*
**==============================================================================
**
** Docker_Container provider class declaration
**
**==============================================================================
*/

class Docker_Container_Class_Provider
{
/* @MIGEN.BEGIN@ CAUTION: PLEASE DO NOT EDIT OR DELETE THIS LINE. */
private:
    Module* m_Module;

public:
    Docker_Container_Class_Provider(
        Module* module);

    ~Docker_Container_Class_Provider();

    void Load(
        Context& context);

    void Unload(
        Context& context);

    void EnumerateInstances(
        Context& context,
        const String& nameSpace,
        const PropertySet& propertySet,
        bool keysOnly,
        const MI_Filter* filter);

    void GetInstance(
        Context& context,
        const String& nameSpace,
        const Docker_Container_Class& instance,
        const PropertySet& propertySet);

    void CreateInstance(
        Context& context,
        const String& nameSpace,
        const Docker_Container_Class& newInstance);

    void ModifyInstance(
        Context& context,
        const String& nameSpace,
        const Docker_Container_Class& modifiedInstance,
        const PropertySet& propertySet);

    void DeleteInstance(
        Context& context,
        const String& nameSpace,
        const Docker_Container_Class& instance);

    void Invoke_RequestStateChange(
        Context& context,
        const String& nameSpace,
        const Docker_Container_Class& instanceName,
        const Docker_Container_RequestStateChange_Class& in);

    void Invoke_SetPowerState(
        Context& context,
        const String& nameSpace,
        const Docker_Container_Class& instanceName,
        const Docker_Container_SetPowerState_Class& in);

/* @MIGEN.END@ CAUTION: PLEASE DO NOT EDIT OR DELETE THIS LINE. */
};

MI_END_NAMESPACE

#endif /* __cplusplus */

#endif /* _Docker_Container_Class_Provider_h */

