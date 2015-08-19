/* @migen@ */
/*
**==============================================================================
**
** WARNING: THIS FILE WAS AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT.
**
**==============================================================================
*/
#ifndef _CIM_ComputerSystem_h
#define _CIM_ComputerSystem_h

#include <MI.h>
#include "CIM_System.h"
#include "CIM_ConcreteJob.h"

/*
**==============================================================================
**
** CIM_ComputerSystem [CIM_ComputerSystem]
**
** Keys:
**    Name
**    CreationClassName
**
**==============================================================================
*/

typedef struct _CIM_ComputerSystem /* extends CIM_System */
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
}
CIM_ComputerSystem;

typedef struct _CIM_ComputerSystem_Ref
{
    CIM_ComputerSystem* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
CIM_ComputerSystem_Ref;

typedef struct _CIM_ComputerSystem_ConstRef
{
    MI_CONST CIM_ComputerSystem* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
CIM_ComputerSystem_ConstRef;

typedef struct _CIM_ComputerSystem_Array
{
    struct _CIM_ComputerSystem** data;
    MI_Uint32 size;
}
CIM_ComputerSystem_Array;

typedef struct _CIM_ComputerSystem_ConstArray
{
    struct _CIM_ComputerSystem MI_CONST* MI_CONST* data;
    MI_Uint32 size;
}
CIM_ComputerSystem_ConstArray;

typedef struct _CIM_ComputerSystem_ArrayRef
{
    CIM_ComputerSystem_Array value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
CIM_ComputerSystem_ArrayRef;

typedef struct _CIM_ComputerSystem_ConstArrayRef
{
    CIM_ComputerSystem_ConstArray value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
CIM_ComputerSystem_ConstArrayRef;

MI_EXTERN_C MI_CONST MI_ClassDecl CIM_ComputerSystem_rtti;

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Construct(
    CIM_ComputerSystem* self,
    MI_Context* context)
{
    return MI_ConstructInstance(context, &CIM_ComputerSystem_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clone(
    const CIM_ComputerSystem* self,
    CIM_ComputerSystem** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Boolean MI_CALL CIM_ComputerSystem_IsA(
    const MI_Instance* self)
{
    MI_Boolean res = MI_FALSE;
    return MI_Instance_IsA(self, &CIM_ComputerSystem_rtti, &res) == MI_RESULT_OK && res;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Destruct(CIM_ComputerSystem* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Delete(CIM_ComputerSystem* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Post(
    const CIM_ComputerSystem* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_InstanceID(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_InstanceID(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_InstanceID(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_Caption(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_Caption(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_Caption(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        1);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_Description(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_Description(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_Description(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_ElementName(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_ElementName(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_ElementName(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        3);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_InstallDate(
    CIM_ComputerSystem* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->InstallDate)->value = x;
    ((MI_DatetimeField*)&self->InstallDate)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_InstallDate(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->InstallDate, 0, sizeof(self->InstallDate));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_Name(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        5,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_Name(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        5,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_Name(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        5);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_OperationalStatus(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_OperationalStatus(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_OperationalStatus(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        6);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_StatusDescriptions(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_StatusDescriptions(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_StatusDescriptions(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        7);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_Status(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        8,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_Status(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        8,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_Status(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        8);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_HealthState(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->HealthState)->value = x;
    ((MI_Uint16Field*)&self->HealthState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_HealthState(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->HealthState, 0, sizeof(self->HealthState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_CommunicationStatus(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->CommunicationStatus)->value = x;
    ((MI_Uint16Field*)&self->CommunicationStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_CommunicationStatus(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->CommunicationStatus, 0, sizeof(self->CommunicationStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_DetailedStatus(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->DetailedStatus)->value = x;
    ((MI_Uint16Field*)&self->DetailedStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_DetailedStatus(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->DetailedStatus, 0, sizeof(self->DetailedStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_OperatingStatus(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->OperatingStatus)->value = x;
    ((MI_Uint16Field*)&self->OperatingStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_OperatingStatus(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->OperatingStatus, 0, sizeof(self->OperatingStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_PrimaryStatus(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->PrimaryStatus)->value = x;
    ((MI_Uint16Field*)&self->PrimaryStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_PrimaryStatus(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->PrimaryStatus, 0, sizeof(self->PrimaryStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_EnabledState(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->EnabledState)->value = x;
    ((MI_Uint16Field*)&self->EnabledState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_EnabledState(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->EnabledState, 0, sizeof(self->EnabledState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_OtherEnabledState(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        15,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_OtherEnabledState(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        15,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_OtherEnabledState(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        15);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_RequestedState(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->RequestedState)->value = x;
    ((MI_Uint16Field*)&self->RequestedState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_RequestedState(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->RequestedState, 0, sizeof(self->RequestedState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_EnabledDefault(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->EnabledDefault)->value = x;
    ((MI_Uint16Field*)&self->EnabledDefault)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_EnabledDefault(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->EnabledDefault, 0, sizeof(self->EnabledDefault));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_TimeOfLastStateChange(
    CIM_ComputerSystem* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->TimeOfLastStateChange)->value = x;
    ((MI_DatetimeField*)&self->TimeOfLastStateChange)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_TimeOfLastStateChange(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->TimeOfLastStateChange, 0, sizeof(self->TimeOfLastStateChange));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_AvailableRequestedStates(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_AvailableRequestedStates(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_AvailableRequestedStates(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        19);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_TransitioningToState(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->TransitioningToState)->value = x;
    ((MI_Uint16Field*)&self->TransitioningToState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_TransitioningToState(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->TransitioningToState, 0, sizeof(self->TransitioningToState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_CreationClassName(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        21,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_CreationClassName(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        21,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_CreationClassName(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        21);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_NameFormat(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        22,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_NameFormat(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        22,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_NameFormat(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        22);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_PrimaryOwnerName(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        23,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_PrimaryOwnerName(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        23,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_PrimaryOwnerName(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        23);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_PrimaryOwnerContact(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        24,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_PrimaryOwnerContact(
    CIM_ComputerSystem* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        24,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_PrimaryOwnerContact(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        24);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_Roles(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_Roles(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_Roles(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        25);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_OtherIdentifyingInfo(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_OtherIdentifyingInfo(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_OtherIdentifyingInfo(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        26);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_IdentifyingDescriptions(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_IdentifyingDescriptions(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_IdentifyingDescriptions(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        27);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_Dedicated(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_Dedicated(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_Dedicated(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        28);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_OtherDedicatedDescriptions(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_OtherDedicatedDescriptions(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_OtherDedicatedDescriptions(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        29);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_ResetCapability(
    CIM_ComputerSystem* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->ResetCapability)->value = x;
    ((MI_Uint16Field*)&self->ResetCapability)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_ResetCapability(
    CIM_ComputerSystem* self)
{
    memset((void*)&self->ResetCapability, 0, sizeof(self->ResetCapability));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Set_PowerManagementCapabilities(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPtr_PowerManagementCapabilities(
    CIM_ComputerSystem* self,
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

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_Clear_PowerManagementCapabilities(
    CIM_ComputerSystem* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        31);
}

/*
**==============================================================================
**
** CIM_ComputerSystem.RequestStateChange()
**
**==============================================================================
*/

typedef struct _CIM_ComputerSystem_RequestStateChange
{
    MI_Instance __instance;
    /*OUT*/ MI_ConstUint32Field MIReturn;
    /*IN*/ MI_ConstUint16Field RequestedState;
    /*OUT*/ CIM_ConcreteJob_ConstRef Job;
    /*IN*/ MI_ConstDatetimeField TimeoutPeriod;
}
CIM_ComputerSystem_RequestStateChange;

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_Set_MIReturn(
    CIM_ComputerSystem_RequestStateChange* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MIReturn)->value = x;
    ((MI_Uint32Field*)&self->MIReturn)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_Clear_MIReturn(
    CIM_ComputerSystem_RequestStateChange* self)
{
    memset((void*)&self->MIReturn, 0, sizeof(self->MIReturn));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_Set_RequestedState(
    CIM_ComputerSystem_RequestStateChange* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->RequestedState)->value = x;
    ((MI_Uint16Field*)&self->RequestedState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_Clear_RequestedState(
    CIM_ComputerSystem_RequestStateChange* self)
{
    memset((void*)&self->RequestedState, 0, sizeof(self->RequestedState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_Set_Job(
    CIM_ComputerSystem_RequestStateChange* self,
    const CIM_ConcreteJob* x)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&x,
        MI_REFERENCE,
        0);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_SetPtr_Job(
    CIM_ComputerSystem_RequestStateChange* self,
    const CIM_ConcreteJob* x)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&x,
        MI_REFERENCE,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_Clear_Job(
    CIM_ComputerSystem_RequestStateChange* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_Set_TimeoutPeriod(
    CIM_ComputerSystem_RequestStateChange* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->TimeoutPeriod)->value = x;
    ((MI_DatetimeField*)&self->TimeoutPeriod)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_RequestStateChange_Clear_TimeoutPeriod(
    CIM_ComputerSystem_RequestStateChange* self)
{
    memset((void*)&self->TimeoutPeriod, 0, sizeof(self->TimeoutPeriod));
    return MI_RESULT_OK;
}

/*
**==============================================================================
**
** CIM_ComputerSystem.SetPowerState()
**
**==============================================================================
*/

typedef struct _CIM_ComputerSystem_SetPowerState
{
    MI_Instance __instance;
    /*OUT*/ MI_ConstUint32Field MIReturn;
    /*IN*/ MI_ConstUint32Field PowerState;
    /*IN*/ MI_ConstDatetimeField Time;
}
CIM_ComputerSystem_SetPowerState;

MI_EXTERN_C MI_CONST MI_MethodDecl CIM_ComputerSystem_SetPowerState_rtti;

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Construct(
    CIM_ComputerSystem_SetPowerState* self,
    MI_Context* context)
{
    return MI_ConstructParameters(context, &CIM_ComputerSystem_SetPowerState_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Clone(
    const CIM_ComputerSystem_SetPowerState* self,
    CIM_ComputerSystem_SetPowerState** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Destruct(
    CIM_ComputerSystem_SetPowerState* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Delete(
    CIM_ComputerSystem_SetPowerState* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Post(
    const CIM_ComputerSystem_SetPowerState* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Set_MIReturn(
    CIM_ComputerSystem_SetPowerState* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->MIReturn)->value = x;
    ((MI_Uint32Field*)&self->MIReturn)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Clear_MIReturn(
    CIM_ComputerSystem_SetPowerState* self)
{
    memset((void*)&self->MIReturn, 0, sizeof(self->MIReturn));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Set_PowerState(
    CIM_ComputerSystem_SetPowerState* self,
    MI_Uint32 x)
{
    ((MI_Uint32Field*)&self->PowerState)->value = x;
    ((MI_Uint32Field*)&self->PowerState)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Clear_PowerState(
    CIM_ComputerSystem_SetPowerState* self)
{
    memset((void*)&self->PowerState, 0, sizeof(self->PowerState));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Set_Time(
    CIM_ComputerSystem_SetPowerState* self,
    MI_Datetime x)
{
    ((MI_DatetimeField*)&self->Time)->value = x;
    ((MI_DatetimeField*)&self->Time)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL CIM_ComputerSystem_SetPowerState_Clear_Time(
    CIM_ComputerSystem_SetPowerState* self)
{
    memset((void*)&self->Time, 0, sizeof(self->Time));
    return MI_RESULT_OK;
}


/*
**==============================================================================
**
** CIM_ComputerSystem_Class
**
**==============================================================================
*/

#ifdef __cplusplus
# include <micxx/micxx.h>

MI_BEGIN_NAMESPACE

class CIM_ComputerSystem_Class : public CIM_System_Class
{
public:
    
    typedef CIM_ComputerSystem Self;
    
    CIM_ComputerSystem_Class() :
        CIM_System_Class(&CIM_ComputerSystem_rtti)
    {
    }
    
    CIM_ComputerSystem_Class(
        const CIM_ComputerSystem* instanceName,
        bool keysOnly) :
        CIM_System_Class(
            &CIM_ComputerSystem_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    CIM_ComputerSystem_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        CIM_System_Class(clDecl, instance, keysOnly)
    {
    }
    
    CIM_ComputerSystem_Class(
        const MI_ClassDecl* clDecl) :
        CIM_System_Class(clDecl)
    {
    }
    
    CIM_ComputerSystem_Class& operator=(
        const CIM_ComputerSystem_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    CIM_ComputerSystem_Class(
        const CIM_ComputerSystem_Class& x) :
        CIM_System_Class(x)
    {
    }

    static const MI_ClassDecl* GetClassDecl()
    {
        return &CIM_ComputerSystem_rtti;
    }

    //
    // CIM_ComputerSystem_Class.Dedicated
    //
    
    const Field<Uint16A>& Dedicated() const
    {
        const size_t n = offsetof(Self, Dedicated);
        return GetField<Uint16A>(n);
    }
    
    void Dedicated(const Field<Uint16A>& x)
    {
        const size_t n = offsetof(Self, Dedicated);
        GetField<Uint16A>(n) = x;
    }
    
    const Uint16A& Dedicated_value() const
    {
        const size_t n = offsetof(Self, Dedicated);
        return GetField<Uint16A>(n).value;
    }
    
    void Dedicated_value(const Uint16A& x)
    {
        const size_t n = offsetof(Self, Dedicated);
        GetField<Uint16A>(n).Set(x);
    }
    
    bool Dedicated_exists() const
    {
        const size_t n = offsetof(Self, Dedicated);
        return GetField<Uint16A>(n).exists ? true : false;
    }
    
    void Dedicated_clear()
    {
        const size_t n = offsetof(Self, Dedicated);
        GetField<Uint16A>(n).Clear();
    }

    //
    // CIM_ComputerSystem_Class.OtherDedicatedDescriptions
    //
    
    const Field<StringA>& OtherDedicatedDescriptions() const
    {
        const size_t n = offsetof(Self, OtherDedicatedDescriptions);
        return GetField<StringA>(n);
    }
    
    void OtherDedicatedDescriptions(const Field<StringA>& x)
    {
        const size_t n = offsetof(Self, OtherDedicatedDescriptions);
        GetField<StringA>(n) = x;
    }
    
    const StringA& OtherDedicatedDescriptions_value() const
    {
        const size_t n = offsetof(Self, OtherDedicatedDescriptions);
        return GetField<StringA>(n).value;
    }
    
    void OtherDedicatedDescriptions_value(const StringA& x)
    {
        const size_t n = offsetof(Self, OtherDedicatedDescriptions);
        GetField<StringA>(n).Set(x);
    }
    
    bool OtherDedicatedDescriptions_exists() const
    {
        const size_t n = offsetof(Self, OtherDedicatedDescriptions);
        return GetField<StringA>(n).exists ? true : false;
    }
    
    void OtherDedicatedDescriptions_clear()
    {
        const size_t n = offsetof(Self, OtherDedicatedDescriptions);
        GetField<StringA>(n).Clear();
    }

    //
    // CIM_ComputerSystem_Class.ResetCapability
    //
    
    const Field<Uint16>& ResetCapability() const
    {
        const size_t n = offsetof(Self, ResetCapability);
        return GetField<Uint16>(n);
    }
    
    void ResetCapability(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, ResetCapability);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& ResetCapability_value() const
    {
        const size_t n = offsetof(Self, ResetCapability);
        return GetField<Uint16>(n).value;
    }
    
    void ResetCapability_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, ResetCapability);
        GetField<Uint16>(n).Set(x);
    }
    
    bool ResetCapability_exists() const
    {
        const size_t n = offsetof(Self, ResetCapability);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void ResetCapability_clear()
    {
        const size_t n = offsetof(Self, ResetCapability);
        GetField<Uint16>(n).Clear();
    }

    //
    // CIM_ComputerSystem_Class.PowerManagementCapabilities
    //
    
    const Field<Uint16A>& PowerManagementCapabilities() const
    {
        const size_t n = offsetof(Self, PowerManagementCapabilities);
        return GetField<Uint16A>(n);
    }
    
    void PowerManagementCapabilities(const Field<Uint16A>& x)
    {
        const size_t n = offsetof(Self, PowerManagementCapabilities);
        GetField<Uint16A>(n) = x;
    }
    
    const Uint16A& PowerManagementCapabilities_value() const
    {
        const size_t n = offsetof(Self, PowerManagementCapabilities);
        return GetField<Uint16A>(n).value;
    }
    
    void PowerManagementCapabilities_value(const Uint16A& x)
    {
        const size_t n = offsetof(Self, PowerManagementCapabilities);
        GetField<Uint16A>(n).Set(x);
    }
    
    bool PowerManagementCapabilities_exists() const
    {
        const size_t n = offsetof(Self, PowerManagementCapabilities);
        return GetField<Uint16A>(n).exists ? true : false;
    }
    
    void PowerManagementCapabilities_clear()
    {
        const size_t n = offsetof(Self, PowerManagementCapabilities);
        GetField<Uint16A>(n).Clear();
    }
};

typedef Array<CIM_ComputerSystem_Class> CIM_ComputerSystem_ClassA;

class CIM_ComputerSystem_SetPowerState_Class : public Instance
{
public:
    
    typedef CIM_ComputerSystem_SetPowerState Self;
    
    CIM_ComputerSystem_SetPowerState_Class() :
        Instance(&CIM_ComputerSystem_SetPowerState_rtti)
    {
    }
    
    CIM_ComputerSystem_SetPowerState_Class(
        const CIM_ComputerSystem_SetPowerState* instanceName,
        bool keysOnly) :
        Instance(
            &CIM_ComputerSystem_SetPowerState_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    CIM_ComputerSystem_SetPowerState_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        Instance(clDecl, instance, keysOnly)
    {
    }
    
    CIM_ComputerSystem_SetPowerState_Class(
        const MI_ClassDecl* clDecl) :
        Instance(clDecl)
    {
    }
    
    CIM_ComputerSystem_SetPowerState_Class& operator=(
        const CIM_ComputerSystem_SetPowerState_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    CIM_ComputerSystem_SetPowerState_Class(
        const CIM_ComputerSystem_SetPowerState_Class& x) :
        Instance(x)
    {
    }

    //
    // CIM_ComputerSystem_SetPowerState_Class.MIReturn
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
    // CIM_ComputerSystem_SetPowerState_Class.PowerState
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
    // CIM_ComputerSystem_SetPowerState_Class.Time
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

typedef Array<CIM_ComputerSystem_SetPowerState_Class> CIM_ComputerSystem_SetPowerState_ClassA;

MI_END_NAMESPACE

#endif /* __cplusplus */

#endif /* _CIM_ComputerSystem_h */
