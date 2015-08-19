/* @migen@ */
/*
**==============================================================================
**
** WARNING: THIS FILE WAS AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT.
**
**==============================================================================
*/
#ifndef _CIM_System_h
#define _CIM_System_h

#include <MI.h>
#include "CIM_EnabledLogicalElement.h"
#include "CIM_ConcreteJob.h"

/*
**==============================================================================
**
** CIM_System [CIM_System]
**
** Keys:
**    Name
**    CreationClassName
**
**==============================================================================
*/

typedef struct _CIM_System /* extends CIM_EnabledLogicalElement */
{
    MI_Instance __instance;
    /* CIM_ManagedElement properties */
    MI_ConstStringField InstanceID;
    MI_ConstStringField Caption;
    MI_ConstStringField Description;
    MI_ConstStringField ElementName;
    /* CIM_ManagedSystemElement properties */
    MI_ConstDatetimeField InstallDate;
    /*KEY*/ MI_ConstStringField Name;
    MI_ConstUint16AField OperationalStatus;
    MI_ConstStringAField StatusDescriptions;
    MI_ConstStringField Status;
    MI_ConstUint16Field HealthState;
    MI_ConstUint16Field CommunicationStatus;
    MI_ConstUint16Field DetailedStatus;
    MI_ConstUint16Field OperatingStatus;
    MI_ConstUint16Field PrimaryStatus;
    /* CIM_LogicalElement properties */
    /* CIM_EnabledLogicalElement properties */
    MI_ConstUint16Field EnabledState;
    MI_ConstStringField OtherEnabledState;
    MI_ConstUint16Field RequestedState;
    MI_ConstUint16Field EnabledDefault;
    MI_ConstDatetimeField TimeOfLastStateChange;
    MI_ConstUint16AField AvailableRequestedStates;
    MI_ConstUint16Field TransitioningToState;
    /* CIM_System properties */
    /*KEY*/ MI_ConstStringField CreationClassName;
    MI_ConstStringField NameFormat;
    MI_ConstStringField PrimaryOwnerName;
    MI_ConstStringField PrimaryOwnerContact;
    MI_ConstStringAField Roles;
    MI_ConstStringAField OtherIdentifyingInfo;
    MI_ConstStringAField IdentifyingDescriptions;
}
CIM_System;

typedef struct _CIM_System_Ref
{
    CIM_System* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
CIM_System_Ref;

typedef struct _CIM_System_ConstRef
{
    MI_CONST CIM_System* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
CIM_System_ConstRef;

typedef struct _CIM_System_Array
{
    struct _CIM_System** data;
    MI_Uint32 size;
}
CIM_System_Array;

typedef struct _CIM_System_ConstArray
{
    struct _CIM_System MI_CONST* MI_CONST* data;
    MI_Uint32 size;
}
CIM_System_ConstArray;

typedef struct _CIM_System_ArrayRef
{
    CIM_System_Array value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
CIM_System_ArrayRef;

typedef struct _CIM_System_ConstArrayRef
{
    CIM_System_ConstArray value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
CIM_System_ConstArrayRef;

MI_EXTERN_C MI_CONST MI_ClassDecl CIM_System_rtti;

MI_INLINE MI_Result MI_CALL CIM_System_Construct(
    CIM_System* self,
    MI_Context* context)
{
    return MI_ConstructInstance(context, &CIM_System_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clone(
    const CIM_System* self,
    CIM_System** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Boolean MI_CALL CIM_System_IsA(
    const MI_Instance* self)
{
    MI_Boolean res = MI_FALSE;
    return MI_Instance_IsA(self, &CIM_System_rtti, &res) == MI_RESULT_OK && res;
}

MI_INLINE MI_Result MI_CALL CIM_System_Destruct(CIM_System* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_System_Delete(CIM_System* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_System_Post(
    const CIM_System* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_InstanceID(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_InstanceID(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_InstanceID(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_Caption(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_Caption(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_Caption(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        1);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_Description(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_Description(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_Description(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_ElementName(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_ElementName(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_ElementName(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        3);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_InstallDate(
    CIM_System* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->InstallDate)->value = x;
    ((MI_DatetimeField*)&self->InstallDate)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_InstallDate(
    CIM_System* self)
{
    memset((void*)&self->InstallDate, 0, sizeof(self->InstallDate));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_Name(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        5,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_Name(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        5,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_Name(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        5);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_OperationalStatus(
    CIM_System* self,
    const MI_Uint16* data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        6,
        (MI_Value*)&arr,
        MI_UINT16A,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_OperationalStatus(
    CIM_System* self,
    const MI_Uint16* data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        6,
        (MI_Value*)&arr,
        MI_UINT16A,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_OperationalStatus(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        6);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_StatusDescriptions(
    CIM_System* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        7,
        (MI_Value*)&arr,
        MI_STRINGA,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_StatusDescriptions(
    CIM_System* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        7,
        (MI_Value*)&arr,
        MI_STRINGA,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_StatusDescriptions(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        7);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_Status(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        8,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_Status(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        8,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_Status(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        8);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_HealthState(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->HealthState)->value = x;
    ((MI_Uint16Field*)&self->HealthState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_HealthState(
    CIM_System* self)
{
    memset((void*)&self->HealthState, 0, sizeof(self->HealthState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_CommunicationStatus(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->CommunicationStatus)->value = x;
    ((MI_Uint16Field*)&self->CommunicationStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_CommunicationStatus(
    CIM_System* self)
{
    memset((void*)&self->CommunicationStatus, 0, sizeof(self->CommunicationStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_DetailedStatus(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->DetailedStatus)->value = x;
    ((MI_Uint16Field*)&self->DetailedStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_DetailedStatus(
    CIM_System* self)
{
    memset((void*)&self->DetailedStatus, 0, sizeof(self->DetailedStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_OperatingStatus(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->OperatingStatus)->value = x;
    ((MI_Uint16Field*)&self->OperatingStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_OperatingStatus(
    CIM_System* self)
{
    memset((void*)&self->OperatingStatus, 0, sizeof(self->OperatingStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_PrimaryStatus(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->PrimaryStatus)->value = x;
    ((MI_Uint16Field*)&self->PrimaryStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_PrimaryStatus(
    CIM_System* self)
{
    memset((void*)&self->PrimaryStatus, 0, sizeof(self->PrimaryStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_EnabledState(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->EnabledState)->value = x;
    ((MI_Uint16Field*)&self->EnabledState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_EnabledState(
    CIM_System* self)
{
    memset((void*)&self->EnabledState, 0, sizeof(self->EnabledState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_OtherEnabledState(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        15,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_OtherEnabledState(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        15,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_OtherEnabledState(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        15);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_RequestedState(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->RequestedState)->value = x;
    ((MI_Uint16Field*)&self->RequestedState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_RequestedState(
    CIM_System* self)
{
    memset((void*)&self->RequestedState, 0, sizeof(self->RequestedState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_EnabledDefault(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->EnabledDefault)->value = x;
    ((MI_Uint16Field*)&self->EnabledDefault)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_EnabledDefault(
    CIM_System* self)
{
    memset((void*)&self->EnabledDefault, 0, sizeof(self->EnabledDefault));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_TimeOfLastStateChange(
    CIM_System* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->TimeOfLastStateChange)->value = x;
    ((MI_DatetimeField*)&self->TimeOfLastStateChange)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_TimeOfLastStateChange(
    CIM_System* self)
{
    memset((void*)&self->TimeOfLastStateChange, 0, sizeof(self->TimeOfLastStateChange));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_AvailableRequestedStates(
    CIM_System* self,
    const MI_Uint16* data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        19,
        (MI_Value*)&arr,
        MI_UINT16A,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_AvailableRequestedStates(
    CIM_System* self,
    const MI_Uint16* data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        19,
        (MI_Value*)&arr,
        MI_UINT16A,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_AvailableRequestedStates(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        19);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_TransitioningToState(
    CIM_System* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->TransitioningToState)->value = x;
    ((MI_Uint16Field*)&self->TransitioningToState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_TransitioningToState(
    CIM_System* self)
{
    memset((void*)&self->TransitioningToState, 0, sizeof(self->TransitioningToState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_CreationClassName(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        21,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_CreationClassName(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        21,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_CreationClassName(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        21);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_NameFormat(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        22,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_NameFormat(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        22,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_NameFormat(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        22);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_PrimaryOwnerName(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        23,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_PrimaryOwnerName(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        23,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_PrimaryOwnerName(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        23);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_PrimaryOwnerContact(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        24,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_PrimaryOwnerContact(
    CIM_System* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        24,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_PrimaryOwnerContact(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        24);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_Roles(
    CIM_System* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        25,
        (MI_Value*)&arr,
        MI_STRINGA,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_Roles(
    CIM_System* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        25,
        (MI_Value*)&arr,
        MI_STRINGA,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_Roles(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        25);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_OtherIdentifyingInfo(
    CIM_System* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        26,
        (MI_Value*)&arr,
        MI_STRINGA,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_OtherIdentifyingInfo(
    CIM_System* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        26,
        (MI_Value*)&arr,
        MI_STRINGA,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_OtherIdentifyingInfo(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        26);
}

MI_INLINE MI_Result MI_CALL CIM_System_Set_IdentifyingDescriptions(
    CIM_System* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        27,
        (MI_Value*)&arr,
        MI_STRINGA,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_SetPtr_IdentifyingDescriptions(
    CIM_System* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        27,
        (MI_Value*)&arr,
        MI_STRINGA,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_Clear_IdentifyingDescriptions(
    CIM_System* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        27);
}

/*
**==============================================================================
**
** CIM_System.RequestStateChange()
**
**==============================================================================
*/

typedef struct _CIM_System_RequestStateChange
{
    MI_Instance __instance;
    /*OUT*/ MI_ConstUint32Field MIReturn;
    /*IN*/ MI_ConstUint16Field RequestedState;
    /*OUT*/ CIM_ConcreteJob_ConstRef Job;
    /*IN*/ MI_ConstDatetimeField TimeoutPeriod;
}
CIM_System_RequestStateChange;

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_Set_MIReturn(
    CIM_System_RequestStateChange* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MIReturn)->value = x;
    ((MI_Uint32Field*)&self->MIReturn)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_Clear_MIReturn(
    CIM_System_RequestStateChange* self)
{
    memset((void*)&self->MIReturn, 0, sizeof(self->MIReturn));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_Set_RequestedState(
    CIM_System_RequestStateChange* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->RequestedState)->value = x;
    ((MI_Uint16Field*)&self->RequestedState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_Clear_RequestedState(
    CIM_System_RequestStateChange* self)
{
    memset((void*)&self->RequestedState, 0, sizeof(self->RequestedState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_Set_Job(
    CIM_System_RequestStateChange* self,
    const CIM_ConcreteJob* x)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&x,
        MI_REFERENCE,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_SetPtr_Job(
    CIM_System_RequestStateChange* self,
    const CIM_ConcreteJob* x)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&x,
        MI_REFERENCE,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_Clear_Job(
    CIM_System_RequestStateChange* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_Set_TimeoutPeriod(
    CIM_System_RequestStateChange* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->TimeoutPeriod)->value = x;
    ((MI_DatetimeField*)&self->TimeoutPeriod)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_System_RequestStateChange_Clear_TimeoutPeriod(
    CIM_System_RequestStateChange* self)
{
    memset((void*)&self->TimeoutPeriod, 0, sizeof(self->TimeoutPeriod));
    return MI_RESULT_OK;
}


/*
**==============================================================================
**
** CIM_System_Class
**
**==============================================================================
*/

#ifdef __cplusplus
# include <micxx/micxx.h>

MI_BEGIN_NAMESPACE

class CIM_System_Class : public CIM_EnabledLogicalElement_Class
{
public:
    
    typedef CIM_System Self;
    
    CIM_System_Class() :
        CIM_EnabledLogicalElement_Class(&CIM_System_rtti)
    {
    }
    
    CIM_System_Class(
        const CIM_System* instanceName,
        bool keysOnly) :
        CIM_EnabledLogicalElement_Class(
            &CIM_System_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    CIM_System_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        CIM_EnabledLogicalElement_Class(clDecl, instance, keysOnly)
    {
    }
    
    CIM_System_Class(
        const MI_ClassDecl* clDecl) :
        CIM_EnabledLogicalElement_Class(clDecl)
    {
    }
    
    CIM_System_Class& operator=(
        const CIM_System_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    CIM_System_Class(
        const CIM_System_Class& x) :
        CIM_EnabledLogicalElement_Class(x)
    {
    }

    static const MI_ClassDecl* GetClassDecl()
    {
        return &CIM_System_rtti;
    }

    //
    // CIM_System_Class.CreationClassName
    //
    
    const Field<String>& CreationClassName() const
    {
        const size_t n = offsetof(Self, CreationClassName);
        return GetField<String>(n);
    }
    
    void CreationClassName(const Field<String>& x)
    {
        const size_t n = offsetof(Self, CreationClassName);
        GetField<String>(n) = x;
    }
    
    const String& CreationClassName_value() const
    {
        const size_t n = offsetof(Self, CreationClassName);
        return GetField<String>(n).value;
    }
    
    void CreationClassName_value(const String& x)
    {
        const size_t n = offsetof(Self, CreationClassName);
        GetField<String>(n).Set(x);
    }
    
    bool CreationClassName_exists() const
    {
        const size_t n = offsetof(Self, CreationClassName);
        return GetField<String>(n).exists ? true : false;
    }
    
    void CreationClassName_clear()
    {
        const size_t n = offsetof(Self, CreationClassName);
        GetField<String>(n).Clear();
    }

    //
    // CIM_System_Class.NameFormat
    //
    
    const Field<String>& NameFormat() const
    {
        const size_t n = offsetof(Self, NameFormat);
        return GetField<String>(n);
    }
    
    void NameFormat(const Field<String>& x)
    {
        const size_t n = offsetof(Self, NameFormat);
        GetField<String>(n) = x;
    }
    
    const String& NameFormat_value() const
    {
        const size_t n = offsetof(Self, NameFormat);
        return GetField<String>(n).value;
    }
    
    void NameFormat_value(const String& x)
    {
        const size_t n = offsetof(Self, NameFormat);
        GetField<String>(n).Set(x);
    }
    
    bool NameFormat_exists() const
    {
        const size_t n = offsetof(Self, NameFormat);
        return GetField<String>(n).exists ? true : false;
    }
    
    void NameFormat_clear()
    {
        const size_t n = offsetof(Self, NameFormat);
        GetField<String>(n).Clear();
    }

    //
    // CIM_System_Class.PrimaryOwnerName
    //
    
    const Field<String>& PrimaryOwnerName() const
    {
        const size_t n = offsetof(Self, PrimaryOwnerName);
        return GetField<String>(n);
    }
    
    void PrimaryOwnerName(const Field<String>& x)
    {
        const size_t n = offsetof(Self, PrimaryOwnerName);
        GetField<String>(n) = x;
    }
    
    const String& PrimaryOwnerName_value() const
    {
        const size_t n = offsetof(Self, PrimaryOwnerName);
        return GetField<String>(n).value;
    }
    
    void PrimaryOwnerName_value(const String& x)
    {
        const size_t n = offsetof(Self, PrimaryOwnerName);
        GetField<String>(n).Set(x);
    }
    
    bool PrimaryOwnerName_exists() const
    {
        const size_t n = offsetof(Self, PrimaryOwnerName);
        return GetField<String>(n).exists ? true : false;
    }
    
    void PrimaryOwnerName_clear()
    {
        const size_t n = offsetof(Self, PrimaryOwnerName);
        GetField<String>(n).Clear();
    }

    //
    // CIM_System_Class.PrimaryOwnerContact
    //
    
    const Field<String>& PrimaryOwnerContact() const
    {
        const size_t n = offsetof(Self, PrimaryOwnerContact);
        return GetField<String>(n);
    }
    
    void PrimaryOwnerContact(const Field<String>& x)
    {
        const size_t n = offsetof(Self, PrimaryOwnerContact);
        GetField<String>(n) = x;
    }
    
    const String& PrimaryOwnerContact_value() const
    {
        const size_t n = offsetof(Self, PrimaryOwnerContact);
        return GetField<String>(n).value;
    }
    
    void PrimaryOwnerContact_value(const String& x)
    {
        const size_t n = offsetof(Self, PrimaryOwnerContact);
        GetField<String>(n).Set(x);
    }
    
    bool PrimaryOwnerContact_exists() const
    {
        const size_t n = offsetof(Self, PrimaryOwnerContact);
        return GetField<String>(n).exists ? true : false;
    }
    
    void PrimaryOwnerContact_clear()
    {
        const size_t n = offsetof(Self, PrimaryOwnerContact);
        GetField<String>(n).Clear();
    }

    //
    // CIM_System_Class.Roles
    //
    
    const Field<StringA>& Roles() const
    {
        const size_t n = offsetof(Self, Roles);
        return GetField<StringA>(n);
    }
    
    void Roles(const Field<StringA>& x)
    {
        const size_t n = offsetof(Self, Roles);
        GetField<StringA>(n) = x;
    }
    
    const StringA& Roles_value() const
    {
        const size_t n = offsetof(Self, Roles);
        return GetField<StringA>(n).value;
    }
    
    void Roles_value(const StringA& x)
    {
        const size_t n = offsetof(Self, Roles);
        GetField<StringA>(n).Set(x);
    }
    
    bool Roles_exists() const
    {
        const size_t n = offsetof(Self, Roles);
        return GetField<StringA>(n).exists ? true : false;
    }
    
    void Roles_clear()
    {
        const size_t n = offsetof(Self, Roles);
        GetField<StringA>(n).Clear();
    }

    //
    // CIM_System_Class.OtherIdentifyingInfo
    //
    
    const Field<StringA>& OtherIdentifyingInfo() const
    {
        const size_t n = offsetof(Self, OtherIdentifyingInfo);
        return GetField<StringA>(n);
    }
    
    void OtherIdentifyingInfo(const Field<StringA>& x)
    {
        const size_t n = offsetof(Self, OtherIdentifyingInfo);
        GetField<StringA>(n) = x;
    }
    
    const StringA& OtherIdentifyingInfo_value() const
    {
        const size_t n = offsetof(Self, OtherIdentifyingInfo);
        return GetField<StringA>(n).value;
    }
    
    void OtherIdentifyingInfo_value(const StringA& x)
    {
        const size_t n = offsetof(Self, OtherIdentifyingInfo);
        GetField<StringA>(n).Set(x);
    }
    
    bool OtherIdentifyingInfo_exists() const
    {
        const size_t n = offsetof(Self, OtherIdentifyingInfo);
        return GetField<StringA>(n).exists ? true : false;
    }
    
    void OtherIdentifyingInfo_clear()
    {
        const size_t n = offsetof(Self, OtherIdentifyingInfo);
        GetField<StringA>(n).Clear();
    }

    //
    // CIM_System_Class.IdentifyingDescriptions
    //
    
    const Field<StringA>& IdentifyingDescriptions() const
    {
        const size_t n = offsetof(Self, IdentifyingDescriptions);
        return GetField<StringA>(n);
    }
    
    void IdentifyingDescriptions(const Field<StringA>& x)
    {
        const size_t n = offsetof(Self, IdentifyingDescriptions);
        GetField<StringA>(n) = x;
    }
    
    const StringA& IdentifyingDescriptions_value() const
    {
        const size_t n = offsetof(Self, IdentifyingDescriptions);
        return GetField<StringA>(n).value;
    }
    
    void IdentifyingDescriptions_value(const StringA& x)
    {
        const size_t n = offsetof(Self, IdentifyingDescriptions);
        GetField<StringA>(n).Set(x);
    }
    
    bool IdentifyingDescriptions_exists() const
    {
        const size_t n = offsetof(Self, IdentifyingDescriptions);
        return GetField<StringA>(n).exists ? true : false;
    }
    
    void IdentifyingDescriptions_clear()
    {
        const size_t n = offsetof(Self, IdentifyingDescriptions);
        GetField<StringA>(n).Clear();
    }
};

typedef Array<CIM_System_Class> CIM_System_ClassA;

MI_END_NAMESPACE

#endif /* __cplusplus */

#endif /* _CIM_System_h */
