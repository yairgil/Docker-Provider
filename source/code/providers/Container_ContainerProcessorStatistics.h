/* @migen@ */
/*
**==============================================================================
**
** WARNING: THIS FILE WAS AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT.
**
**==============================================================================
*/
#ifndef _Container_ContainerProcessorStatistics_h
#define _Container_ContainerProcessorStatistics_h

#include <MI.h>
#include "CIM_StatisticalData.h"

/*
**==============================================================================
**
** Container_ContainerProcessorStatistics [Container_ContainerProcessorStatistics]
**
** Keys:
**    InstanceID
**
**==============================================================================
*/

typedef struct _Container_ContainerProcessorStatistics /* extends CIM_StatisticalData */
{
    MI_Instance __instance;
    /* CIM_ManagedElement properties */
    /*KEY*/ MI_ConstStringField InstanceID;
    MI_ConstStringField Caption;
    MI_ConstStringField Description;
    MI_ConstStringField ElementName;
    /* CIM_StatisticalData properties */
    MI_ConstDatetimeField StartStatisticTime;
    MI_ConstDatetimeField StatisticTime;
    MI_ConstDatetimeField SampleInterval;
    /* Container_ContainerProcessorStatistics properties */
    MI_ConstUint16Field ProcessorID;
    MI_ConstUint64Field CPUTotal;
    MI_ConstUint16Field CPUTotalPct;
}
Container_ContainerProcessorStatistics;

typedef struct _Container_ContainerProcessorStatistics_Ref
{
    Container_ContainerProcessorStatistics* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Container_ContainerProcessorStatistics_Ref;

typedef struct _Container_ContainerProcessorStatistics_ConstRef
{
    MI_CONST Container_ContainerProcessorStatistics* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Container_ContainerProcessorStatistics_ConstRef;

typedef struct _Container_ContainerProcessorStatistics_Array
{
    struct _Container_ContainerProcessorStatistics** data;
    MI_Uint32 size;
}
Container_ContainerProcessorStatistics_Array;

typedef struct _Container_ContainerProcessorStatistics_ConstArray
{
    struct _Container_ContainerProcessorStatistics MI_CONST* MI_CONST* data;
    MI_Uint32 size;
}
Container_ContainerProcessorStatistics_ConstArray;

typedef struct _Container_ContainerProcessorStatistics_ArrayRef
{
    Container_ContainerProcessorStatistics_Array value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Container_ContainerProcessorStatistics_ArrayRef;

typedef struct _Container_ContainerProcessorStatistics_ConstArrayRef
{
    Container_ContainerProcessorStatistics_ConstArray value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Container_ContainerProcessorStatistics_ConstArrayRef;

MI_EXTERN_C MI_CONST MI_ClassDecl Container_ContainerProcessorStatistics_rtti;

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Construct(
    Container_ContainerProcessorStatistics* self,
    MI_Context* context)
{
    return MI_ConstructInstance(context, &Container_ContainerProcessorStatistics_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clone(
    const Container_ContainerProcessorStatistics* self,
    Container_ContainerProcessorStatistics** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Boolean MI_CALL Container_ContainerProcessorStatistics_IsA(
    const MI_Instance* self)
{
    MI_Boolean res = MI_FALSE;
    return MI_Instance_IsA(self, &Container_ContainerProcessorStatistics_rtti, &res) == MI_RESULT_OK && res;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Destruct(Container_ContainerProcessorStatistics* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Delete(Container_ContainerProcessorStatistics* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Post(
    const Container_ContainerProcessorStatistics* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_InstanceID(
    Container_ContainerProcessorStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_SetPtr_InstanceID(
    Container_ContainerProcessorStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_InstanceID(
    Container_ContainerProcessorStatistics* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_Caption(
    Container_ContainerProcessorStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_SetPtr_Caption(
    Container_ContainerProcessorStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_Caption(
    Container_ContainerProcessorStatistics* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        1);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_Description(
    Container_ContainerProcessorStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_SetPtr_Description(
    Container_ContainerProcessorStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_Description(
    Container_ContainerProcessorStatistics* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_ElementName(
    Container_ContainerProcessorStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_SetPtr_ElementName(
    Container_ContainerProcessorStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_ElementName(
    Container_ContainerProcessorStatistics* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        3);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_StartStatisticTime(
    Container_ContainerProcessorStatistics* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->StartStatisticTime)->value = x;
    ((MI_DatetimeField*)&self->StartStatisticTime)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_StartStatisticTime(
    Container_ContainerProcessorStatistics* self)
{
    memset((void*)&self->StartStatisticTime, 0, sizeof(self->StartStatisticTime));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_StatisticTime(
    Container_ContainerProcessorStatistics* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->StatisticTime)->value = x;
    ((MI_DatetimeField*)&self->StatisticTime)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_StatisticTime(
    Container_ContainerProcessorStatistics* self)
{
    memset((void*)&self->StatisticTime, 0, sizeof(self->StatisticTime));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_SampleInterval(
    Container_ContainerProcessorStatistics* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->SampleInterval)->value = x;
    ((MI_DatetimeField*)&self->SampleInterval)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_SampleInterval(
    Container_ContainerProcessorStatistics* self)
{
    memset((void*)&self->SampleInterval, 0, sizeof(self->SampleInterval));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_ProcessorID(
    Container_ContainerProcessorStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->ProcessorID)->value = x;
    ((MI_Uint16Field*)&self->ProcessorID)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_ProcessorID(
    Container_ContainerProcessorStatistics* self)
{
    memset((void*)&self->ProcessorID, 0, sizeof(self->ProcessorID));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_CPUTotal(
    Container_ContainerProcessorStatistics* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->CPUTotal)->value = x;
    ((MI_Uint64Field*)&self->CPUTotal)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_CPUTotal(
    Container_ContainerProcessorStatistics* self)
{
    memset((void*)&self->CPUTotal, 0, sizeof(self->CPUTotal));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Set_CPUTotalPct(
    Container_ContainerProcessorStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->CPUTotalPct)->value = x;
    ((MI_Uint16Field*)&self->CPUTotalPct)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_Clear_CPUTotalPct(
    Container_ContainerProcessorStatistics* self)
{
    memset((void*)&self->CPUTotalPct, 0, sizeof(self->CPUTotalPct));
    return MI_RESULT_OK;
}

/*
**==============================================================================
**
** Container_ContainerProcessorStatistics.ResetSelectedStats()
**
**==============================================================================
*/

typedef struct _Container_ContainerProcessorStatistics_ResetSelectedStats
{
    MI_Instance __instance;
    /*OUT*/ MI_ConstUint32Field MIReturn;
    /*IN*/ MI_ConstStringAField SelectedStatistics;
}
Container_ContainerProcessorStatistics_ResetSelectedStats;

MI_EXTERN_C MI_CONST MI_MethodDecl Container_ContainerProcessorStatistics_ResetSelectedStats_rtti;

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Construct(
    Container_ContainerProcessorStatistics_ResetSelectedStats* self,
    MI_Context* context)
{
    return MI_ConstructParameters(context, &Container_ContainerProcessorStatistics_ResetSelectedStats_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Clone(
    const Container_ContainerProcessorStatistics_ResetSelectedStats* self,
    Container_ContainerProcessorStatistics_ResetSelectedStats** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Destruct(
    Container_ContainerProcessorStatistics_ResetSelectedStats* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Delete(
    Container_ContainerProcessorStatistics_ResetSelectedStats* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Post(
    const Container_ContainerProcessorStatistics_ResetSelectedStats* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Set_MIReturn(
    Container_ContainerProcessorStatistics_ResetSelectedStats* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MIReturn)->value = x;
    ((MI_Uint32Field*)&self->MIReturn)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Clear_MIReturn(
    Container_ContainerProcessorStatistics_ResetSelectedStats* self)
{
    memset((void*)&self->MIReturn, 0, sizeof(self->MIReturn));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Set_SelectedStatistics(
    Container_ContainerProcessorStatistics_ResetSelectedStats* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&arr,
        MI_STRINGA,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_SetPtr_SelectedStatistics(
    Container_ContainerProcessorStatistics_ResetSelectedStats* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&arr,
        MI_STRINGA,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerProcessorStatistics_ResetSelectedStats_Clear_SelectedStatistics(
    Container_ContainerProcessorStatistics_ResetSelectedStats* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        1);
}

/*
**==============================================================================
**
** Container_ContainerProcessorStatistics provider function prototypes
**
**==============================================================================
*/

/* The developer may optionally define this structure */
typedef struct _Container_ContainerProcessorStatistics_Self Container_ContainerProcessorStatistics_Self;

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_Load(
    Container_ContainerProcessorStatistics_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context);

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_Unload(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context);

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_EnumerateInstances(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter);

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_GetInstance(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerProcessorStatistics* instanceName,
    const MI_PropertySet* propertySet);

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_CreateInstance(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerProcessorStatistics* newInstance);

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_ModifyInstance(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerProcessorStatistics* modifiedInstance,
    const MI_PropertySet* propertySet);

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_DeleteInstance(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerProcessorStatistics* instanceName);

MI_EXTERN_C void MI_CALL Container_ContainerProcessorStatistics_Invoke_ResetSelectedStats(
    Container_ContainerProcessorStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Container_ContainerProcessorStatistics* instanceName,
    const Container_ContainerProcessorStatistics_ResetSelectedStats* in);


/*
**==============================================================================
**
** Container_ContainerProcessorStatistics_Class
**
**==============================================================================
*/

#ifdef __cplusplus
# include <micxx/micxx.h>

MI_BEGIN_NAMESPACE

class Container_ContainerProcessorStatistics_Class : public CIM_StatisticalData_Class
{
public:
    
    typedef Container_ContainerProcessorStatistics Self;
    
    Container_ContainerProcessorStatistics_Class() :
        CIM_StatisticalData_Class(&Container_ContainerProcessorStatistics_rtti)
    {
    }
    
    Container_ContainerProcessorStatistics_Class(
        const Container_ContainerProcessorStatistics* instanceName,
        bool keysOnly) :
        CIM_StatisticalData_Class(
            &Container_ContainerProcessorStatistics_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    Container_ContainerProcessorStatistics_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        CIM_StatisticalData_Class(clDecl, instance, keysOnly)
    {
    }
    
    Container_ContainerProcessorStatistics_Class(
        const MI_ClassDecl* clDecl) :
        CIM_StatisticalData_Class(clDecl)
    {
    }
    
    Container_ContainerProcessorStatistics_Class& operator=(
        const Container_ContainerProcessorStatistics_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    Container_ContainerProcessorStatistics_Class(
        const Container_ContainerProcessorStatistics_Class& x) :
        CIM_StatisticalData_Class(x)
    {
    }

    static const MI_ClassDecl* GetClassDecl()
    {
        return &Container_ContainerProcessorStatistics_rtti;
    }

    //
    // Container_ContainerProcessorStatistics_Class.ProcessorID
    //
    
    const Field<Uint16>& ProcessorID() const
    {
        const size_t n = offsetof(Self, ProcessorID);
        return GetField<Uint16>(n);
    }
    
    void ProcessorID(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, ProcessorID);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& ProcessorID_value() const
    {
        const size_t n = offsetof(Self, ProcessorID);
        return GetField<Uint16>(n).value;
    }
    
    void ProcessorID_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, ProcessorID);
        GetField<Uint16>(n).Set(x);
    }
    
    bool ProcessorID_exists() const
    {
        const size_t n = offsetof(Self, ProcessorID);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void ProcessorID_clear()
    {
        const size_t n = offsetof(Self, ProcessorID);
        GetField<Uint16>(n).Clear();
    }

    //
    // Container_ContainerProcessorStatistics_Class.CPUTotal
    //
    
    const Field<Uint64>& CPUTotal() const
    {
        const size_t n = offsetof(Self, CPUTotal);
        return GetField<Uint64>(n);
    }
    
    void CPUTotal(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, CPUTotal);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& CPUTotal_value() const
    {
        const size_t n = offsetof(Self, CPUTotal);
        return GetField<Uint64>(n).value;
    }
    
    void CPUTotal_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, CPUTotal);
        GetField<Uint64>(n).Set(x);
    }
    
    bool CPUTotal_exists() const
    {
        const size_t n = offsetof(Self, CPUTotal);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void CPUTotal_clear()
    {
        const size_t n = offsetof(Self, CPUTotal);
        GetField<Uint64>(n).Clear();
    }

    //
    // Container_ContainerProcessorStatistics_Class.CPUTotalPct
    //
    
    const Field<Uint16>& CPUTotalPct() const
    {
        const size_t n = offsetof(Self, CPUTotalPct);
        return GetField<Uint16>(n);
    }
    
    void CPUTotalPct(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, CPUTotalPct);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& CPUTotalPct_value() const
    {
        const size_t n = offsetof(Self, CPUTotalPct);
        return GetField<Uint16>(n).value;
    }
    
    void CPUTotalPct_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, CPUTotalPct);
        GetField<Uint16>(n).Set(x);
    }
    
    bool CPUTotalPct_exists() const
    {
        const size_t n = offsetof(Self, CPUTotalPct);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void CPUTotalPct_clear()
    {
        const size_t n = offsetof(Self, CPUTotalPct);
        GetField<Uint16>(n).Clear();
    }
};

typedef Array<Container_ContainerProcessorStatistics_Class> Container_ContainerProcessorStatistics_ClassA;

class Container_ContainerProcessorStatistics_ResetSelectedStats_Class : public Instance
{
public:
    
    typedef Container_ContainerProcessorStatistics_ResetSelectedStats Self;
    
    Container_ContainerProcessorStatistics_ResetSelectedStats_Class() :
        Instance(&Container_ContainerProcessorStatistics_ResetSelectedStats_rtti)
    {
    }
    
    Container_ContainerProcessorStatistics_ResetSelectedStats_Class(
        const Container_ContainerProcessorStatistics_ResetSelectedStats* instanceName,
        bool keysOnly) :
        Instance(
            &Container_ContainerProcessorStatistics_ResetSelectedStats_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    Container_ContainerProcessorStatistics_ResetSelectedStats_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        Instance(clDecl, instance, keysOnly)
    {
    }
    
    Container_ContainerProcessorStatistics_ResetSelectedStats_Class(
        const MI_ClassDecl* clDecl) :
        Instance(clDecl)
    {
    }
    
    Container_ContainerProcessorStatistics_ResetSelectedStats_Class& operator=(
        const Container_ContainerProcessorStatistics_ResetSelectedStats_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    Container_ContainerProcessorStatistics_ResetSelectedStats_Class(
        const Container_ContainerProcessorStatistics_ResetSelectedStats_Class& x) :
        Instance(x)
    {
    }

    //
    // Container_ContainerProcessorStatistics_ResetSelectedStats_Class.MIReturn
    //
    
    const Field<Uint32>& MIReturn() const
    {
        const size_t n = offsetof(Self, MIReturn);
        return GetField<Uint32>(n);
    }
    
    void MIReturn(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, MIReturn);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& MIReturn_value() const
    {
        const size_t n = offsetof(Self, MIReturn);
        return GetField<Uint32>(n).value;
    }
    
    void MIReturn_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, MIReturn);
        GetField<Uint32>(n).Set(x);
    }
    
    bool MIReturn_exists() const
    {
        const size_t n = offsetof(Self, MIReturn);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void MIReturn_clear()
    {
        const size_t n = offsetof(Self, MIReturn);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerProcessorStatistics_ResetSelectedStats_Class.SelectedStatistics
    //
    
    const Field<StringA>& SelectedStatistics() const
    {
        const size_t n = offsetof(Self, SelectedStatistics);
        return GetField<StringA>(n);
    }
    
    void SelectedStatistics(const Field<StringA>& x)
    {
        const size_t n = offsetof(Self, SelectedStatistics);
        GetField<StringA>(n) = x;
    }
    
    const StringA& SelectedStatistics_value() const
    {
        const size_t n = offsetof(Self, SelectedStatistics);
        return GetField<StringA>(n).value;
    }
    
    void SelectedStatistics_value(const StringA& x)
    {
        const size_t n = offsetof(Self, SelectedStatistics);
        GetField<StringA>(n).Set(x);
    }
    
    bool SelectedStatistics_exists() const
    {
        const size_t n = offsetof(Self, SelectedStatistics);
        return GetField<StringA>(n).exists ? true : false;
    }
    
    void SelectedStatistics_clear()
    {
        const size_t n = offsetof(Self, SelectedStatistics);
        GetField<StringA>(n).Clear();
    }
};

typedef Array<Container_ContainerProcessorStatistics_ResetSelectedStats_Class> Container_ContainerProcessorStatistics_ResetSelectedStats_ClassA;

MI_END_NAMESPACE

#endif /* __cplusplus */

#endif /* _Container_ContainerProcessorStatistics_h */
