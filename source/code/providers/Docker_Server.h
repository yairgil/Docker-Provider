/* @migen@ */
/*
**==============================================================================
**
** WARNING: THIS FILE WAS AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT.
**
**==============================================================================
*/
#ifndef _Docker_Server_h
#define _Docker_Server_h

#include <MI.h>
#include "CIM_InstalledProduct.h"

/*
**==============================================================================
**
** Docker_Server [Docker_Server]
**
** Keys:
**    ProductIdentifyingNumber
**    ProductName
**    ProductVendor
**    ProductVersion
**    SystemID
**    CollectionID
**
**==============================================================================
*/

typedef struct _Docker_Server /* extends CIM_InstalledProduct */
{
    MI_Instance __instance;
    /* CIM_ManagedElement properties */
    MI_ConstStringField InstanceID;
    MI_ConstStringField Caption;
    MI_ConstStringField Description;
    MI_ConstStringField ElementName;
    /* CIM_Collection properties */
    /* CIM_InstalledProduct properties */
    /*KEY*/ MI_ConstStringField ProductIdentifyingNumber;
    /*KEY*/ MI_ConstStringField ProductName;
    /*KEY*/ MI_ConstStringField ProductVendor;
    /*KEY*/ MI_ConstStringField ProductVersion;
    /*KEY*/ MI_ConstStringField SystemID;
    /*KEY*/ MI_ConstStringField CollectionID;
    MI_ConstStringField Name;
    /* Docker_Server properties */
    MI_ConstUint16Field Containers;
    MI_ConstStringField DockerRootDir;
    MI_ConstStringField Hostname;
    MI_ConstStringField Driver;
    MI_ConstUint16Field DriverStatus;
    MI_ConstUint16Field Images;
    MI_ConstStringField InitPath;
    MI_ConstStringField KernelVersion;
    MI_ConstUint16Field OperatingStatus;
    MI_ConstUint64Field MemTotal;
    MI_ConstUint64Field MemLimit;
    MI_ConstUint64Field SwapLimit;
    MI_ConstUint16Field NCPU;
}
Docker_Server;

typedef struct _Docker_Server_Ref
{
    Docker_Server* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Docker_Server_Ref;

typedef struct _Docker_Server_ConstRef
{
    MI_CONST Docker_Server* value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Docker_Server_ConstRef;

typedef struct _Docker_Server_Array
{
    struct _Docker_Server** data;
    MI_Uint32 size;
}
Docker_Server_Array;

typedef struct _Docker_Server_ConstArray
{
    struct _Docker_Server MI_CONST* MI_CONST* data;
    MI_Uint32 size;
}
Docker_Server_ConstArray;

typedef struct _Docker_Server_ArrayRef
{
    Docker_Server_Array value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Docker_Server_ArrayRef;

typedef struct _Docker_Server_ConstArrayRef
{
    Docker_Server_ConstArray value;
    MI_Boolean exists;
    MI_Uint8 flags;
}
Docker_Server_ConstArrayRef;

MI_EXTERN_C MI_CONST MI_ClassDecl Docker_Server_rtti;

MI_INLINE MI_Result MI_CALL Docker_Server_Construct(
    Docker_Server* self,
    MI_Context* context)
{
    return MI_ConstructInstance(context, &Docker_Server_rtti,
        (MI_Instance*)&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clone(
    const Docker_Server* self,
    Docker_Server** newInstance)
{
    return MI_Instance_Clone(
        &self->__instance, (MI_Instance**)newInstance);
}

MI_INLINE MI_Boolean MI_CALL Docker_Server_IsA(
    const MI_Instance* self)
{
    MI_Boolean res = MI_FALSE;
    return MI_Instance_IsA(self, &Docker_Server_rtti, &res) == MI_RESULT_OK && res;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Destruct(Docker_Server* self)
{
    return MI_Instance_Destruct(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Delete(Docker_Server* self)
{
    return MI_Instance_Delete(&self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Post(
    const Docker_Server* self,
    MI_Context* context)
{
    return MI_PostInstance(context, &self->__instance);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_InstanceID(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_InstanceID(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        0,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_InstanceID(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_Caption(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_Caption(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        1,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_Caption(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        1);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_Description(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_Description(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        2,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_Description(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        2);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_ElementName(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_ElementName(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        3,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_ElementName(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        3);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_ProductIdentifyingNumber(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        4,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_ProductIdentifyingNumber(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        4,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_ProductIdentifyingNumber(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        4);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_ProductName(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        5,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_ProductName(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        5,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_ProductName(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        5);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_ProductVendor(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        6,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_ProductVendor(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        6,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_ProductVendor(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        6);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_ProductVersion(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        7,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_ProductVersion(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        7,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_ProductVersion(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        7);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_SystemID(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        8,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_SystemID(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        8,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_SystemID(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        8);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_CollectionID(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        9,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_CollectionID(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        9,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_CollectionID(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        9);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_Name(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        10,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_Name(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        10,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_Name(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        10);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_Containers(
    Docker_Server* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->Containers)->value = x;
    ((MI_Uint16Field*)&self->Containers)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_Containers(
    Docker_Server* self)
{
    memset((void*)&self->Containers, 0, sizeof(self->Containers));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_DockerRootDir(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        12,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_DockerRootDir(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        12,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_DockerRootDir(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        12);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_Hostname(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        13,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_Hostname(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        13,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_Hostname(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        13);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_Driver(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        14,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_Driver(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        14,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_Driver(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        14);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_DriverStatus(
    Docker_Server* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->DriverStatus)->value = x;
    ((MI_Uint16Field*)&self->DriverStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_DriverStatus(
    Docker_Server* self)
{
    memset((void*)&self->DriverStatus, 0, sizeof(self->DriverStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_Images(
    Docker_Server* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->Images)->value = x;
    ((MI_Uint16Field*)&self->Images)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_Images(
    Docker_Server* self)
{
    memset((void*)&self->Images, 0, sizeof(self->Images));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_InitPath(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        17,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_InitPath(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        17,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_InitPath(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        17);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_KernelVersion(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        18,
        (MI_Value*)&str,
        MI_STRING,
        0);
}

MI_INLINE MI_Result MI_CALL Docker_Server_SetPtr_KernelVersion(
    Docker_Server* self,
    const MI_Char* str)
{
    return self->__instance.ft->SetElementAt(
        (MI_Instance*)&self->__instance,
        18,
        (MI_Value*)&str,
        MI_STRING,
        MI_FLAG_BORROW);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_KernelVersion(
    Docker_Server* self)
{
    return self->__instance.ft->ClearElementAt(
        (MI_Instance*)&self->__instance,
        18);
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_OperatingStatus(
    Docker_Server* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->OperatingStatus)->value = x;
    ((MI_Uint16Field*)&self->OperatingStatus)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_OperatingStatus(
    Docker_Server* self)
{
    memset((void*)&self->OperatingStatus, 0, sizeof(self->OperatingStatus));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_MemTotal(
    Docker_Server* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->MemTotal)->value = x;
    ((MI_Uint64Field*)&self->MemTotal)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_MemTotal(
    Docker_Server* self)
{
    memset((void*)&self->MemTotal, 0, sizeof(self->MemTotal));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_MemLimit(
    Docker_Server* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->MemLimit)->value = x;
    ((MI_Uint64Field*)&self->MemLimit)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_MemLimit(
    Docker_Server* self)
{
    memset((void*)&self->MemLimit, 0, sizeof(self->MemLimit));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_SwapLimit(
    Docker_Server* self,
    MI_Uint64 x)
{
    ((MI_Uint64Field*)&self->SwapLimit)->value = x;
    ((MI_Uint64Field*)&self->SwapLimit)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_SwapLimit(
    Docker_Server* self)
{
    memset((void*)&self->SwapLimit, 0, sizeof(self->SwapLimit));
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Set_NCPU(
    Docker_Server* self,
    MI_Uint16 x)
{
    ((MI_Uint16Field*)&self->NCPU)->value = x;
    ((MI_Uint16Field*)&self->NCPU)->exists = 1;
    return MI_RESULT_OK;
}

MI_INLINE MI_Result MI_CALL Docker_Server_Clear_NCPU(
    Docker_Server* self)
{
    memset((void*)&self->NCPU, 0, sizeof(self->NCPU));
    return MI_RESULT_OK;
}

/*
**==============================================================================
**
** Docker_Server provider function prototypes
**
**==============================================================================
*/

/* The developer may optionally define this structure */
typedef struct _Docker_Server_Self Docker_Server_Self;

MI_EXTERN_C void MI_CALL Docker_Server_Load(
    Docker_Server_Self** self,
    MI_Module_Self* selfModule,
    MI_Context* context);

MI_EXTERN_C void MI_CALL Docker_Server_Unload(
    Docker_Server_Self* self,
    MI_Context* context);

MI_EXTERN_C void MI_CALL Docker_Server_EnumerateInstances(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const MI_PropertySet* propertySet,
    MI_Boolean keysOnly,
    const MI_Filter* filter);

MI_EXTERN_C void MI_CALL Docker_Server_GetInstance(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Server* instanceName,
    const MI_PropertySet* propertySet);

MI_EXTERN_C void MI_CALL Docker_Server_CreateInstance(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Server* newInstance);

MI_EXTERN_C void MI_CALL Docker_Server_ModifyInstance(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Server* modifiedInstance,
    const MI_PropertySet* propertySet);

MI_EXTERN_C void MI_CALL Docker_Server_DeleteInstance(
    Docker_Server_Self* self,
    MI_Context* context,
    const MI_Char* nameSpace,
    const MI_Char* className,
    const Docker_Server* instanceName);


/*
**==============================================================================
**
** Docker_Server_Class
**
**==============================================================================
*/

#ifdef __cplusplus
# include <micxx/micxx.h>

MI_BEGIN_NAMESPACE

class Docker_Server_Class : public CIM_InstalledProduct_Class
{
public:
    
    typedef Docker_Server Self;
    
    Docker_Server_Class() :
        CIM_InstalledProduct_Class(&Docker_Server_rtti)
    {
    }
    
    Docker_Server_Class(
        const Docker_Server* instanceName,
        bool keysOnly) :
        CIM_InstalledProduct_Class(
            &Docker_Server_rtti,
            &instanceName->__instance,
            keysOnly)
    {
    }
    
    Docker_Server_Class(
        const MI_ClassDecl* clDecl,
        const MI_Instance* instance,
        bool keysOnly) :
        CIM_InstalledProduct_Class(clDecl, instance, keysOnly)
    {
    }
    
    Docker_Server_Class(
        const MI_ClassDecl* clDecl) :
        CIM_InstalledProduct_Class(clDecl)
    {
    }
    
    Docker_Server_Class& operator=(
        const Docker_Server_Class& x)
    {
        CopyRef(x);
        return *this;
    }
    
    Docker_Server_Class(
        const Docker_Server_Class& x) :
        CIM_InstalledProduct_Class(x)
    {
    }

    static const MI_ClassDecl* GetClassDecl()
    {
        return &Docker_Server_rtti;
    }

    //
    // Docker_Server_Class.Containers
    //
    
    const Field<Uint16>& Containers() const
    {
        const size_t n = offsetof(Self, Containers);
        return GetField<Uint16>(n);
    }
    
    void Containers(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, Containers);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& Containers_value() const
    {
        const size_t n = offsetof(Self, Containers);
        return GetField<Uint16>(n).value;
    }
    
    void Containers_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, Containers);
        GetField<Uint16>(n).Set(x);
    }
    
    bool Containers_exists() const
    {
        const size_t n = offsetof(Self, Containers);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void Containers_clear()
    {
        const size_t n = offsetof(Self, Containers);
        GetField<Uint16>(n).Clear();
    }

    //
    // Docker_Server_Class.DockerRootDir
    //
    
    const Field<String>& DockerRootDir() const
    {
        const size_t n = offsetof(Self, DockerRootDir);
        return GetField<String>(n);
    }
    
    void DockerRootDir(const Field<String>& x)
    {
        const size_t n = offsetof(Self, DockerRootDir);
        GetField<String>(n) = x;
    }
    
    const String& DockerRootDir_value() const
    {
        const size_t n = offsetof(Self, DockerRootDir);
        return GetField<String>(n).value;
    }
    
    void DockerRootDir_value(const String& x)
    {
        const size_t n = offsetof(Self, DockerRootDir);
        GetField<String>(n).Set(x);
    }
    
    bool DockerRootDir_exists() const
    {
        const size_t n = offsetof(Self, DockerRootDir);
        return GetField<String>(n).exists ? true : false;
    }
    
    void DockerRootDir_clear()
    {
        const size_t n = offsetof(Self, DockerRootDir);
        GetField<String>(n).Clear();
    }

    //
    // Docker_Server_Class.Hostname
    //
    
    const Field<String>& Hostname() const
    {
        const size_t n = offsetof(Self, Hostname);
        return GetField<String>(n);
    }
    
    void Hostname(const Field<String>& x)
    {
        const size_t n = offsetof(Self, Hostname);
        GetField<String>(n) = x;
    }
    
    const String& Hostname_value() const
    {
        const size_t n = offsetof(Self, Hostname);
        return GetField<String>(n).value;
    }
    
    void Hostname_value(const String& x)
    {
        const size_t n = offsetof(Self, Hostname);
        GetField<String>(n).Set(x);
    }
    
    bool Hostname_exists() const
    {
        const size_t n = offsetof(Self, Hostname);
        return GetField<String>(n).exists ? true : false;
    }
    
    void Hostname_clear()
    {
        const size_t n = offsetof(Self, Hostname);
        GetField<String>(n).Clear();
    }

    //
    // Docker_Server_Class.Driver
    //
    
    const Field<String>& Driver() const
    {
        const size_t n = offsetof(Self, Driver);
        return GetField<String>(n);
    }
    
    void Driver(const Field<String>& x)
    {
        const size_t n = offsetof(Self, Driver);
        GetField<String>(n) = x;
    }
    
    const String& Driver_value() const
    {
        const size_t n = offsetof(Self, Driver);
        return GetField<String>(n).value;
    }
    
    void Driver_value(const String& x)
    {
        const size_t n = offsetof(Self, Driver);
        GetField<String>(n).Set(x);
    }
    
    bool Driver_exists() const
    {
        const size_t n = offsetof(Self, Driver);
        return GetField<String>(n).exists ? true : false;
    }
    
    void Driver_clear()
    {
        const size_t n = offsetof(Self, Driver);
        GetField<String>(n).Clear();
    }

    //
    // Docker_Server_Class.DriverStatus
    //
    
    const Field<Uint16>& DriverStatus() const
    {
        const size_t n = offsetof(Self, DriverStatus);
        return GetField<Uint16>(n);
    }
    
    void DriverStatus(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, DriverStatus);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& DriverStatus_value() const
    {
        const size_t n = offsetof(Self, DriverStatus);
        return GetField<Uint16>(n).value;
    }
    
    void DriverStatus_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, DriverStatus);
        GetField<Uint16>(n).Set(x);
    }
    
    bool DriverStatus_exists() const
    {
        const size_t n = offsetof(Self, DriverStatus);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void DriverStatus_clear()
    {
        const size_t n = offsetof(Self, DriverStatus);
        GetField<Uint16>(n).Clear();
    }

    //
    // Docker_Server_Class.Images
    //
    
    const Field<Uint16>& Images() const
    {
        const size_t n = offsetof(Self, Images);
        return GetField<Uint16>(n);
    }
    
    void Images(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, Images);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& Images_value() const
    {
        const size_t n = offsetof(Self, Images);
        return GetField<Uint16>(n).value;
    }
    
    void Images_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, Images);
        GetField<Uint16>(n).Set(x);
    }
    
    bool Images_exists() const
    {
        const size_t n = offsetof(Self, Images);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void Images_clear()
    {
        const size_t n = offsetof(Self, Images);
        GetField<Uint16>(n).Clear();
    }

    //
    // Docker_Server_Class.InitPath
    //
    
    const Field<String>& InitPath() const
    {
        const size_t n = offsetof(Self, InitPath);
        return GetField<String>(n);
    }
    
    void InitPath(const Field<String>& x)
    {
        const size_t n = offsetof(Self, InitPath);
        GetField<String>(n) = x;
    }
    
    const String& InitPath_value() const
    {
        const size_t n = offsetof(Self, InitPath);
        return GetField<String>(n).value;
    }
    
    void InitPath_value(const String& x)
    {
        const size_t n = offsetof(Self, InitPath);
        GetField<String>(n).Set(x);
    }
    
    bool InitPath_exists() const
    {
        const size_t n = offsetof(Self, InitPath);
        return GetField<String>(n).exists ? true : false;
    }
    
    void InitPath_clear()
    {
        const size_t n = offsetof(Self, InitPath);
        GetField<String>(n).Clear();
    }

    //
    // Docker_Server_Class.KernelVersion
    //
    
    const Field<String>& KernelVersion() const
    {
        const size_t n = offsetof(Self, KernelVersion);
        return GetField<String>(n);
    }
    
    void KernelVersion(const Field<String>& x)
    {
        const size_t n = offsetof(Self, KernelVersion);
        GetField<String>(n) = x;
    }
    
    const String& KernelVersion_value() const
    {
        const size_t n = offsetof(Self, KernelVersion);
        return GetField<String>(n).value;
    }
    
    void KernelVersion_value(const String& x)
    {
        const size_t n = offsetof(Self, KernelVersion);
        GetField<String>(n).Set(x);
    }
    
    bool KernelVersion_exists() const
    {
        const size_t n = offsetof(Self, KernelVersion);
        return GetField<String>(n).exists ? true : false;
    }
    
    void KernelVersion_clear()
    {
        const size_t n = offsetof(Self, KernelVersion);
        GetField<String>(n).Clear();
    }

    //
    // Docker_Server_Class.OperatingStatus
    //
    
    const Field<Uint16>& OperatingStatus() const
    {
        const size_t n = offsetof(Self, OperatingStatus);
        return GetField<Uint16>(n);
    }
    
    void OperatingStatus(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, OperatingStatus);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& OperatingStatus_value() const
    {
        const size_t n = offsetof(Self, OperatingStatus);
        return GetField<Uint16>(n).value;
    }
    
    void OperatingStatus_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, OperatingStatus);
        GetField<Uint16>(n).Set(x);
    }
    
    bool OperatingStatus_exists() const
    {
        const size_t n = offsetof(Self, OperatingStatus);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void OperatingStatus_clear()
    {
        const size_t n = offsetof(Self, OperatingStatus);
        GetField<Uint16>(n).Clear();
    }

    //
    // Docker_Server_Class.MemTotal
    //
    
    const Field<Uint64>& MemTotal() const
    {
        const size_t n = offsetof(Self, MemTotal);
        return GetField<Uint64>(n);
    }
    
    void MemTotal(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, MemTotal);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& MemTotal_value() const
    {
        const size_t n = offsetof(Self, MemTotal);
        return GetField<Uint64>(n).value;
    }
    
    void MemTotal_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, MemTotal);
        GetField<Uint64>(n).Set(x);
    }
    
    bool MemTotal_exists() const
    {
        const size_t n = offsetof(Self, MemTotal);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void MemTotal_clear()
    {
        const size_t n = offsetof(Self, MemTotal);
        GetField<Uint64>(n).Clear();
    }

    //
    // Docker_Server_Class.MemLimit
    //
    
    const Field<Uint64>& MemLimit() const
    {
        const size_t n = offsetof(Self, MemLimit);
        return GetField<Uint64>(n);
    }
    
    void MemLimit(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, MemLimit);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& MemLimit_value() const
    {
        const size_t n = offsetof(Self, MemLimit);
        return GetField<Uint64>(n).value;
    }
    
    void MemLimit_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, MemLimit);
        GetField<Uint64>(n).Set(x);
    }
    
    bool MemLimit_exists() const
    {
        const size_t n = offsetof(Self, MemLimit);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void MemLimit_clear()
    {
        const size_t n = offsetof(Self, MemLimit);
        GetField<Uint64>(n).Clear();
    }

    //
    // Docker_Server_Class.SwapLimit
    //
    
    const Field<Uint64>& SwapLimit() const
    {
        const size_t n = offsetof(Self, SwapLimit);
        return GetField<Uint64>(n);
    }
    
    void SwapLimit(const Field<Uint64>& x)
    {
        const size_t n = offsetof(Self, SwapLimit);
        GetField<Uint64>(n) = x;
    }
    
    const Uint64& SwapLimit_value() const
    {
        const size_t n = offsetof(Self, SwapLimit);
        return GetField<Uint64>(n).value;
    }
    
    void SwapLimit_value(const Uint64& x)
    {
        const size_t n = offsetof(Self, SwapLimit);
        GetField<Uint64>(n).Set(x);
    }
    
    bool SwapLimit_exists() const
    {
        const size_t n = offsetof(Self, SwapLimit);
        return GetField<Uint64>(n).exists ? true : false;
    }
    
    void SwapLimit_clear()
    {
        const size_t n = offsetof(Self, SwapLimit);
        GetField<Uint64>(n).Clear();
    }

    //
    // Docker_Server_Class.NCPU
    //
    
    const Field<Uint16>& NCPU() const
    {
        const size_t n = offsetof(Self, NCPU);
        return GetField<Uint16>(n);
    }
    
    void NCPU(const Field<Uint16>& x)
    {
        const size_t n = offsetof(Self, NCPU);
        GetField<Uint16>(n) = x;
    }
    
    const Uint16& NCPU_value() const
    {
        const size_t n = offsetof(Self, NCPU);
        return GetField<Uint16>(n).value;
    }
    
    void NCPU_value(const Uint16& x)
    {
        const size_t n = offsetof(Self, NCPU);
        GetField<Uint16>(n).Set(x);
    }
    
    bool NCPU_exists() const
    {
        const size_t n = offsetof(Self, NCPU);
        return GetField<Uint16>(n).exists ? true : false;
    }
    
    void NCPU_clear()
    {
        const size_t n = offsetof(Self, NCPU);
        GetField<Uint16>(n).Clear();
    }
};

typedef Array<Docker_Server_Class> Docker_Server_ClassA;

MI_END_NAMESPACE

#endif /* __cplusplus */

#endif /* _Docker_Server_h */
