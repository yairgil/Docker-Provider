/* @migen@ */
/*
**==============================================================================
**
** WARNING: THIS FILE WAS AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT.
**
**==============================================================================
*/
#ifndef _Container_ContainerStatistics_h
#define _Container_ContainerStatistics_h

#include <MI.h>
#include "CIM_StatisticalData.h"

/*
**==============================================================================
**
** Container_ContainerStatistics [Container_ContainerStatistics]
**
** Keys:
**    InstanceID
**
**==============================================================================
*/

typedef struct _Container_ContainerStatistics /* extends CIM_StatisticalData */
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
    /* Container_ContainerStatistics properties */
    MI_ConstUint64Field updatetime;
    MI_ConstUint64Field NetRXBytes;
    MI_ConstUint64Field NetTXBytes;
    MI_ConstUint64Field NetBytes;
    MI_ConstUint32Field NetRXKBytesPerSec;
    MI_ConstUint32Field NetTXKBytesPerSec;
    MI_ConstUint32Field MemCacheMB;
    MI_ConstUint32Field MemRSSMB;
    MI_ConstUint16Field MemPGFault;
    MI_ConstUint16Field MemPGMajFault;
    MI_ConstUint16Field MemPGFaultPerSec;
    MI_ConstUint16Field MemPGMajFaultPerSec;
    MI_ConstUint32Field MemSwapMB;
    MI_ConstUint32Field MemUnevictableMB;
    MI_ConstUint32Field MemLimitMB;
    MI_ConstUint32Field MemSWLimitMB;
    MI_ConstUint16Field MemUsedPct;
    MI_ConstUint16Field MemSWUsedPct;
    MI_ConstUint64Field CPUTotal;
    MI_ConstUint64Field CPUSystem;
    MI_ConstUint16Field CPUTotalPct;
    MI_ConstUint16Field CPUSystemPct;
    MI_ConstUint64Field CPUHost;
}
Container_ContainerStatistics;

typedef struct _Container_ContainerStatistics_Ref
{
    Container_ContainerStatistics* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Container_ContainerStatistics_Ref;

typedef struct _Container_ContainerStatistics_ConstRef
{
    MI_CONST Container_ContainerStatistics* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Container_ContainerStatistics_ConstRef;

typedef struct _Container_ContainerStatistics_Array
{
    struct _Container_ContainerStatistics** data;
    MI_Uint32 size;
}
Container_ContainerStatistics_Array;

typedef struct _Container_ContainerStatistics_ConstArray
{
    struct _Container_ContainerStatistics MI_CONST* MI_CONST* data;
    MI_Uint32 size;
}
Container_ContainerStatistics_ConstArray;

typedef struct _Container_ContainerStatistics_ArrayRef
{
    Container_ContainerStatistics_Array value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Container_ContainerStatistics_ArrayRef;

typedef struct _Container_ContainerStatistics_ConstArrayRef
{
    Container_ContainerStatistics_ConstArray value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Container_ContainerStatistics_ConstArrayRef;

MI_EXTERN_C MI_CONST MI_ClassDecl Container_ContainerStatistics_rtti;

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Construct(
    Container_ContainerStatistics* self,
    MI_Context* context)
{
    return MI_ConstructInstance(context, &Container_ContainerStatistics_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clone(
    const Container_ContainerStatistics* self,
    Container_ContainerStatistics** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Boolean MI_CALL Container_ContainerStatistics_IsA(
    const MI_Instance* self)
{
    MI_Boolean res = MI_FALSE;
    return MI_Instance_IsA(self, &Container_ContainerStatistics_rtti, &res) == MI_RESULT_OK && res;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Destruct(Container_ContainerStatistics* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Delete(Container_ContainerStatistics* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Post(
    const Container_ContainerStatistics* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_InstanceID(
    Container_ContainerStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_SetPtr_InstanceID(
    Container_ContainerStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_InstanceID(
    Container_ContainerStatistics* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_Caption(
    Container_ContainerStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_SetPtr_Caption(
    Container_ContainerStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_Caption(
    Container_ContainerStatistics* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        1);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_Description(
    Container_ContainerStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_SetPtr_Description(
    Container_ContainerStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_Description(
    Container_ContainerStatistics* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_ElementName(
    Container_ContainerStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_SetPtr_ElementName(
    Container_ContainerStatistics* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_ElementName(
    Container_ContainerStatistics* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        3);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_StartStatisticTime(
    Container_ContainerStatistics* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->StartStatisticTime)->value = x;
    ((MI_DatetimeField*)&self->StartStatisticTime)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_StartStatisticTime(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->StartStatisticTime, 0, sizeof(self->StartStatisticTime));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_StatisticTime(
    Container_ContainerStatistics* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->StatisticTime)->value = x;
    ((MI_DatetimeField*)&self->StatisticTime)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_StatisticTime(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->StatisticTime, 0, sizeof(self->StatisticTime));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_SampleInterval(
    Container_ContainerStatistics* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->SampleInterval)->value = x;
    ((MI_DatetimeField*)&self->SampleInterval)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_SampleInterval(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->SampleInterval, 0, sizeof(self->SampleInterval));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_updatetime(
    Container_ContainerStatistics* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->updatetime)->value = x;
    ((MI_Uint64Field*)&self->updatetime)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_updatetime(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->updatetime, 0, sizeof(self->updatetime));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_NetRXBytes(
    Container_ContainerStatistics* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->NetRXBytes)->value = x;
    ((MI_Uint64Field*)&self->NetRXBytes)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_NetRXBytes(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->NetRXBytes, 0, sizeof(self->NetRXBytes));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_NetTXBytes(
    Container_ContainerStatistics* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->NetTXBytes)->value = x;
    ((MI_Uint64Field*)&self->NetTXBytes)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_NetTXBytes(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->NetTXBytes, 0, sizeof(self->NetTXBytes));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_NetBytes(
    Container_ContainerStatistics* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->NetBytes)->value = x;
    ((MI_Uint64Field*)&self->NetBytes)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_NetBytes(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->NetBytes, 0, sizeof(self->NetBytes));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_NetRXKBytesPerSec(
    Container_ContainerStatistics* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->NetRXKBytesPerSec)->value = x;
    ((MI_Uint32Field*)&self->NetRXKBytesPerSec)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_NetRXKBytesPerSec(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->NetRXKBytesPerSec, 0, sizeof(self->NetRXKBytesPerSec));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_NetTXKBytesPerSec(
    Container_ContainerStatistics* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->NetTXKBytesPerSec)->value = x;
    ((MI_Uint32Field*)&self->NetTXKBytesPerSec)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_NetTXKBytesPerSec(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->NetTXKBytesPerSec, 0, sizeof(self->NetTXKBytesPerSec));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemCacheMB(
    Container_ContainerStatistics* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MemCacheMB)->value = x;
    ((MI_Uint32Field*)&self->MemCacheMB)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemCacheMB(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemCacheMB, 0, sizeof(self->MemCacheMB));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemRSSMB(
    Container_ContainerStatistics* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MemRSSMB)->value = x;
    ((MI_Uint32Field*)&self->MemRSSMB)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemRSSMB(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemRSSMB, 0, sizeof(self->MemRSSMB));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemPGFault(
    Container_ContainerStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->MemPGFault)->value = x;
    ((MI_Uint16Field*)&self->MemPGFault)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemPGFault(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemPGFault, 0, sizeof(self->MemPGFault));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemPGMajFault(
    Container_ContainerStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->MemPGMajFault)->value = x;
    ((MI_Uint16Field*)&self->MemPGMajFault)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemPGMajFault(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemPGMajFault, 0, sizeof(self->MemPGMajFault));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemPGFaultPerSec(
    Container_ContainerStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->MemPGFaultPerSec)->value = x;
    ((MI_Uint16Field*)&self->MemPGFaultPerSec)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemPGFaultPerSec(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemPGFaultPerSec, 0, sizeof(self->MemPGFaultPerSec));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemPGMajFaultPerSec(
    Container_ContainerStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->MemPGMajFaultPerSec)->value = x;
    ((MI_Uint16Field*)&self->MemPGMajFaultPerSec)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemPGMajFaultPerSec(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemPGMajFaultPerSec, 0, sizeof(self->MemPGMajFaultPerSec));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemSwapMB(
    Container_ContainerStatistics* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MemSwapMB)->value = x;
    ((MI_Uint32Field*)&self->MemSwapMB)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemSwapMB(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemSwapMB, 0, sizeof(self->MemSwapMB));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemUnevictableMB(
    Container_ContainerStatistics* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MemUnevictableMB)->value = x;
    ((MI_Uint32Field*)&self->MemUnevictableMB)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemUnevictableMB(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemUnevictableMB, 0, sizeof(self->MemUnevictableMB));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemLimitMB(
    Container_ContainerStatistics* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MemLimitMB)->value = x;
    ((MI_Uint32Field*)&self->MemLimitMB)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemLimitMB(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemLimitMB, 0, sizeof(self->MemLimitMB));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemSWLimitMB(
    Container_ContainerStatistics* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MemSWLimitMB)->value = x;
    ((MI_Uint32Field*)&self->MemSWLimitMB)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemSWLimitMB(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemSWLimitMB, 0, sizeof(self->MemSWLimitMB));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemUsedPct(
    Container_ContainerStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->MemUsedPct)->value = x;
    ((MI_Uint16Field*)&self->MemUsedPct)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemUsedPct(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemUsedPct, 0, sizeof(self->MemUsedPct));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_MemSWUsedPct(
    Container_ContainerStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->MemSWUsedPct)->value = x;
    ((MI_Uint16Field*)&self->MemSWUsedPct)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_MemSWUsedPct(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->MemSWUsedPct, 0, sizeof(self->MemSWUsedPct));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_CPUTotal(
    Container_ContainerStatistics* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->CPUTotal)->value = x;
    ((MI_Uint64Field*)&self->CPUTotal)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_CPUTotal(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->CPUTotal, 0, sizeof(self->CPUTotal));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_CPUSystem(
    Container_ContainerStatistics* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->CPUSystem)->value = x;
    ((MI_Uint64Field*)&self->CPUSystem)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_CPUSystem(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->CPUSystem, 0, sizeof(self->CPUSystem));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_CPUTotalPct(
    Container_ContainerStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->CPUTotalPct)->value = x;
    ((MI_Uint16Field*)&self->CPUTotalPct)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_CPUTotalPct(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->CPUTotalPct, 0, sizeof(self->CPUTotalPct));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_CPUSystemPct(
    Container_ContainerStatistics* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->CPUSystemPct)->value = x;
    ((MI_Uint16Field*)&self->CPUSystemPct)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_CPUSystemPct(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->CPUSystemPct, 0, sizeof(self->CPUSystemPct));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Set_CPUHost(
    Container_ContainerStatistics* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->CPUHost)->value = x;
    ((MI_Uint64Field*)&self->CPUHost)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_Clear_CPUHost(
    Container_ContainerStatistics* self)
{
    memset((void*)&self->CPUHost, 0, sizeof(self->CPUHost));
    return MI_RESULT_OK;
}

/*
**==============================================================================
**
** Container_ContainerStatistics.ResetSelectedStats()
**
**==============================================================================
*/

typedef struct _Container_ContainerStatistics_ResetSelectedStats
{
    MI_Instance __instance;
    /*OUT*/ MI_ConstUint32Field MIReturn;
    /*IN*/ MI_ConstStringAField SelectedStatistics;
}
Container_ContainerStatistics_ResetSelectedStats;

MI_EXTERN_C MI_CONST MI_MethodDecl Container_ContainerStatistics_ResetSelectedStats_rtti;

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Construct(
    Container_ContainerStatistics_ResetSelectedStats* self,
    MI_Context* context)
{
    return MI_ConstructParameters(context, &Container_ContainerStatistics_ResetSelectedStats_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Clone(
    const Container_ContainerStatistics_ResetSelectedStats* self,
    Container_ContainerStatistics_ResetSelectedStats** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Destruct(
    Container_ContainerStatistics_ResetSelectedStats* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Delete(
    Container_ContainerStatistics_ResetSelectedStats* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Post(
    const Container_ContainerStatistics_ResetSelectedStats* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Set_MIReturn(
    Container_ContainerStatistics_ResetSelectedStats* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MIReturn)->value = x;
    ((MI_Uint32Field*)&self->MIReturn)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Clear_MIReturn(
    Container_ContainerStatistics_ResetSelectedStats* self)
{
    memset((void*)&self->MIReturn, 0, sizeof(self->MIReturn));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Set_SelectedStatistics(
    Container_ContainerStatistics_ResetSelectedStats* self,
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

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_SetPtr_SelectedStatistics(
    Container_ContainerStatistics_ResetSelectedStats* self,
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

MI_INLINE MI_Result MI_CALL Container_ContainerStatistics_ResetSelectedStats_Clear_SelectedStatistics(
    Container_ContainerStatistics_ResetSelectedStats* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        1);
}

/*
**==============================================================================
**
** Container_ContainerStatistics provider function prototypes
**
**==============================================================================
*/

/* The developer may optionally define this structure */
typedef struct _Container_ContainerStatistics_Self Container_ContainerStatistics_Self;

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_Load(
    Container_ContainerStatistics_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context);

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_Unload(
    Container_ContainerStatistics_Self* self,
    MI_Context* context);

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_EnumerateInstances(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter);

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_GetInstance(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerStatistics* instanceName,
    const MI_PropertySet* propertySet);

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_CreateInstance(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerStatistics* newInstance);

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_ModifyInstance(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerStatistics* modifiedInstance,
    const MI_PropertySet* propertySet);

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_DeleteInstance(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Container_ContainerStatistics* instanceName);

MI_EXTERN_C void MI_CALL Container_ContainerStatistics_Invoke_ResetSelectedStats(
    Container_ContainerStatistics_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Container_ContainerStatistics* instanceName,
    const Container_ContainerStatistics_ResetSelectedStats* in);


/*
**==============================================================================
**
** Container_ContainerStatistics_Class
**
**==============================================================================
*/

#ifdef __cplusplus
# include <micxx/micxx.h>

MI_BEGIN_NAMESPACE

class Container_ContainerStatistics_Class : public CIM_StatisticalData_Class
{
public:
    
    typedef Container_ContainerStatistics Self;
    
    Container_ContainerStatistics_Class() :
        CIM_StatisticalData_Class(&Container_ContainerStatistics_rtti)
    {
    }
    
    Container_ContainerStatistics_Class(
        const Container_ContainerStatistics* instanceName,
        bool keysOnly) :
        CIM_StatisticalData_Class(
            &Container_ContainerStatistics_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    Container_ContainerStatistics_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        CIM_StatisticalData_Class(clDecl, instance, keysOnly)
    {
    }
    
    Container_ContainerStatistics_Class(
        const MI_ClassDecl* clDecl) :
        CIM_StatisticalData_Class(clDecl)
    {
    }
    
    Container_ContainerStatistics_Class& operator=(
        const Container_ContainerStatistics_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    Container_ContainerStatistics_Class(
        const Container_ContainerStatistics_Class& x) :
        CIM_StatisticalData_Class(x)
    {
    }

    static const MI_ClassDecl* GetClassDecl()
    {
        return &Container_ContainerStatistics_rtti;
    }

    //
    // Container_ContainerStatistics_Class.updatetime
    //
    
    const Field<Uint64>& updatetime() const
    {
        const size_t n = offsetof(Self, updatetime);
        return GetField<Uint64>(n);
    }
    
    void updatetime(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, updatetime);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& updatetime_value() const
    {
        const size_t n = offsetof(Self, updatetime);
        return GetField<Uint64>(n).value;
    }
    
    void updatetime_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, updatetime);
        GetField<Uint64>(n).Set(x);
    }
    
    bool updatetime_exists() const
    {
        const size_t n = offsetof(Self, updatetime);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void updatetime_clear()
    {
        const size_t n = offsetof(Self, updatetime);
        GetField<Uint64>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.NetRXBytes
    //
    
    const Field<Uint64>& NetRXBytes() const
    {
        const size_t n = offsetof(Self, NetRXBytes);
        return GetField<Uint64>(n);
    }
    
    void NetRXBytes(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, NetRXBytes);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& NetRXBytes_value() const
    {
        const size_t n = offsetof(Self, NetRXBytes);
        return GetField<Uint64>(n).value;
    }
    
    void NetRXBytes_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, NetRXBytes);
        GetField<Uint64>(n).Set(x);
    }
    
    bool NetRXBytes_exists() const
    {
        const size_t n = offsetof(Self, NetRXBytes);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void NetRXBytes_clear()
    {
        const size_t n = offsetof(Self, NetRXBytes);
        GetField<Uint64>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.NetTXBytes
    //
    
    const Field<Uint64>& NetTXBytes() const
    {
        const size_t n = offsetof(Self, NetTXBytes);
        return GetField<Uint64>(n);
    }
    
    void NetTXBytes(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, NetTXBytes);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& NetTXBytes_value() const
    {
        const size_t n = offsetof(Self, NetTXBytes);
        return GetField<Uint64>(n).value;
    }
    
    void NetTXBytes_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, NetTXBytes);
        GetField<Uint64>(n).Set(x);
    }
    
    bool NetTXBytes_exists() const
    {
        const size_t n = offsetof(Self, NetTXBytes);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void NetTXBytes_clear()
    {
        const size_t n = offsetof(Self, NetTXBytes);
        GetField<Uint64>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.NetBytes
    //
    
    const Field<Uint64>& NetBytes() const
    {
        const size_t n = offsetof(Self, NetBytes);
        return GetField<Uint64>(n);
    }
    
    void NetBytes(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, NetBytes);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& NetBytes_value() const
    {
        const size_t n = offsetof(Self, NetBytes);
        return GetField<Uint64>(n).value;
    }
    
    void NetBytes_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, NetBytes);
        GetField<Uint64>(n).Set(x);
    }
    
    bool NetBytes_exists() const
    {
        const size_t n = offsetof(Self, NetBytes);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void NetBytes_clear()
    {
        const size_t n = offsetof(Self, NetBytes);
        GetField<Uint64>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.NetRXKBytesPerSec
    //
    
    const Field<Uint32>& NetRXKBytesPerSec() const
    {
        const size_t n = offsetof(Self, NetRXKBytesPerSec);
        return GetField<Uint32>(n);
    }
    
    void NetRXKBytesPerSec(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, NetRXKBytesPerSec);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& NetRXKBytesPerSec_value() const
    {
        const size_t n = offsetof(Self, NetRXKBytesPerSec);
        return GetField<Uint32>(n).value;
    }
    
    void NetRXKBytesPerSec_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, NetRXKBytesPerSec);
        GetField<Uint32>(n).Set(x);
    }
    
    bool NetRXKBytesPerSec_exists() const
    {
        const size_t n = offsetof(Self, NetRXKBytesPerSec);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void NetRXKBytesPerSec_clear()
    {
        const size_t n = offsetof(Self, NetRXKBytesPerSec);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.NetTXKBytesPerSec
    //
    
    const Field<Uint32>& NetTXKBytesPerSec() const
    {
        const size_t n = offsetof(Self, NetTXKBytesPerSec);
        return GetField<Uint32>(n);
    }
    
    void NetTXKBytesPerSec(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, NetTXKBytesPerSec);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& NetTXKBytesPerSec_value() const
    {
        const size_t n = offsetof(Self, NetTXKBytesPerSec);
        return GetField<Uint32>(n).value;
    }
    
    void NetTXKBytesPerSec_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, NetTXKBytesPerSec);
        GetField<Uint32>(n).Set(x);
    }
    
    bool NetTXKBytesPerSec_exists() const
    {
        const size_t n = offsetof(Self, NetTXKBytesPerSec);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void NetTXKBytesPerSec_clear()
    {
        const size_t n = offsetof(Self, NetTXKBytesPerSec);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemCacheMB
    //
    
    const Field<Uint32>& MemCacheMB() const
    {
        const size_t n = offsetof(Self, MemCacheMB);
        return GetField<Uint32>(n);
    }
    
    void MemCacheMB(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, MemCacheMB);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& MemCacheMB_value() const
    {
        const size_t n = offsetof(Self, MemCacheMB);
        return GetField<Uint32>(n).value;
    }
    
    void MemCacheMB_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, MemCacheMB);
        GetField<Uint32>(n).Set(x);
    }
    
    bool MemCacheMB_exists() const
    {
        const size_t n = offsetof(Self, MemCacheMB);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void MemCacheMB_clear()
    {
        const size_t n = offsetof(Self, MemCacheMB);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemRSSMB
    //
    
    const Field<Uint32>& MemRSSMB() const
    {
        const size_t n = offsetof(Self, MemRSSMB);
        return GetField<Uint32>(n);
    }
    
    void MemRSSMB(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, MemRSSMB);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& MemRSSMB_value() const
    {
        const size_t n = offsetof(Self, MemRSSMB);
        return GetField<Uint32>(n).value;
    }
    
    void MemRSSMB_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, MemRSSMB);
        GetField<Uint32>(n).Set(x);
    }
    
    bool MemRSSMB_exists() const
    {
        const size_t n = offsetof(Self, MemRSSMB);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void MemRSSMB_clear()
    {
        const size_t n = offsetof(Self, MemRSSMB);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemPGFault
    //
    
    const Field<Uint16>& MemPGFault() const
    {
        const size_t n = offsetof(Self, MemPGFault);
        return GetField<Uint16>(n);
    }
    
    void MemPGFault(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, MemPGFault);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& MemPGFault_value() const
    {
        const size_t n = offsetof(Self, MemPGFault);
        return GetField<Uint16>(n).value;
    }
    
    void MemPGFault_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, MemPGFault);
        GetField<Uint16>(n).Set(x);
    }
    
    bool MemPGFault_exists() const
    {
        const size_t n = offsetof(Self, MemPGFault);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void MemPGFault_clear()
    {
        const size_t n = offsetof(Self, MemPGFault);
        GetField<Uint16>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemPGMajFault
    //
    
    const Field<Uint16>& MemPGMajFault() const
    {
        const size_t n = offsetof(Self, MemPGMajFault);
        return GetField<Uint16>(n);
    }
    
    void MemPGMajFault(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, MemPGMajFault);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& MemPGMajFault_value() const
    {
        const size_t n = offsetof(Self, MemPGMajFault);
        return GetField<Uint16>(n).value;
    }
    
    void MemPGMajFault_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, MemPGMajFault);
        GetField<Uint16>(n).Set(x);
    }
    
    bool MemPGMajFault_exists() const
    {
        const size_t n = offsetof(Self, MemPGMajFault);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void MemPGMajFault_clear()
    {
        const size_t n = offsetof(Self, MemPGMajFault);
        GetField<Uint16>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemPGFaultPerSec
    //
    
    const Field<Uint16>& MemPGFaultPerSec() const
    {
        const size_t n = offsetof(Self, MemPGFaultPerSec);
        return GetField<Uint16>(n);
    }
    
    void MemPGFaultPerSec(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, MemPGFaultPerSec);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& MemPGFaultPerSec_value() const
    {
        const size_t n = offsetof(Self, MemPGFaultPerSec);
        return GetField<Uint16>(n).value;
    }
    
    void MemPGFaultPerSec_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, MemPGFaultPerSec);
        GetField<Uint16>(n).Set(x);
    }
    
    bool MemPGFaultPerSec_exists() const
    {
        const size_t n = offsetof(Self, MemPGFaultPerSec);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void MemPGFaultPerSec_clear()
    {
        const size_t n = offsetof(Self, MemPGFaultPerSec);
        GetField<Uint16>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemPGMajFaultPerSec
    //
    
    const Field<Uint16>& MemPGMajFaultPerSec() const
    {
        const size_t n = offsetof(Self, MemPGMajFaultPerSec);
        return GetField<Uint16>(n);
    }
    
    void MemPGMajFaultPerSec(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, MemPGMajFaultPerSec);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& MemPGMajFaultPerSec_value() const
    {
        const size_t n = offsetof(Self, MemPGMajFaultPerSec);
        return GetField<Uint16>(n).value;
    }
    
    void MemPGMajFaultPerSec_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, MemPGMajFaultPerSec);
        GetField<Uint16>(n).Set(x);
    }
    
    bool MemPGMajFaultPerSec_exists() const
    {
        const size_t n = offsetof(Self, MemPGMajFaultPerSec);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void MemPGMajFaultPerSec_clear()
    {
        const size_t n = offsetof(Self, MemPGMajFaultPerSec);
        GetField<Uint16>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemSwapMB
    //
    
    const Field<Uint32>& MemSwapMB() const
    {
        const size_t n = offsetof(Self, MemSwapMB);
        return GetField<Uint32>(n);
    }
    
    void MemSwapMB(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, MemSwapMB);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& MemSwapMB_value() const
    {
        const size_t n = offsetof(Self, MemSwapMB);
        return GetField<Uint32>(n).value;
    }
    
    void MemSwapMB_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, MemSwapMB);
        GetField<Uint32>(n).Set(x);
    }
    
    bool MemSwapMB_exists() const
    {
        const size_t n = offsetof(Self, MemSwapMB);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void MemSwapMB_clear()
    {
        const size_t n = offsetof(Self, MemSwapMB);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemUnevictableMB
    //
    
    const Field<Uint32>& MemUnevictableMB() const
    {
        const size_t n = offsetof(Self, MemUnevictableMB);
        return GetField<Uint32>(n);
    }
    
    void MemUnevictableMB(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, MemUnevictableMB);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& MemUnevictableMB_value() const
    {
        const size_t n = offsetof(Self, MemUnevictableMB);
        return GetField<Uint32>(n).value;
    }
    
    void MemUnevictableMB_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, MemUnevictableMB);
        GetField<Uint32>(n).Set(x);
    }
    
    bool MemUnevictableMB_exists() const
    {
        const size_t n = offsetof(Self, MemUnevictableMB);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void MemUnevictableMB_clear()
    {
        const size_t n = offsetof(Self, MemUnevictableMB);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemLimitMB
    //
    
    const Field<Uint32>& MemLimitMB() const
    {
        const size_t n = offsetof(Self, MemLimitMB);
        return GetField<Uint32>(n);
    }
    
    void MemLimitMB(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, MemLimitMB);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& MemLimitMB_value() const
    {
        const size_t n = offsetof(Self, MemLimitMB);
        return GetField<Uint32>(n).value;
    }
    
    void MemLimitMB_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, MemLimitMB);
        GetField<Uint32>(n).Set(x);
    }
    
    bool MemLimitMB_exists() const
    {
        const size_t n = offsetof(Self, MemLimitMB);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void MemLimitMB_clear()
    {
        const size_t n = offsetof(Self, MemLimitMB);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemSWLimitMB
    //
    
    const Field<Uint32>& MemSWLimitMB() const
    {
        const size_t n = offsetof(Self, MemSWLimitMB);
        return GetField<Uint32>(n);
    }
    
    void MemSWLimitMB(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, MemSWLimitMB);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& MemSWLimitMB_value() const
    {
        const size_t n = offsetof(Self, MemSWLimitMB);
        return GetField<Uint32>(n).value;
    }
    
    void MemSWLimitMB_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, MemSWLimitMB);
        GetField<Uint32>(n).Set(x);
    }
    
    bool MemSWLimitMB_exists() const
    {
        const size_t n = offsetof(Self, MemSWLimitMB);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void MemSWLimitMB_clear()
    {
        const size_t n = offsetof(Self, MemSWLimitMB);
        GetField<Uint32>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemUsedPct
    //
    
    const Field<Uint16>& MemUsedPct() const
    {
        const size_t n = offsetof(Self, MemUsedPct);
        return GetField<Uint16>(n);
    }
    
    void MemUsedPct(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, MemUsedPct);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& MemUsedPct_value() const
    {
        const size_t n = offsetof(Self, MemUsedPct);
        return GetField<Uint16>(n).value;
    }
    
    void MemUsedPct_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, MemUsedPct);
        GetField<Uint16>(n).Set(x);
    }
    
    bool MemUsedPct_exists() const
    {
        const size_t n = offsetof(Self, MemUsedPct);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void MemUsedPct_clear()
    {
        const size_t n = offsetof(Self, MemUsedPct);
        GetField<Uint16>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.MemSWUsedPct
    //
    
    const Field<Uint16>& MemSWUsedPct() const
    {
        const size_t n = offsetof(Self, MemSWUsedPct);
        return GetField<Uint16>(n);
    }
    
    void MemSWUsedPct(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, MemSWUsedPct);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& MemSWUsedPct_value() const
    {
        const size_t n = offsetof(Self, MemSWUsedPct);
        return GetField<Uint16>(n).value;
    }
    
    void MemSWUsedPct_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, MemSWUsedPct);
        GetField<Uint16>(n).Set(x);
    }
    
    bool MemSWUsedPct_exists() const
    {
        const size_t n = offsetof(Self, MemSWUsedPct);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void MemSWUsedPct_clear()
    {
        const size_t n = offsetof(Self, MemSWUsedPct);
        GetField<Uint16>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.CPUTotal
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
    // Container_ContainerStatistics_Class.CPUSystem
    //
    
    const Field<Uint64>& CPUSystem() const
    {
        const size_t n = offsetof(Self, CPUSystem);
        return GetField<Uint64>(n);
    }
    
    void CPUSystem(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, CPUSystem);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& CPUSystem_value() const
    {
        const size_t n = offsetof(Self, CPUSystem);
        return GetField<Uint64>(n).value;
    }
    
    void CPUSystem_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, CPUSystem);
        GetField<Uint64>(n).Set(x);
    }
    
    bool CPUSystem_exists() const
    {
        const size_t n = offsetof(Self, CPUSystem);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void CPUSystem_clear()
    {
        const size_t n = offsetof(Self, CPUSystem);
        GetField<Uint64>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.CPUTotalPct
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

    //
    // Container_ContainerStatistics_Class.CPUSystemPct
    //
    
    const Field<Uint16>& CPUSystemPct() const
    {
        const size_t n = offsetof(Self, CPUSystemPct);
        return GetField<Uint16>(n);
    }
    
    void CPUSystemPct(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, CPUSystemPct);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& CPUSystemPct_value() const
    {
        const size_t n = offsetof(Self, CPUSystemPct);
        return GetField<Uint16>(n).value;
    }
    
    void CPUSystemPct_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, CPUSystemPct);
        GetField<Uint16>(n).Set(x);
    }
    
    bool CPUSystemPct_exists() const
    {
        const size_t n = offsetof(Self, CPUSystemPct);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void CPUSystemPct_clear()
    {
        const size_t n = offsetof(Self, CPUSystemPct);
        GetField<Uint16>(n).Clear();
    }

    //
    // Container_ContainerStatistics_Class.CPUHost
    //
    
    const Field<Uint64>& CPUHost() const
    {
        const size_t n = offsetof(Self, CPUHost);
        return GetField<Uint64>(n);
    }
    
    void CPUHost(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, CPUHost);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& CPUHost_value() const
    {
        const size_t n = offsetof(Self, CPUHost);
        return GetField<Uint64>(n).value;
    }
    
    void CPUHost_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, CPUHost);
        GetField<Uint64>(n).Set(x);
    }
    
    bool CPUHost_exists() const
    {
        const size_t n = offsetof(Self, CPUHost);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void CPUHost_clear()
    {
        const size_t n = offsetof(Self, CPUHost);
        GetField<Uint64>(n).Clear();
    }
};

typedef Array<Container_ContainerStatistics_Class> Container_ContainerStatistics_ClassA;

class Container_ContainerStatistics_ResetSelectedStats_Class : public Instance
{
public:
    
    typedef Container_ContainerStatistics_ResetSelectedStats Self;
    
    Container_ContainerStatistics_ResetSelectedStats_Class() :
        Instance(&Container_ContainerStatistics_ResetSelectedStats_rtti)
    {
    }
    
    Container_ContainerStatistics_ResetSelectedStats_Class(
        const Container_ContainerStatistics_ResetSelectedStats* instanceName,
        bool keysOnly) :
        Instance(
            &Container_ContainerStatistics_ResetSelectedStats_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    Container_ContainerStatistics_ResetSelectedStats_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        Instance(clDecl, instance, keysOnly)
    {
    }
    
    Container_ContainerStatistics_ResetSelectedStats_Class(
        const MI_ClassDecl* clDecl) :
        Instance(clDecl)
    {
    }
    
    Container_ContainerStatistics_ResetSelectedStats_Class& operator=(
        const Container_ContainerStatistics_ResetSelectedStats_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    Container_ContainerStatistics_ResetSelectedStats_Class(
        const Container_ContainerStatistics_ResetSelectedStats_Class& x) :
        Instance(x)
    {
    }

    //
    // Container_ContainerStatistics_ResetSelectedStats_Class.MIReturn
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
    // Container_ContainerStatistics_ResetSelectedStats_Class.SelectedStatistics
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

typedef Array<Container_ContainerStatistics_ResetSelectedStats_Class> Container_ContainerStatistics_ResetSelectedStats_ClassA;

MI_END_NAMESPACE

#endif /* __cplusplus */

#endif /* _Container_ContainerStatistics_h */
