/* @migen@ */
#ifndef _Container_Container_Class_Provider_h
#define _Container_Container_Class_Provider_h

#include "Container_Container.h"
#ifdef __cplusplus
# include <micxx/micxx.h>
# include "module.h"

MI_BEGIN_NAMESPACE

/*
**==============================================================================
**
** Container_Container provider class declaration
**
**==============================================================================
*/

class Container_Container_Class_Provider
{
/* @MIGEN.BEGIN@ CAUTION: PLEASE DO NOT EDIT OR DELETE THIS LINE. */
private:
    Module* m_Module;

public:
    Container_Container_Class_Provider(
        Module* module);

    ~Container_Container_Class_Provider();

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
        const Container_Container_Class& instance,
        const PropertySet& propertySet);

    void CreateInstance(
        Context& context,
        const String& nameSpace,
        const Container_Container_Class& newInstance);

    void ModifyInstance(
        Context& context,
        const String& nameSpace,
        const Container_Container_Class& modifiedInstance,
        const PropertySet& propertySet);

    void DeleteInstance(
        Context& context,
        const String& nameSpace,
        const Container_Container_Class& instance);

    void Invoke_RequestStateChange(
        Context& context,
        const String& nameSpace,
        const Container_Container_Class& instanceName,
        const Container_Container_RequestStateChange_Class& in);

    void Invoke_SetPowerState(
        Context& context,
        const String& nameSpace,
        const Container_Container_Class& instanceName,
        const Container_Container_SetPowerState_Class& in);

/* @MIGEN.END@ CAUTION: PLEASE DO NOT EDIT OR DELETE THIS LINE. */
};

MI_END_NAMESPACE

#endif /* __cplusplus */

#endif /* _Container_Container_Class_Provider_h */

