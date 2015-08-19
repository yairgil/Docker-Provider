/* @migen@ */
/*
**==============================================================================
**
** WARNING: THIS FILE WAS AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT.
**
**==============================================================================
*/
#ifndef _Docker_Container_h
#define _Docker_Container_h

#include <MI.h>
#include "CIM_VirtualComputerSystem.h"
#include "CIM_ConcreteJob.h"

/*
**==============================================================================
**
** Docker_Container [Docker_Container]
**
** Keys:
**    Name
**    CreationClassName
**
**==============================================================================
*/

typedef struct _Docker_Container /* extends CIM_VirtualComputerSystem */
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
    /* CIM_ComputerSystem properties */
    MI_ConstUint16AField Dedicated;
    MI_ConstStringAField OtherDedicatedDescriptions;
    MI_ConstUint16Field ResetCapability;
    MI_ConstUint16AField PowerManagementCapabilities;
    /* CIM_VirtualComputerSystem properties */
    MI_ConstStringField VirtualSystem;
    /* Docker_Container properties */
    MI_ConstStringField Ports;
    MI_ConstStringField Command;
    MI_ConstStringField Image;
    MI_ConstStringField SizeRW;
}
Docker_Container;

typedef struct _Docker_Container_Ref
{
    Docker_Container* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Docker_Container_Ref;

typedef struct _Docker_Container_ConstRef
{
    MI_CONST Docker_Container* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Docker_Container_ConstRef;

typedef struct _Docker_Container_Array
{
    struct _Docker_Container** data;
    MI_Uint32 size;
}
Docker_Container_Array;

typedef struct _Docker_Container_ConstArray
{
    struct _Docker_Container MI_CONST* MI_CONST* data;
    MI_Uint32 size;
}
Docker_Container_ConstArray;

typedef struct _Docker_Container_ArrayRef
{
    Docker_Container_Array value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Docker_Container_ArrayRef;

typedef struct _Docker_Container_ConstArrayRef
{
    Docker_Container_ConstArray value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Docker_Container_ConstArrayRef;

MI_EXTERN_C MI_CONST MI_ClassDecl Docker_Container_rtti;

MI_INLINE MI_Result MI_CALL Docker_Container_Construct(
    Docker_Container* self,
    MI_Context* context)
{
    return MI_ConstructInstance(context, &Docker_Container_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clone(
    const Docker_Container* self,
    Docker_Container** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Boolean MI_CALL Docker_Container_IsA(
    const MI_Instance* self)
{
    MI_Boolean res = MI_FALSE;
    return MI_Instance_IsA(self, &Docker_Container_rtti, &res) == MI_RESULT_OK && res;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Destruct(Docker_Container* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Delete(Docker_Container* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Post(
    const Docker_Container* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_InstanceID(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_InstanceID(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_InstanceID(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Caption(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Caption(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Caption(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        1);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Description(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Description(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Description(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_ElementName(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_ElementName(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_ElementName(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        3);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_InstallDate(
    Docker_Container* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->InstallDate)->value = x;
    ((MI_DatetimeField*)&self->InstallDate)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_InstallDate(
    Docker_Container* self)
{
    memset((void*)&self->InstallDate, 0, sizeof(self->InstallDate));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Name(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        5,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Name(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        5,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Name(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        5);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_OperationalStatus(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_OperationalStatus(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_OperationalStatus(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        6);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_StatusDescriptions(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_StatusDescriptions(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_StatusDescriptions(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        7);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Status(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        8,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Status(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        8,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Status(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        8);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_HealthState(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->HealthState)->value = x;
    ((MI_Uint16Field*)&self->HealthState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_HealthState(
    Docker_Container* self)
{
    memset((void*)&self->HealthState, 0, sizeof(self->HealthState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_CommunicationStatus(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->CommunicationStatus)->value = x;
    ((MI_Uint16Field*)&self->CommunicationStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_CommunicationStatus(
    Docker_Container* self)
{
    memset((void*)&self->CommunicationStatus, 0, sizeof(self->CommunicationStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_DetailedStatus(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->DetailedStatus)->value = x;
    ((MI_Uint16Field*)&self->DetailedStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_DetailedStatus(
    Docker_Container* self)
{
    memset((void*)&self->DetailedStatus, 0, sizeof(self->DetailedStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_OperatingStatus(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->OperatingStatus)->value = x;
    ((MI_Uint16Field*)&self->OperatingStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_OperatingStatus(
    Docker_Container* self)
{
    memset((void*)&self->OperatingStatus, 0, sizeof(self->OperatingStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_PrimaryStatus(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->PrimaryStatus)->value = x;
    ((MI_Uint16Field*)&self->PrimaryStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_PrimaryStatus(
    Docker_Container* self)
{
    memset((void*)&self->PrimaryStatus, 0, sizeof(self->PrimaryStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_EnabledState(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->EnabledState)->value = x;
    ((MI_Uint16Field*)&self->EnabledState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_EnabledState(
    Docker_Container* self)
{
    memset((void*)&self->EnabledState, 0, sizeof(self->EnabledState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_OtherEnabledState(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        15,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_OtherEnabledState(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        15,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_OtherEnabledState(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        15);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_RequestedState(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->RequestedState)->value = x;
    ((MI_Uint16Field*)&self->RequestedState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_RequestedState(
    Docker_Container* self)
{
    memset((void*)&self->RequestedState, 0, sizeof(self->RequestedState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_EnabledDefault(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->EnabledDefault)->value = x;
    ((MI_Uint16Field*)&self->EnabledDefault)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_EnabledDefault(
    Docker_Container* self)
{
    memset((void*)&self->EnabledDefault, 0, sizeof(self->EnabledDefault));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_TimeOfLastStateChange(
    Docker_Container* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->TimeOfLastStateChange)->value = x;
    ((MI_DatetimeField*)&self->TimeOfLastStateChange)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_TimeOfLastStateChange(
    Docker_Container* self)
{
    memset((void*)&self->TimeOfLastStateChange, 0, sizeof(self->TimeOfLastStateChange));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_AvailableRequestedStates(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_AvailableRequestedStates(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_AvailableRequestedStates(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        19);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_TransitioningToState(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->TransitioningToState)->value = x;
    ((MI_Uint16Field*)&self->TransitioningToState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_TransitioningToState(
    Docker_Container* self)
{
    memset((void*)&self->TransitioningToState, 0, sizeof(self->TransitioningToState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_CreationClassName(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        21,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_CreationClassName(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        21,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_CreationClassName(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        21);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_NameFormat(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        22,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_NameFormat(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        22,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_NameFormat(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        22);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_PrimaryOwnerName(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        23,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_PrimaryOwnerName(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        23,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_PrimaryOwnerName(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        23);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_PrimaryOwnerContact(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        24,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_PrimaryOwnerContact(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        24,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_PrimaryOwnerContact(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        24);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Roles(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Roles(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Roles(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        25);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_OtherIdentifyingInfo(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_OtherIdentifyingInfo(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_OtherIdentifyingInfo(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        26);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_IdentifyingDescriptions(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_IdentifyingDescriptions(
    Docker_Container* self,
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

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_IdentifyingDescriptions(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        27);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Dedicated(
    Docker_Container* self,
    const MI_Uint16* data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        28,
        (MI_Value*)&arr,
        MI_UINT16A,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Dedicated(
    Docker_Container* self,
    const MI_Uint16* data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        28,
        (MI_Value*)&arr,
        MI_UINT16A,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Dedicated(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        28);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_OtherDedicatedDescriptions(
    Docker_Container* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        29,
        (MI_Value*)&arr,
        MI_STRINGA,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_OtherDedicatedDescriptions(
    Docker_Container* self,
    const MI_Char** data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        29,
        (MI_Value*)&arr,
        MI_STRINGA,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_OtherDedicatedDescriptions(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        29);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_ResetCapability(
    Docker_Container* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->ResetCapability)->value = x;
    ((MI_Uint16Field*)&self->ResetCapability)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_ResetCapability(
    Docker_Container* self)
{
    memset((void*)&self->ResetCapability, 0, sizeof(self->ResetCapability));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_PowerManagementCapabilities(
    Docker_Container* self,
    const MI_Uint16* data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        31,
        (MI_Value*)&arr,
        MI_UINT16A,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_PowerManagementCapabilities(
    Docker_Container* self,
    const MI_Uint16* data,
    MI_Uint32 size)
{
    MI_Array arr;
    arr.data = (void*)data;
    arr.size = size;
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        31,
        (MI_Value*)&arr,
        MI_UINT16A,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_PowerManagementCapabilities(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        31);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_VirtualSystem(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        32,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_VirtualSystem(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        32,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_VirtualSystem(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        32);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Ports(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        33,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Ports(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        33,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Ports(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        33);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Command(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        34,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Command(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        34,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Command(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        34);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_Image(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        35,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_Image(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        35,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_Image(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        35);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Set_SizeRW(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        36,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPtr_SizeRW(
    Docker_Container* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        36,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_Clear_SizeRW(
    Docker_Container* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        36);
}

/*
**==============================================================================
**
** Docker_Container.RequestStateChange()
**
**==============================================================================
*/

typedef struct _Docker_Container_RequestStateChange
{
    MI_Instance __instance;
    /*OUT*/ MI_ConstUint32Field MIReturn;
    /*IN*/ MI_ConstUint16Field RequestedState;
    /*OUT*/ CIM_ConcreteJob_ConstRef Job;
    /*IN*/ MI_ConstDatetimeField TimeoutPeriod;
}
Docker_Container_RequestStateChange;

MI_EXTERN_C MI_CONST MI_MethodDecl Docker_Container_RequestStateChange_rtti;

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Construct(
    Docker_Container_RequestStateChange* self,
    MI_Context* context)
{
    return MI_ConstructParameters(context, &Docker_Container_RequestStateChange_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Clone(
    const Docker_Container_RequestStateChange* self,
    Docker_Container_RequestStateChange** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Destruct(
    Docker_Container_RequestStateChange* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Delete(
    Docker_Container_RequestStateChange* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Post(
    const Docker_Container_RequestStateChange* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Set_MIReturn(
    Docker_Container_RequestStateChange* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MIReturn)->value = x;
    ((MI_Uint32Field*)&self->MIReturn)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Clear_MIReturn(
    Docker_Container_RequestStateChange* self)
{
    memset((void*)&self->MIReturn, 0, sizeof(self->MIReturn));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Set_RequestedState(
    Docker_Container_RequestStateChange* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->RequestedState)->value = x;
    ((MI_Uint16Field*)&self->RequestedState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Clear_RequestedState(
    Docker_Container_RequestStateChange* self)
{
    memset((void*)&self->RequestedState, 0, sizeof(self->RequestedState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Set_Job(
    Docker_Container_RequestStateChange* self,
    const CIM_ConcreteJob* x)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&x,
        MI_REFERENCE,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_SetPtr_Job(
    Docker_Container_RequestStateChange* self,
    const CIM_ConcreteJob* x)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&x,
        MI_REFERENCE,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Clear_Job(
    Docker_Container_RequestStateChange* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Set_TimeoutPeriod(
    Docker_Container_RequestStateChange* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->TimeoutPeriod)->value = x;
    ((MI_DatetimeField*)&self->TimeoutPeriod)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_RequestStateChange_Clear_TimeoutPeriod(
    Docker_Container_RequestStateChange* self)
{
    memset((void*)&self->TimeoutPeriod, 0, sizeof(self->TimeoutPeriod));
    return MI_RESULT_OK;
}

/*
**==============================================================================
**
** Docker_Container.SetPowerState()
**
**==============================================================================
*/

typedef struct _Docker_Container_SetPowerState
{
    MI_Instance __instance;
    /*OUT*/ MI_ConstUint32Field MIReturn;
    /*IN*/ MI_ConstUint32Field PowerState;
    /*IN*/ MI_ConstDatetimeField Time;
}
Docker_Container_SetPowerState;

MI_EXTERN_C MI_CONST MI_MethodDecl Docker_Container_SetPowerState_rtti;

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Construct(
    Docker_Container_SetPowerState* self,
    MI_Context* context)
{
    return MI_ConstructParameters(context, &Docker_Container_SetPowerState_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Clone(
    const Docker_Container_SetPowerState* self,
    Docker_Container_SetPowerState** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Destruct(
    Docker_Container_SetPowerState* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Delete(
    Docker_Container_SetPowerState* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Post(
    const Docker_Container_SetPowerState* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Set_MIReturn(
    Docker_Container_SetPowerState* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MIReturn)->value = x;
    ((MI_Uint32Field*)&self->MIReturn)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Clear_MIReturn(
    Docker_Container_SetPowerState* self)
{
    memset((void*)&self->MIReturn, 0, sizeof(self->MIReturn));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Set_PowerState(
    Docker_Container_SetPowerState* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->PowerState)->value = x;
    ((MI_Uint32Field*)&self->PowerState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Clear_PowerState(
    Docker_Container_SetPowerState* self)
{
    memset((void*)&self->PowerState, 0, sizeof(self->PowerState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Set_Time(
    Docker_Container_SetPowerState* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->Time)->value = x;
    ((MI_DatetimeField*)&self->Time)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Container_SetPowerState_Clear_Time(
    Docker_Container_SetPowerState* self)
{
    memset((void*)&self->Time, 0, sizeof(self->Time));
    return MI_RESULT_OK;
}

/*
**==============================================================================
**
** Docker_Container provider function prototypes
**
**==============================================================================
*/

/* The developer may optionally define this structure */
typedef struct _Docker_Container_Self Docker_Container_Self;

MI_EXTERN_C void MI_CALL Docker_Container_Load(
    Docker_Container_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context);

MI_EXTERN_C void MI_CALL Docker_Container_Unload(
    Docker_Container_Self* self,
    MI_Context* context);

MI_EXTERN_C void MI_CALL Docker_Container_EnumerateInstances(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter);

MI_EXTERN_C void MI_CALL Docker_Container_GetInstance(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Container* instanceName,
    const MI_PropertySet* propertySet);

MI_EXTERN_C void MI_CALL Docker_Container_CreateInstance(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Container* newInstance);

MI_EXTERN_C void MI_CALL Docker_Container_ModifyInstance(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Container* modifiedInstance,
    const MI_PropertySet* propertySet);

MI_EXTERN_C void MI_CALL Docker_Container_DeleteInstance(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Container* instanceName);

MI_EXTERN_C void MI_CALL Docker_Container_Invoke_RequestStateChange(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Docker_Container* instanceName,
    const Docker_Container_RequestStateChange* in);

MI_EXTERN_C void MI_CALL Docker_Container_Invoke_SetPowerState(
    Docker_Container_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_Char* methodName,
    const Docker_Container* instanceName,
    const Docker_Container_SetPowerState* in);


/*
**==============================================================================
**
** Docker_Container_Class
**
**==============================================================================
*/

#ifdef __cplusplus
# include <micxx/micxx.h>

MI_BEGIN_NAMESPACE

class Docker_Container_Class : public CIM_VirtualComputerSystem_Class
{
public:
    
    typedef Docker_Container Self;
    
    Docker_Container_Class() :
        CIM_VirtualComputerSystem_Class(&Docker_Container_rtti)
    {
    }
    
    Docker_Container_Class(
        const Docker_Container* instanceName,
        bool keysOnly) :
        CIM_VirtualComputerSystem_Class(
            &Docker_Container_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    Docker_Container_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        CIM_VirtualComputerSystem_Class(clDecl, instance, keysOnly)
    {
    }
    
    Docker_Container_Class(
        const MI_ClassDecl* clDecl) :
        CIM_VirtualComputerSystem_Class(clDecl)
    {
    }
    
    Docker_Container_Class& operator=(
        const Docker_Container_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    Docker_Container_Class(
        const Docker_Container_Class& x) :
        CIM_VirtualComputerSystem_Class(x)
    {
    }

    static const MI_ClassDecl* GetClassDecl()
    {
        return &Docker_Container_rtti;
    }

    //
    // Docker_Container_Class.Ports
    //
    
    const Field<String>& Ports() const
    {
        const size_t n = offsetof(Self, Ports);
        return GetField<String>(n);
    }
    
    void Ports(const Field<String>& x)
    {
        const size_t n = offsetof(Self, Ports);
        GetField<String>(n) = x;
    }
    
    const String& Ports_value() const
    {
        const size_t n = offsetof(Self, Ports);
        return GetField<String>(n).value;
    }
    
    void Ports_value(const String& x)
    {
        const size_t n = offsetof(Self, Ports);
        GetField<String>(n).Set(x);
    }
    
    bool Ports_exists() const
    {
        const size_t n = offsetof(Self, Ports);
        return GetField<String>(n).exists ? true : false;
    }
    
    void Ports_clear()
    {
        const size_t n = offsetof(Self, Ports);
        GetField<String>(n).Clear();
    }

    //
    // Docker_Container_Class.Command
    //
    
    const Field<String>& Command() const
    {
        const size_t n = offsetof(Self, Command);
        return GetField<String>(n);
    }
    
    void Command(const Field<String>& x)
    {
        const size_t n = offsetof(Self, Command);
        GetField<String>(n) = x;
    }
    
    const String& Command_value() const
    {
        const size_t n = offsetof(Self, Command);
        return GetField<String>(n).value;
    }
    
    void Command_value(const String& x)
    {
        const size_t n = offsetof(Self, Command);
        GetField<String>(n).Set(x);
    }
    
    bool Command_exists() const
    {
        const size_t n = offsetof(Self, Command);
        return GetField<String>(n).exists ? true : false;
    }
    
    void Command_clear()
    {
        const size_t n = offsetof(Self, Command);
        GetField<String>(n).Clear();
    }

    //
    // Docker_Container_Class.Image
    //
    
    const Field<String>& Image() const
    {
        const size_t n = offsetof(Self, Image);
        return GetField<String>(n);
    }
    
    void Image(const Field<String>& x)
    {
        const size_t n = offsetof(Self, Image);
        GetField<String>(n) = x;
    }
    
    const String& Image_value() const
    {
        const size_t n = offsetof(Self, Image);
        return GetField<String>(n).value;
    }
    
    void Image_value(const String& x)
    {
        const size_t n = offsetof(Self, Image);
        GetField<String>(n).Set(x);
    }
    
    bool Image_exists() const
    {
        const size_t n = offsetof(Self, Image);
        return GetField<String>(n).exists ? true : false;
    }
    
    void Image_clear()
    {
        const size_t n = offsetof(Self, Image);
        GetField<String>(n).Clear();
    }

    //
    // Docker_Container_Class.SizeRW
    //
    
    const Field<String>& SizeRW() const
    {
        const size_t n = offsetof(Self, SizeRW);
        return GetField<String>(n);
    }
    
    void SizeRW(const Field<String>& x)
    {
        const size_t n = offsetof(Self, SizeRW);
        GetField<String>(n) = x;
    }
    
    const String& SizeRW_value() const
    {
        const size_t n = offsetof(Self, SizeRW);
        return GetField<String>(n).value;
    }
    
    void SizeRW_value(const String& x)
    {
        const size_t n = offsetof(Self, SizeRW);
        GetField<String>(n).Set(x);
    }
    
    bool SizeRW_exists() const
    {
        const size_t n = offsetof(Self, SizeRW);
        return GetField<String>(n).exists ? true : false;
    }
    
    void SizeRW_clear()
    {
        const size_t n = offsetof(Self, SizeRW);
        GetField<String>(n).Clear();
    }
};

typedef Array<Docker_Container_Class> Docker_Container_ClassA;

class Docker_Container_RequestStateChange_Class : public Instance
{
public:
    
    typedef Docker_Container_RequestStateChange Self;
    
    Docker_Container_RequestStateChange_Class() :
        Instance(&Docker_Container_RequestStateChange_rtti)
    {
    }
    
    Docker_Container_RequestStateChange_Class(
        const Docker_Container_RequestStateChange* instanceName,
        bool keysOnly) :
        Instance(
            &Docker_Container_RequestStateChange_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    Docker_Container_RequestStateChange_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        Instance(clDecl, instance, keysOnly)
    {
    }
    
    Docker_Container_RequestStateChange_Class(
        const MI_ClassDecl* clDecl) :
        Instance(clDecl)
    {
    }
    
    Docker_Container_RequestStateChange_Class& operator=(
        const Docker_Container_RequestStateChange_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    Docker_Container_RequestStateChange_Class(
        const Docker_Container_RequestStateChange_Class& x) :
        Instance(x)
    {
    }

    //
    // Docker_Container_RequestStateChange_Class.MIReturn
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
    // Docker_Container_RequestStateChange_Class.RequestedState
    //
    
    const Field<Uint16>& RequestedState() const
    {
        const size_t n = offsetof(Self, RequestedState);
        return GetField<Uint16>(n);
    }
    
    void RequestedState(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, RequestedState);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& RequestedState_value() const
    {
        const size_t n = offsetof(Self, RequestedState);
        return GetField<Uint16>(n).value;
    }
    
    void RequestedState_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, RequestedState);
        GetField<Uint16>(n).Set(x);
    }
    
    bool RequestedState_exists() const
    {
        const size_t n = offsetof(Self, RequestedState);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void RequestedState_clear()
    {
        const size_t n = offsetof(Self, RequestedState);
        GetField<Uint16>(n).Clear();
    }

    //
    // Docker_Container_RequestStateChange_Class.Job
    //
    
    const Field<CIM_ConcreteJob_Class>& Job() const
    {
        const size_t n = offsetof(Self, Job);
        return GetField<CIM_ConcreteJob_Class>(n);
    }
    
    void Job(const Field<CIM_ConcreteJob_Class>& x)
    {
        const size_t n = offsetof(Self, Job);
        GetField<CIM_ConcreteJob_Class>(n) = x;
    }
    
    const CIM_ConcreteJob_Class& Job_value() const
    {
        const size_t n = offsetof(Self, Job);
        return GetField<CIM_ConcreteJob_Class>(n).value;
    }
    
    void Job_value(const CIM_ConcreteJob_Class& x)
    {
        const size_t n = offsetof(Self, Job);
        GetField<CIM_ConcreteJob_Class>(n).Set(x);
    }
    
    bool Job_exists() const
    {
        const size_t n = offsetof(Self, Job);
        return GetField<CIM_ConcreteJob_Class>(n).exists ? true : false;
    }
    
    void Job_clear()
    {
        const size_t n = offsetof(Self, Job);
        GetField<CIM_ConcreteJob_Class>(n).Clear();
    }

    //
    // Docker_Container_RequestStateChange_Class.TimeoutPeriod
    //
    
    const Field<Datetime>& TimeoutPeriod() const
    {
        const size_t n = offsetof(Self, TimeoutPeriod);
        return GetField<Datetime>(n);
    }
    
    void TimeoutPeriod(const Field<Datetime>& x)
    {
        const size_t n = offsetof(Self, TimeoutPeriod);
        GetField<Datetime>(n) = x;
    }
    
    const Datetime& TimeoutPeriod_value() const
    {
        const size_t n = offsetof(Self, TimeoutPeriod);
        return GetField<Datetime>(n).value;
    }
    
    void TimeoutPeriod_value(const Datetime& x)
    {
        const size_t n = offsetof(Self, TimeoutPeriod);
        GetField<Datetime>(n).Set(x);
    }
    
    bool TimeoutPeriod_exists() const
    {
        const size_t n = offsetof(Self, TimeoutPeriod);
        return GetField<Datetime>(n).exists ? true : false;
    }
    
    void TimeoutPeriod_clear()
    {
        const size_t n = offsetof(Self, TimeoutPeriod);
        GetField<Datetime>(n).Clear();
    }
};

typedef Array<Docker_Container_RequestStateChange_Class> Docker_Container_RequestStateChange_ClassA;

class Docker_Container_SetPowerState_Class : public Instance
{
public:
    
    typedef Docker_Container_SetPowerState Self;
    
    Docker_Container_SetPowerState_Class() :
        Instance(&Docker_Container_SetPowerState_rtti)
    {
    }
    
    Docker_Container_SetPowerState_Class(
        const Docker_Container_SetPowerState* instanceName,
        bool keysOnly) :
        Instance(
            &Docker_Container_SetPowerState_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    Docker_Container_SetPowerState_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        Instance(clDecl, instance, keysOnly)
    {
    }
    
    Docker_Container_SetPowerState_Class(
        const MI_ClassDecl* clDecl) :
        Instance(clDecl)
    {
    }
    
    Docker_Container_SetPowerState_Class& operator=(
        const Docker_Container_SetPowerState_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    Docker_Container_SetPowerState_Class(
        const Docker_Container_SetPowerState_Class& x) :
        Instance(x)
    {
    }

    //
    // Docker_Container_SetPowerState_Class.MIReturn
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
    // Docker_Container_SetPowerState_Class.PowerState
    //
    
    const Field<Uint32>& PowerState() const
    {
        const size_t n = offsetof(Self, PowerState);
        return GetField<Uint32>(n);
    }
    
    void PowerState(const Field<Uint32>& x)
    {
        const size_t n = offsetof(Self, PowerState);
        GetField<Uint32>(n) = x;
    }
    
    const Uint32& PowerState_value() const
    {
        const size_t n = offsetof(Self, PowerState);
        return GetField<Uint32>(n).value;
    }
    
    void PowerState_value(const Uint32& x)
    {
        const size_t n = offsetof(Self, PowerState);
        GetField<Uint32>(n).Set(x);
    }
    
    bool PowerState_exists() const
    {
        const size_t n = offsetof(Self, PowerState);
        return GetField<Uint32>(n).exists ? true : false;
    }
    
    void PowerState_clear()
    {
        const size_t n = offsetof(Self, PowerState);
        GetField<Uint32>(n).Clear();
    }

    //
    // Docker_Container_SetPowerState_Class.Time
    //
    
    const Field<Datetime>& Time() const
    {
        const size_t n = offsetof(Self, Time);
        return GetField<Datetime>(n);
    }
    
    void Time(const Field<Datetime>& x)
    {
        const size_t n = offsetof(Self, Time);
        GetField<Datetime>(n) = x;
    }
    
    const Datetime& Time_value() const
    {
        const size_t n = offsetof(Self, Time);
        return GetField<Datetime>(n).value;
    }
    
    void Time_value(const Datetime& x)
    {
        const size_t n = offsetof(Self, Time);
        GetField<Datetime>(n).Set(x);
    }
    
    bool Time_exists() const
    {
        const size_t n = offsetof(Self, Time);
        return GetField<Datetime>(n).exists ? true : false;
    }
    
    void Time_clear()
    {
        const size_t n = offsetof(Self, Time);
        GetField<Datetime>(n).Clear();
    }
};

typedef Array<Docker_Container_SetPowerState_Class> Docker_Container_SetPowerState_ClassA;

MI_END_NAMESPACE

#endif /* __cplusplus */

#endif /* _Docker_Container_h */
