/* @migen@ */
/*
**==============================================================================
**
** WARNING: THIS FILE WAS AUTOMATICALLY GENERATED. PLEASE DO NOT EDIT.
**
**==============================================================================
*/
#include <ctype.h>
#include <MI.h>
#include "Docker_ContainerStatistics.h"
#include "Docker_Server.h"
#include "Docker_ContainerProcessorStatistics.h"
#include "Docker_Container.h"

/*
**==============================================================================
**
** Schema Declaration
**
**==============================================================================
*/

extern MI_SchemaDecl schemaDecl;

/*
**==============================================================================
**
** Qualifier declarations
**
**==============================================================================
*/

/*
**==============================================================================
**
** CIM_ManagedElement
**
**==============================================================================
*/

/* property CIM_ManagedElement.InstanceID */
static MI_CONST MI_PropertyDecl CIM_ManagedElement_InstanceID_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0069640A, /* code */
    MI_T("InstanceID"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedElement, InstanceID), /* offset */
    MI_T("CIM_ManagedElement"), /* origin */
    MI_T("CIM_ManagedElement"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_ManagedElement_Caption_MaxLen_qual_value = 64U;

static MI_CONST MI_Qualifier CIM_ManagedElement_Caption_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_ManagedElement_Caption_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ManagedElement_Caption_quals[] =
{
    &CIM_ManagedElement_Caption_MaxLen_qual,
};

/* property CIM_ManagedElement.Caption */
static MI_CONST MI_PropertyDecl CIM_ManagedElement_Caption_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00636E07, /* code */
    MI_T("Caption"), /* name */
    CIM_ManagedElement_Caption_quals, /* qualifiers */
    MI_COUNT(CIM_ManagedElement_Caption_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedElement, Caption), /* offset */
    MI_T("CIM_ManagedElement"), /* origin */
    MI_T("CIM_ManagedElement"), /* propagator */
    NULL,
};

/* property CIM_ManagedElement.Description */
static MI_CONST MI_PropertyDecl CIM_ManagedElement_Description_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00646E0B, /* code */
    MI_T("Description"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedElement, Description), /* offset */
    MI_T("CIM_ManagedElement"), /* origin */
    MI_T("CIM_ManagedElement"), /* propagator */
    NULL,
};

/* property CIM_ManagedElement.ElementName */
static MI_CONST MI_PropertyDecl CIM_ManagedElement_ElementName_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0065650B, /* code */
    MI_T("ElementName"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedElement, ElementName), /* offset */
    MI_T("CIM_ManagedElement"), /* origin */
    MI_T("CIM_ManagedElement"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_ManagedElement_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
};

static MI_CONST MI_Char* CIM_ManagedElement_Version_qual_value = MI_T("2.19.0");

static MI_CONST MI_Qualifier CIM_ManagedElement_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_ManagedElement_Version_qual_value
};

static MI_CONST MI_Char* CIM_ManagedElement_UMLPackagePath_qual_value = MI_T("CIM::Core::CoreElements");

static MI_CONST MI_Qualifier CIM_ManagedElement_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_ManagedElement_UMLPackagePath_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ManagedElement_quals[] =
{
    &CIM_ManagedElement_Version_qual,
    &CIM_ManagedElement_UMLPackagePath_qual,
};

/* class CIM_ManagedElement */
MI_CONST MI_ClassDecl CIM_ManagedElement_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_ABSTRACT, /* flags */
    0x00637412, /* code */
    MI_T("CIM_ManagedElement"), /* name */
    CIM_ManagedElement_quals, /* qualifiers */
    MI_COUNT(CIM_ManagedElement_quals), /* numQualifiers */
    CIM_ManagedElement_props, /* properties */
    MI_COUNT(CIM_ManagedElement_props), /* numProperties */
    sizeof(CIM_ManagedElement), /* size */
    NULL, /* superClass */
    NULL, /* superClassDecl */
    NULL, /* methods */
    0, /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_StatisticalData
**
**==============================================================================
*/

static MI_CONST MI_Char* CIM_StatisticalData_InstanceID_Override_qual_value = MI_T("InstanceID");

static MI_CONST MI_Qualifier CIM_StatisticalData_InstanceID_Override_qual =
{
    MI_T("Override"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_StatisticalData_InstanceID_Override_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_StatisticalData_InstanceID_quals[] =
{
    &CIM_StatisticalData_InstanceID_Override_qual,
};

/* property CIM_StatisticalData.InstanceID */
static MI_CONST MI_PropertyDecl CIM_StatisticalData_InstanceID_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x0069640A, /* code */
    MI_T("InstanceID"), /* name */
    CIM_StatisticalData_InstanceID_quals, /* qualifiers */
    MI_COUNT(CIM_StatisticalData_InstanceID_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_StatisticalData, InstanceID), /* offset */
    MI_T("CIM_ManagedElement"), /* origin */
    MI_T("CIM_StatisticalData"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_StatisticalData_ElementName_Override_qual_value = MI_T("ElementName");

static MI_CONST MI_Qualifier CIM_StatisticalData_ElementName_Override_qual =
{
    MI_T("Override"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_StatisticalData_ElementName_Override_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_StatisticalData_ElementName_quals[] =
{
    &CIM_StatisticalData_ElementName_Override_qual,
};

/* property CIM_StatisticalData.ElementName */
static MI_CONST MI_PropertyDecl CIM_StatisticalData_ElementName_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_REQUIRED, /* flags */
    0x0065650B, /* code */
    MI_T("ElementName"), /* name */
    CIM_StatisticalData_ElementName_quals, /* qualifiers */
    MI_COUNT(CIM_StatisticalData_ElementName_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_StatisticalData, ElementName), /* offset */
    MI_T("CIM_ManagedElement"), /* origin */
    MI_T("CIM_StatisticalData"), /* propagator */
    NULL,
};

/* property CIM_StatisticalData.StartStatisticTime */
static MI_CONST MI_PropertyDecl CIM_StatisticalData_StartStatisticTime_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00736512, /* code */
    MI_T("StartStatisticTime"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_StatisticalData, StartStatisticTime), /* offset */
    MI_T("CIM_StatisticalData"), /* origin */
    MI_T("CIM_StatisticalData"), /* propagator */
    NULL,
};

/* property CIM_StatisticalData.StatisticTime */
static MI_CONST MI_PropertyDecl CIM_StatisticalData_StatisticTime_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0073650D, /* code */
    MI_T("StatisticTime"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_StatisticalData, StatisticTime), /* offset */
    MI_T("CIM_StatisticalData"), /* origin */
    MI_T("CIM_StatisticalData"), /* propagator */
    NULL,
};

static MI_CONST MI_Datetime CIM_StatisticalData_SampleInterval_value = {0,{{0,0,0,0,0}}};

/* property CIM_StatisticalData.SampleInterval */
static MI_CONST MI_PropertyDecl CIM_StatisticalData_SampleInterval_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00736C0E, /* code */
    MI_T("SampleInterval"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_StatisticalData, SampleInterval), /* offset */
    MI_T("CIM_StatisticalData"), /* origin */
    MI_T("CIM_StatisticalData"), /* propagator */
    &CIM_StatisticalData_SampleInterval_value,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_StatisticalData_props[] =
{
    &CIM_StatisticalData_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_StatisticalData_ElementName_prop,
    &CIM_StatisticalData_StartStatisticTime_prop,
    &CIM_StatisticalData_StatisticTime_prop,
    &CIM_StatisticalData_SampleInterval_prop,
};

/* parameter CIM_StatisticalData.ResetSelectedStats(): SelectedStatistics */
static MI_CONST MI_ParameterDecl CIM_StatisticalData_ResetSelectedStats_SelectedStatistics_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x00737312, /* code */
    MI_T("SelectedStatistics"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_StatisticalData_ResetSelectedStats, SelectedStatistics), /* offset */
};

/* parameter CIM_StatisticalData.ResetSelectedStats(): MIReturn */
static MI_CONST MI_ParameterDecl CIM_StatisticalData_ResetSelectedStats_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_StatisticalData_ResetSelectedStats, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST CIM_StatisticalData_ResetSelectedStats_params[] =
{
    &CIM_StatisticalData_ResetSelectedStats_MIReturn_param,
    &CIM_StatisticalData_ResetSelectedStats_SelectedStatistics_param,
};

/* method CIM_StatisticalData.ResetSelectedStats() */
MI_CONST MI_MethodDecl CIM_StatisticalData_ResetSelectedStats_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x00727312, /* code */
    MI_T("ResetSelectedStats"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    CIM_StatisticalData_ResetSelectedStats_params, /* parameters */
    MI_COUNT(CIM_StatisticalData_ResetSelectedStats_params), /* numParameters */
    sizeof(CIM_StatisticalData_ResetSelectedStats), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_StatisticalData"), /* origin */
    MI_T("CIM_StatisticalData"), /* propagator */
    &schemaDecl, /* schema */
    NULL, /* method */
};

static MI_MethodDecl MI_CONST* MI_CONST CIM_StatisticalData_meths[] =
{
    &CIM_StatisticalData_ResetSelectedStats_rtti,
};

static MI_CONST MI_Char* CIM_StatisticalData_UMLPackagePath_qual_value = MI_T("CIM::Core::Statistics");

static MI_CONST MI_Qualifier CIM_StatisticalData_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_StatisticalData_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_StatisticalData_Version_qual_value = MI_T("2.19.0");

static MI_CONST MI_Qualifier CIM_StatisticalData_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_StatisticalData_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_StatisticalData_quals[] =
{
    &CIM_StatisticalData_UMLPackagePath_qual,
    &CIM_StatisticalData_Version_qual,
};

/* class CIM_StatisticalData */
MI_CONST MI_ClassDecl CIM_StatisticalData_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_ABSTRACT, /* flags */
    0x00636113, /* code */
    MI_T("CIM_StatisticalData"), /* name */
    CIM_StatisticalData_quals, /* qualifiers */
    MI_COUNT(CIM_StatisticalData_quals), /* numQualifiers */
    CIM_StatisticalData_props, /* properties */
    MI_COUNT(CIM_StatisticalData_props), /* numProperties */
    sizeof(CIM_StatisticalData), /* size */
    MI_T("CIM_ManagedElement"), /* superClass */
    &CIM_ManagedElement_rtti, /* superClassDecl */
    CIM_StatisticalData_meths, /* methods */
    MI_COUNT(CIM_StatisticalData_meths), /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** Docker_ContainerStatistics
**
**==============================================================================
*/

/* property Docker_ContainerStatistics.updatetime */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_updatetime_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0075650A, /* code */
    MI_T("updatetime"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, updatetime), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.NetRXBytes */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_NetRXBytes_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E730A, /* code */
    MI_T("NetRXBytes"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, NetRXBytes), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.NetTXBytes */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_NetTXBytes_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E730A, /* code */
    MI_T("NetTXBytes"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, NetTXBytes), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.NetBytes */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_NetBytes_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E7308, /* code */
    MI_T("NetBytes"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, NetBytes), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.NetRXKBytesPerSec */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_NetRXKBytesPerSec_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E6311, /* code */
    MI_T("NetRXKBytesPerSec"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, NetRXKBytesPerSec), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.NetTXKBytesPerSec */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_NetTXKBytesPerSec_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E6311, /* code */
    MI_T("NetTXKBytesPerSec"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, NetTXKBytesPerSec), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemCacheMB */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemCacheMB_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D620A, /* code */
    MI_T("MemCacheMB"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemCacheMB), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemRSSMB */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemRSSMB_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D6208, /* code */
    MI_T("MemRSSMB"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemRSSMB), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemPGFault */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemPGFault_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D740A, /* code */
    MI_T("MemPGFault"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemPGFault), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemPGMajFault */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemPGMajFault_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D740D, /* code */
    MI_T("MemPGMajFault"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemPGMajFault), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemPGFaultPerSec */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemPGFaultPerSec_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D6310, /* code */
    MI_T("MemPGFaultPerSec"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemPGFaultPerSec), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemPGMajFaultPerSec */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemPGMajFaultPerSec_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D6313, /* code */
    MI_T("MemPGMajFaultPerSec"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemPGMajFaultPerSec), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemSwapMB */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemSwapMB_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D6209, /* code */
    MI_T("MemSwapMB"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemSwapMB), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemUnevictableMB */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemUnevictableMB_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D6210, /* code */
    MI_T("MemUnevictableMB"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemUnevictableMB), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemLimitMB */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemLimitMB_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D620A, /* code */
    MI_T("MemLimitMB"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemLimitMB), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemSWLimitMB */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemSWLimitMB_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D620C, /* code */
    MI_T("MemSWLimitMB"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemSWLimitMB), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemUsedPct */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemUsedPct_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D740A, /* code */
    MI_T("MemUsedPct"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemUsedPct), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.MemSWUsedPct */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_MemSWUsedPct_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D740C, /* code */
    MI_T("MemSWUsedPct"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, MemSWUsedPct), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.CPUTotal */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_CPUTotal_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00636C08, /* code */
    MI_T("CPUTotal"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, CPUTotal), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.CPUSystem */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_CPUSystem_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00636D09, /* code */
    MI_T("CPUSystem"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, CPUSystem), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.CPUTotalPct */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_CPUTotalPct_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0063740B, /* code */
    MI_T("CPUTotalPct"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, CPUTotalPct), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.CPUSystemPct */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_CPUSystemPct_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0063740C, /* code */
    MI_T("CPUSystemPct"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, CPUSystemPct), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerStatistics.CPUHost */
static MI_CONST MI_PropertyDecl Docker_ContainerStatistics_CPUHost_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00637407, /* code */
    MI_T("CPUHost"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics, CPUHost), /* offset */
    MI_T("Docker_ContainerStatistics"), /* origin */
    MI_T("Docker_ContainerStatistics"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST Docker_ContainerStatistics_props[] =
{
    &CIM_StatisticalData_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_StatisticalData_ElementName_prop,
    &CIM_StatisticalData_StartStatisticTime_prop,
    &CIM_StatisticalData_StatisticTime_prop,
    &CIM_StatisticalData_SampleInterval_prop,
    &Docker_ContainerStatistics_updatetime_prop,
    &Docker_ContainerStatistics_NetRXBytes_prop,
    &Docker_ContainerStatistics_NetTXBytes_prop,
    &Docker_ContainerStatistics_NetBytes_prop,
    &Docker_ContainerStatistics_NetRXKBytesPerSec_prop,
    &Docker_ContainerStatistics_NetTXKBytesPerSec_prop,
    &Docker_ContainerStatistics_MemCacheMB_prop,
    &Docker_ContainerStatistics_MemRSSMB_prop,
    &Docker_ContainerStatistics_MemPGFault_prop,
    &Docker_ContainerStatistics_MemPGMajFault_prop,
    &Docker_ContainerStatistics_MemPGFaultPerSec_prop,
    &Docker_ContainerStatistics_MemPGMajFaultPerSec_prop,
    &Docker_ContainerStatistics_MemSwapMB_prop,
    &Docker_ContainerStatistics_MemUnevictableMB_prop,
    &Docker_ContainerStatistics_MemLimitMB_prop,
    &Docker_ContainerStatistics_MemSWLimitMB_prop,
    &Docker_ContainerStatistics_MemUsedPct_prop,
    &Docker_ContainerStatistics_MemSWUsedPct_prop,
    &Docker_ContainerStatistics_CPUTotal_prop,
    &Docker_ContainerStatistics_CPUSystem_prop,
    &Docker_ContainerStatistics_CPUTotalPct_prop,
    &Docker_ContainerStatistics_CPUSystemPct_prop,
    &Docker_ContainerStatistics_CPUHost_prop,
};

/* parameter Docker_ContainerStatistics.ResetSelectedStats(): SelectedStatistics */
static MI_CONST MI_ParameterDecl Docker_ContainerStatistics_ResetSelectedStats_SelectedStatistics_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x00737312, /* code */
    MI_T("SelectedStatistics"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics_ResetSelectedStats, SelectedStatistics), /* offset */
};

/* parameter Docker_ContainerStatistics.ResetSelectedStats(): MIReturn */
static MI_CONST MI_ParameterDecl Docker_ContainerStatistics_ResetSelectedStats_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerStatistics_ResetSelectedStats, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST Docker_ContainerStatistics_ResetSelectedStats_params[] =
{
    &Docker_ContainerStatistics_ResetSelectedStats_MIReturn_param,
    &Docker_ContainerStatistics_ResetSelectedStats_SelectedStatistics_param,
};

/* method Docker_ContainerStatistics.ResetSelectedStats() */
MI_CONST MI_MethodDecl Docker_ContainerStatistics_ResetSelectedStats_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x00727312, /* code */
    MI_T("ResetSelectedStats"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    Docker_ContainerStatistics_ResetSelectedStats_params, /* parameters */
    MI_COUNT(Docker_ContainerStatistics_ResetSelectedStats_params), /* numParameters */
    sizeof(Docker_ContainerStatistics_ResetSelectedStats), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_StatisticalData"), /* origin */
    MI_T("CIM_StatisticalData"), /* propagator */
    &schemaDecl, /* schema */
    (MI_ProviderFT_Invoke)Docker_ContainerStatistics_Invoke_ResetSelectedStats, /* method */
};

static MI_MethodDecl MI_CONST* MI_CONST Docker_ContainerStatistics_meths[] =
{
    &Docker_ContainerStatistics_ResetSelectedStats_rtti,
};

static MI_CONST MI_ProviderFT Docker_ContainerStatistics_funcs =
{
  (MI_ProviderFT_Load)Docker_ContainerStatistics_Load,
  (MI_ProviderFT_Unload)Docker_ContainerStatistics_Unload,
  (MI_ProviderFT_GetInstance)Docker_ContainerStatistics_GetInstance,
  (MI_ProviderFT_EnumerateInstances)Docker_ContainerStatistics_EnumerateInstances,
  (MI_ProviderFT_CreateInstance)Docker_ContainerStatistics_CreateInstance,
  (MI_ProviderFT_ModifyInstance)Docker_ContainerStatistics_ModifyInstance,
  (MI_ProviderFT_DeleteInstance)Docker_ContainerStatistics_DeleteInstance,
  (MI_ProviderFT_AssociatorInstances)NULL,
  (MI_ProviderFT_ReferenceInstances)NULL,
  (MI_ProviderFT_EnableIndications)NULL,
  (MI_ProviderFT_DisableIndications)NULL,
  (MI_ProviderFT_Subscribe)NULL,
  (MI_ProviderFT_Unsubscribe)NULL,
  (MI_ProviderFT_Invoke)NULL,
};

static MI_CONST MI_Char* Docker_ContainerStatistics_UMLPackagePath_qual_value = MI_T("CIM::Core::Statistics");

static MI_CONST MI_Qualifier Docker_ContainerStatistics_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &Docker_ContainerStatistics_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* Docker_ContainerStatistics_Version_qual_value = MI_T("1.0.0");

static MI_CONST MI_Qualifier Docker_ContainerStatistics_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &Docker_ContainerStatistics_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST Docker_ContainerStatistics_quals[] =
{
    &Docker_ContainerStatistics_UMLPackagePath_qual,
    &Docker_ContainerStatistics_Version_qual,
};

/* class Docker_ContainerStatistics */
MI_CONST MI_ClassDecl Docker_ContainerStatistics_rtti =
{
    MI_FLAG_CLASS, /* flags */
    0x0064731A, /* code */
    MI_T("Docker_ContainerStatistics"), /* name */
    Docker_ContainerStatistics_quals, /* qualifiers */
    MI_COUNT(Docker_ContainerStatistics_quals), /* numQualifiers */
    Docker_ContainerStatistics_props, /* properties */
    MI_COUNT(Docker_ContainerStatistics_props), /* numProperties */
    sizeof(Docker_ContainerStatistics), /* size */
    MI_T("CIM_StatisticalData"), /* superClass */
    &CIM_StatisticalData_rtti, /* superClassDecl */
    Docker_ContainerStatistics_meths, /* methods */
    MI_COUNT(Docker_ContainerStatistics_meths), /* numMethods */
    &schemaDecl, /* schema */
    &Docker_ContainerStatistics_funcs, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_Collection
**
**==============================================================================
*/

static MI_PropertyDecl MI_CONST* MI_CONST CIM_Collection_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
};

static MI_CONST MI_Char* CIM_Collection_UMLPackagePath_qual_value = MI_T("CIM::Core::Collection");

static MI_CONST MI_Qualifier CIM_Collection_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_Collection_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_Collection_Version_qual_value = MI_T("2.6.0");

static MI_CONST MI_Qualifier CIM_Collection_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_Collection_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_Collection_quals[] =
{
    &CIM_Collection_UMLPackagePath_qual,
    &CIM_Collection_Version_qual,
};

/* class CIM_Collection */
MI_CONST MI_ClassDecl CIM_Collection_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_ABSTRACT, /* flags */
    0x00636E0E, /* code */
    MI_T("CIM_Collection"), /* name */
    CIM_Collection_quals, /* qualifiers */
    MI_COUNT(CIM_Collection_quals), /* numQualifiers */
    CIM_Collection_props, /* properties */
    MI_COUNT(CIM_Collection_props), /* numProperties */
    sizeof(CIM_Collection), /* size */
    MI_T("CIM_ManagedElement"), /* superClass */
    &CIM_ManagedElement_rtti, /* superClassDecl */
    NULL, /* methods */
    0, /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_InstalledProduct
**
**==============================================================================
*/

static MI_CONST MI_Uint32 CIM_InstalledProduct_ProductIdentifyingNumber_MaxLen_qual_value = 64U;

static MI_CONST MI_Qualifier CIM_InstalledProduct_ProductIdentifyingNumber_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_InstalledProduct_ProductIdentifyingNumber_MaxLen_qual_value
};

static MI_CONST MI_Char* CIM_InstalledProduct_ProductIdentifyingNumber_Propagated_qual_value = MI_T("CIM_Product.IdentifyingNumber");

static MI_CONST MI_Qualifier CIM_InstalledProduct_ProductIdentifyingNumber_Propagated_qual =
{
    MI_T("Propagated"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_InstalledProduct_ProductIdentifyingNumber_Propagated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_InstalledProduct_ProductIdentifyingNumber_quals[] =
{
    &CIM_InstalledProduct_ProductIdentifyingNumber_MaxLen_qual,
    &CIM_InstalledProduct_ProductIdentifyingNumber_Propagated_qual,
};

/* property CIM_InstalledProduct.ProductIdentifyingNumber */
static MI_CONST MI_PropertyDecl CIM_InstalledProduct_ProductIdentifyingNumber_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x00707218, /* code */
    MI_T("ProductIdentifyingNumber"), /* name */
    CIM_InstalledProduct_ProductIdentifyingNumber_quals, /* qualifiers */
    MI_COUNT(CIM_InstalledProduct_ProductIdentifyingNumber_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_InstalledProduct, ProductIdentifyingNumber), /* offset */
    MI_T("CIM_InstalledProduct"), /* origin */
    MI_T("CIM_InstalledProduct"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_InstalledProduct_ProductName_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_InstalledProduct_ProductName_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_InstalledProduct_ProductName_MaxLen_qual_value
};

static MI_CONST MI_Char* CIM_InstalledProduct_ProductName_Propagated_qual_value = MI_T("CIM_Product.Name");

static MI_CONST MI_Qualifier CIM_InstalledProduct_ProductName_Propagated_qual =
{
    MI_T("Propagated"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_InstalledProduct_ProductName_Propagated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_InstalledProduct_ProductName_quals[] =
{
    &CIM_InstalledProduct_ProductName_MaxLen_qual,
    &CIM_InstalledProduct_ProductName_Propagated_qual,
};

/* property CIM_InstalledProduct.ProductName */
static MI_CONST MI_PropertyDecl CIM_InstalledProduct_ProductName_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x0070650B, /* code */
    MI_T("ProductName"), /* name */
    CIM_InstalledProduct_ProductName_quals, /* qualifiers */
    MI_COUNT(CIM_InstalledProduct_ProductName_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_InstalledProduct, ProductName), /* offset */
    MI_T("CIM_InstalledProduct"), /* origin */
    MI_T("CIM_InstalledProduct"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_InstalledProduct_ProductVendor_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_InstalledProduct_ProductVendor_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_InstalledProduct_ProductVendor_MaxLen_qual_value
};

static MI_CONST MI_Char* CIM_InstalledProduct_ProductVendor_Propagated_qual_value = MI_T("CIM_Product.Vendor");

static MI_CONST MI_Qualifier CIM_InstalledProduct_ProductVendor_Propagated_qual =
{
    MI_T("Propagated"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_InstalledProduct_ProductVendor_Propagated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_InstalledProduct_ProductVendor_quals[] =
{
    &CIM_InstalledProduct_ProductVendor_MaxLen_qual,
    &CIM_InstalledProduct_ProductVendor_Propagated_qual,
};

/* property CIM_InstalledProduct.ProductVendor */
static MI_CONST MI_PropertyDecl CIM_InstalledProduct_ProductVendor_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x0070720D, /* code */
    MI_T("ProductVendor"), /* name */
    CIM_InstalledProduct_ProductVendor_quals, /* qualifiers */
    MI_COUNT(CIM_InstalledProduct_ProductVendor_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_InstalledProduct, ProductVendor), /* offset */
    MI_T("CIM_InstalledProduct"), /* origin */
    MI_T("CIM_InstalledProduct"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_InstalledProduct_ProductVersion_MaxLen_qual_value = 64U;

static MI_CONST MI_Qualifier CIM_InstalledProduct_ProductVersion_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_InstalledProduct_ProductVersion_MaxLen_qual_value
};

static MI_CONST MI_Char* CIM_InstalledProduct_ProductVersion_Propagated_qual_value = MI_T("CIM_Product.Version");

static MI_CONST MI_Qualifier CIM_InstalledProduct_ProductVersion_Propagated_qual =
{
    MI_T("Propagated"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_InstalledProduct_ProductVersion_Propagated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_InstalledProduct_ProductVersion_quals[] =
{
    &CIM_InstalledProduct_ProductVersion_MaxLen_qual,
    &CIM_InstalledProduct_ProductVersion_Propagated_qual,
};

/* property CIM_InstalledProduct.ProductVersion */
static MI_CONST MI_PropertyDecl CIM_InstalledProduct_ProductVersion_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x00706E0E, /* code */
    MI_T("ProductVersion"), /* name */
    CIM_InstalledProduct_ProductVersion_quals, /* qualifiers */
    MI_COUNT(CIM_InstalledProduct_ProductVersion_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_InstalledProduct, ProductVersion), /* offset */
    MI_T("CIM_InstalledProduct"), /* origin */
    MI_T("CIM_InstalledProduct"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_InstalledProduct_SystemID_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_InstalledProduct_SystemID_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_InstalledProduct_SystemID_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_InstalledProduct_SystemID_quals[] =
{
    &CIM_InstalledProduct_SystemID_MaxLen_qual,
};

/* property CIM_InstalledProduct.SystemID */
static MI_CONST MI_PropertyDecl CIM_InstalledProduct_SystemID_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x00736408, /* code */
    MI_T("SystemID"), /* name */
    CIM_InstalledProduct_SystemID_quals, /* qualifiers */
    MI_COUNT(CIM_InstalledProduct_SystemID_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_InstalledProduct, SystemID), /* offset */
    MI_T("CIM_InstalledProduct"), /* origin */
    MI_T("CIM_InstalledProduct"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_InstalledProduct_CollectionID_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_InstalledProduct_CollectionID_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_InstalledProduct_CollectionID_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_InstalledProduct_CollectionID_quals[] =
{
    &CIM_InstalledProduct_CollectionID_MaxLen_qual,
};

/* property CIM_InstalledProduct.CollectionID */
static MI_CONST MI_PropertyDecl CIM_InstalledProduct_CollectionID_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x0063640C, /* code */
    MI_T("CollectionID"), /* name */
    CIM_InstalledProduct_CollectionID_quals, /* qualifiers */
    MI_COUNT(CIM_InstalledProduct_CollectionID_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_InstalledProduct, CollectionID), /* offset */
    MI_T("CIM_InstalledProduct"), /* origin */
    MI_T("CIM_InstalledProduct"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_InstalledProduct_Name_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_InstalledProduct_Name_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_InstalledProduct_Name_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_InstalledProduct_Name_quals[] =
{
    &CIM_InstalledProduct_Name_MaxLen_qual,
};

/* property CIM_InstalledProduct.Name */
static MI_CONST MI_PropertyDecl CIM_InstalledProduct_Name_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E6504, /* code */
    MI_T("Name"), /* name */
    CIM_InstalledProduct_Name_quals, /* qualifiers */
    MI_COUNT(CIM_InstalledProduct_Name_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_InstalledProduct, Name), /* offset */
    MI_T("CIM_InstalledProduct"), /* origin */
    MI_T("CIM_InstalledProduct"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_InstalledProduct_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_InstalledProduct_ProductIdentifyingNumber_prop,
    &CIM_InstalledProduct_ProductName_prop,
    &CIM_InstalledProduct_ProductVendor_prop,
    &CIM_InstalledProduct_ProductVersion_prop,
    &CIM_InstalledProduct_SystemID_prop,
    &CIM_InstalledProduct_CollectionID_prop,
    &CIM_InstalledProduct_Name_prop,
};

static MI_CONST MI_Char* CIM_InstalledProduct_UMLPackagePath_qual_value = MI_T("CIM::Application::InstalledProduct");

static MI_CONST MI_Qualifier CIM_InstalledProduct_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_InstalledProduct_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_InstalledProduct_Version_qual_value = MI_T("2.6.0");

static MI_CONST MI_Qualifier CIM_InstalledProduct_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_InstalledProduct_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_InstalledProduct_quals[] =
{
    &CIM_InstalledProduct_UMLPackagePath_qual,
    &CIM_InstalledProduct_Version_qual,
};

/* class CIM_InstalledProduct */
MI_CONST MI_ClassDecl CIM_InstalledProduct_rtti =
{
    MI_FLAG_CLASS, /* flags */
    0x00637414, /* code */
    MI_T("CIM_InstalledProduct"), /* name */
    CIM_InstalledProduct_quals, /* qualifiers */
    MI_COUNT(CIM_InstalledProduct_quals), /* numQualifiers */
    CIM_InstalledProduct_props, /* properties */
    MI_COUNT(CIM_InstalledProduct_props), /* numProperties */
    sizeof(CIM_InstalledProduct), /* size */
    MI_T("CIM_Collection"), /* superClass */
    &CIM_Collection_rtti, /* superClassDecl */
    NULL, /* methods */
    0, /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** Docker_Server
**
**==============================================================================
*/

/* property Docker_Server.Containers */
static MI_CONST MI_PropertyDecl Docker_Server_Containers_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0063730A, /* code */
    MI_T("Containers"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, Containers), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.DockerRootDir */
static MI_CONST MI_PropertyDecl Docker_Server_DockerRootDir_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0064720D, /* code */
    MI_T("DockerRootDir"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, DockerRootDir), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.Hostname */
static MI_CONST MI_PropertyDecl Docker_Server_Hostname_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00686508, /* code */
    MI_T("Hostname"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, Hostname), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.Driver */
static MI_CONST MI_PropertyDecl Docker_Server_Driver_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00647206, /* code */
    MI_T("Driver"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, Driver), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.DriverStatus */
static MI_CONST MI_PropertyDecl Docker_Server_DriverStatus_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0064730C, /* code */
    MI_T("DriverStatus"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, DriverStatus), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.Images */
static MI_CONST MI_PropertyDecl Docker_Server_Images_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00697306, /* code */
    MI_T("Images"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, Images), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.InitPath */
static MI_CONST MI_PropertyDecl Docker_Server_InitPath_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00696808, /* code */
    MI_T("InitPath"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, InitPath), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.KernelVersion */
static MI_CONST MI_PropertyDecl Docker_Server_KernelVersion_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006B6E0D, /* code */
    MI_T("KernelVersion"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, KernelVersion), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.OperatingStatus */
static MI_CONST MI_PropertyDecl Docker_Server_OperatingStatus_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F730F, /* code */
    MI_T("OperatingStatus"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, OperatingStatus), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.MemTotal */
static MI_CONST MI_PropertyDecl Docker_Server_MemTotal_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D6C08, /* code */
    MI_T("MemTotal"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, MemTotal), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.MemLimit */
static MI_CONST MI_PropertyDecl Docker_Server_MemLimit_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D7408, /* code */
    MI_T("MemLimit"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, MemLimit), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.SwapLimit */
static MI_CONST MI_PropertyDecl Docker_Server_SwapLimit_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00737409, /* code */
    MI_T("SwapLimit"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, SwapLimit), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

/* property Docker_Server.NCPU */
static MI_CONST MI_PropertyDecl Docker_Server_NCPU_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E7504, /* code */
    MI_T("NCPU"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Server, NCPU), /* offset */
    MI_T("Docker_Server"), /* origin */
    MI_T("Docker_Server"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST Docker_Server_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_InstalledProduct_ProductIdentifyingNumber_prop,
    &CIM_InstalledProduct_ProductName_prop,
    &CIM_InstalledProduct_ProductVendor_prop,
    &CIM_InstalledProduct_ProductVersion_prop,
    &CIM_InstalledProduct_SystemID_prop,
    &CIM_InstalledProduct_CollectionID_prop,
    &CIM_InstalledProduct_Name_prop,
    &Docker_Server_Containers_prop,
    &Docker_Server_DockerRootDir_prop,
    &Docker_Server_Hostname_prop,
    &Docker_Server_Driver_prop,
    &Docker_Server_DriverStatus_prop,
    &Docker_Server_Images_prop,
    &Docker_Server_InitPath_prop,
    &Docker_Server_KernelVersion_prop,
    &Docker_Server_OperatingStatus_prop,
    &Docker_Server_MemTotal_prop,
    &Docker_Server_MemLimit_prop,
    &Docker_Server_SwapLimit_prop,
    &Docker_Server_NCPU_prop,
};

static MI_CONST MI_ProviderFT Docker_Server_funcs =
{
  (MI_ProviderFT_Load)Docker_Server_Load,
  (MI_ProviderFT_Unload)Docker_Server_Unload,
  (MI_ProviderFT_GetInstance)Docker_Server_GetInstance,
  (MI_ProviderFT_EnumerateInstances)Docker_Server_EnumerateInstances,
  (MI_ProviderFT_CreateInstance)Docker_Server_CreateInstance,
  (MI_ProviderFT_ModifyInstance)Docker_Server_ModifyInstance,
  (MI_ProviderFT_DeleteInstance)Docker_Server_DeleteInstance,
  (MI_ProviderFT_AssociatorInstances)NULL,
  (MI_ProviderFT_ReferenceInstances)NULL,
  (MI_ProviderFT_EnableIndications)NULL,
  (MI_ProviderFT_DisableIndications)NULL,
  (MI_ProviderFT_Subscribe)NULL,
  (MI_ProviderFT_Unsubscribe)NULL,
  (MI_ProviderFT_Invoke)NULL,
};

static MI_CONST MI_Char* Docker_Server_UMLPackagePath_qual_value = MI_T("CIM::Application::InstalledProduct");

static MI_CONST MI_Qualifier Docker_Server_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &Docker_Server_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* Docker_Server_Version_qual_value = MI_T("1.0.0");

static MI_CONST MI_Qualifier Docker_Server_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &Docker_Server_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST Docker_Server_quals[] =
{
    &Docker_Server_UMLPackagePath_qual,
    &Docker_Server_Version_qual,
};

/* class Docker_Server */
MI_CONST MI_ClassDecl Docker_Server_rtti =
{
    MI_FLAG_CLASS, /* flags */
    0x0064720D, /* code */
    MI_T("Docker_Server"), /* name */
    Docker_Server_quals, /* qualifiers */
    MI_COUNT(Docker_Server_quals), /* numQualifiers */
    Docker_Server_props, /* properties */
    MI_COUNT(Docker_Server_props), /* numProperties */
    sizeof(Docker_Server), /* size */
    MI_T("CIM_InstalledProduct"), /* superClass */
    &CIM_InstalledProduct_rtti, /* superClassDecl */
    NULL, /* methods */
    0, /* numMethods */
    &schemaDecl, /* schema */
    &Docker_Server_funcs, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** Docker_ContainerProcessorStatistics
**
**==============================================================================
*/

/* property Docker_ContainerProcessorStatistics.ProcessorID */
static MI_CONST MI_PropertyDecl Docker_ContainerProcessorStatistics_ProcessorID_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0070640B, /* code */
    MI_T("ProcessorID"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerProcessorStatistics, ProcessorID), /* offset */
    MI_T("Docker_ContainerProcessorStatistics"), /* origin */
    MI_T("Docker_ContainerProcessorStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerProcessorStatistics.CPUTotal */
static MI_CONST MI_PropertyDecl Docker_ContainerProcessorStatistics_CPUTotal_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00636C08, /* code */
    MI_T("CPUTotal"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT64, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerProcessorStatistics, CPUTotal), /* offset */
    MI_T("Docker_ContainerProcessorStatistics"), /* origin */
    MI_T("Docker_ContainerProcessorStatistics"), /* propagator */
    NULL,
};

/* property Docker_ContainerProcessorStatistics.CPUTotalPct */
static MI_CONST MI_PropertyDecl Docker_ContainerProcessorStatistics_CPUTotalPct_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0063740B, /* code */
    MI_T("CPUTotalPct"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerProcessorStatistics, CPUTotalPct), /* offset */
    MI_T("Docker_ContainerProcessorStatistics"), /* origin */
    MI_T("Docker_ContainerProcessorStatistics"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST Docker_ContainerProcessorStatistics_props[] =
{
    &CIM_StatisticalData_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_StatisticalData_ElementName_prop,
    &CIM_StatisticalData_StartStatisticTime_prop,
    &CIM_StatisticalData_StatisticTime_prop,
    &CIM_StatisticalData_SampleInterval_prop,
    &Docker_ContainerProcessorStatistics_ProcessorID_prop,
    &Docker_ContainerProcessorStatistics_CPUTotal_prop,
    &Docker_ContainerProcessorStatistics_CPUTotalPct_prop,
};

/* parameter Docker_ContainerProcessorStatistics.ResetSelectedStats(): SelectedStatistics */
static MI_CONST MI_ParameterDecl Docker_ContainerProcessorStatistics_ResetSelectedStats_SelectedStatistics_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x00737312, /* code */
    MI_T("SelectedStatistics"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerProcessorStatistics_ResetSelectedStats, SelectedStatistics), /* offset */
};

/* parameter Docker_ContainerProcessorStatistics.ResetSelectedStats(): MIReturn */
static MI_CONST MI_ParameterDecl Docker_ContainerProcessorStatistics_ResetSelectedStats_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_ContainerProcessorStatistics_ResetSelectedStats, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST Docker_ContainerProcessorStatistics_ResetSelectedStats_params[] =
{
    &Docker_ContainerProcessorStatistics_ResetSelectedStats_MIReturn_param,
    &Docker_ContainerProcessorStatistics_ResetSelectedStats_SelectedStatistics_param,
};

/* method Docker_ContainerProcessorStatistics.ResetSelectedStats() */
MI_CONST MI_MethodDecl Docker_ContainerProcessorStatistics_ResetSelectedStats_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x00727312, /* code */
    MI_T("ResetSelectedStats"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    Docker_ContainerProcessorStatistics_ResetSelectedStats_params, /* parameters */
    MI_COUNT(Docker_ContainerProcessorStatistics_ResetSelectedStats_params), /* numParameters */
    sizeof(Docker_ContainerProcessorStatistics_ResetSelectedStats), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_StatisticalData"), /* origin */
    MI_T("CIM_StatisticalData"), /* propagator */
    &schemaDecl, /* schema */
    (MI_ProviderFT_Invoke)Docker_ContainerProcessorStatistics_Invoke_ResetSelectedStats, /* method */
};

static MI_MethodDecl MI_CONST* MI_CONST Docker_ContainerProcessorStatistics_meths[] =
{
    &Docker_ContainerProcessorStatistics_ResetSelectedStats_rtti,
};

static MI_CONST MI_ProviderFT Docker_ContainerProcessorStatistics_funcs =
{
  (MI_ProviderFT_Load)Docker_ContainerProcessorStatistics_Load,
  (MI_ProviderFT_Unload)Docker_ContainerProcessorStatistics_Unload,
  (MI_ProviderFT_GetInstance)Docker_ContainerProcessorStatistics_GetInstance,
  (MI_ProviderFT_EnumerateInstances)Docker_ContainerProcessorStatistics_EnumerateInstances,
  (MI_ProviderFT_CreateInstance)Docker_ContainerProcessorStatistics_CreateInstance,
  (MI_ProviderFT_ModifyInstance)Docker_ContainerProcessorStatistics_ModifyInstance,
  (MI_ProviderFT_DeleteInstance)Docker_ContainerProcessorStatistics_DeleteInstance,
  (MI_ProviderFT_AssociatorInstances)NULL,
  (MI_ProviderFT_ReferenceInstances)NULL,
  (MI_ProviderFT_EnableIndications)NULL,
  (MI_ProviderFT_DisableIndications)NULL,
  (MI_ProviderFT_Subscribe)NULL,
  (MI_ProviderFT_Unsubscribe)NULL,
  (MI_ProviderFT_Invoke)NULL,
};

static MI_CONST MI_Char* Docker_ContainerProcessorStatistics_UMLPackagePath_qual_value = MI_T("CIM::Core::Statistics");

static MI_CONST MI_Qualifier Docker_ContainerProcessorStatistics_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &Docker_ContainerProcessorStatistics_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* Docker_ContainerProcessorStatistics_Version_qual_value = MI_T("1.0.0");

static MI_CONST MI_Qualifier Docker_ContainerProcessorStatistics_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &Docker_ContainerProcessorStatistics_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST Docker_ContainerProcessorStatistics_quals[] =
{
    &Docker_ContainerProcessorStatistics_UMLPackagePath_qual,
    &Docker_ContainerProcessorStatistics_Version_qual,
};

/* class Docker_ContainerProcessorStatistics */
MI_CONST MI_ClassDecl Docker_ContainerProcessorStatistics_rtti =
{
    MI_FLAG_CLASS, /* flags */
    0x00647323, /* code */
    MI_T("Docker_ContainerProcessorStatistics"), /* name */
    Docker_ContainerProcessorStatistics_quals, /* qualifiers */
    MI_COUNT(Docker_ContainerProcessorStatistics_quals), /* numQualifiers */
    Docker_ContainerProcessorStatistics_props, /* properties */
    MI_COUNT(Docker_ContainerProcessorStatistics_props), /* numProperties */
    sizeof(Docker_ContainerProcessorStatistics), /* size */
    MI_T("CIM_StatisticalData"), /* superClass */
    &CIM_StatisticalData_rtti, /* superClassDecl */
    Docker_ContainerProcessorStatistics_meths, /* methods */
    MI_COUNT(Docker_ContainerProcessorStatistics_meths), /* numMethods */
    &schemaDecl, /* schema */
    &Docker_ContainerProcessorStatistics_funcs, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_ManagedSystemElement
**
**==============================================================================
*/

/* property CIM_ManagedSystemElement.InstallDate */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_InstallDate_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0069650B, /* code */
    MI_T("InstallDate"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, InstallDate), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_ManagedSystemElement_Name_MaxLen_qual_value = 1024U;

static MI_CONST MI_Qualifier CIM_ManagedSystemElement_Name_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_ManagedSystemElement_Name_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ManagedSystemElement_Name_quals[] =
{
    &CIM_ManagedSystemElement_Name_MaxLen_qual,
};

/* property CIM_ManagedSystemElement.Name */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_Name_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E6504, /* code */
    MI_T("Name"), /* name */
    CIM_ManagedSystemElement_Name_quals, /* qualifiers */
    MI_COUNT(CIM_ManagedSystemElement_Name_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, Name), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_ManagedSystemElement_OperationalStatus_ArrayType_qual_value = MI_T("Indexed");

static MI_CONST MI_Qualifier CIM_ManagedSystemElement_OperationalStatus_ArrayType_qual =
{
    MI_T("ArrayType"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_ManagedSystemElement_OperationalStatus_ArrayType_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ManagedSystemElement_OperationalStatus_quals[] =
{
    &CIM_ManagedSystemElement_OperationalStatus_ArrayType_qual,
};

/* property CIM_ManagedSystemElement.OperationalStatus */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_OperationalStatus_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F7311, /* code */
    MI_T("OperationalStatus"), /* name */
    CIM_ManagedSystemElement_OperationalStatus_quals, /* qualifiers */
    MI_COUNT(CIM_ManagedSystemElement_OperationalStatus_quals), /* numQualifiers */
    MI_UINT16A, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, OperationalStatus), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_ManagedSystemElement_StatusDescriptions_ArrayType_qual_value = MI_T("Indexed");

static MI_CONST MI_Qualifier CIM_ManagedSystemElement_StatusDescriptions_ArrayType_qual =
{
    MI_T("ArrayType"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_ManagedSystemElement_StatusDescriptions_ArrayType_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ManagedSystemElement_StatusDescriptions_quals[] =
{
    &CIM_ManagedSystemElement_StatusDescriptions_ArrayType_qual,
};

/* property CIM_ManagedSystemElement.StatusDescriptions */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_StatusDescriptions_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00737312, /* code */
    MI_T("StatusDescriptions"), /* name */
    CIM_ManagedSystemElement_StatusDescriptions_quals, /* qualifiers */
    MI_COUNT(CIM_ManagedSystemElement_StatusDescriptions_quals), /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, StatusDescriptions), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_ManagedSystemElement_Status_Deprecated_qual_data_value[] =
{
    MI_T("CIM_ManagedSystemElement.OperationalStatus"),
};

static MI_CONST MI_ConstStringA CIM_ManagedSystemElement_Status_Deprecated_qual_value =
{
    CIM_ManagedSystemElement_Status_Deprecated_qual_data_value,
    MI_COUNT(CIM_ManagedSystemElement_Status_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_ManagedSystemElement_Status_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ManagedSystemElement_Status_Deprecated_qual_value
};

static MI_CONST MI_Uint32 CIM_ManagedSystemElement_Status_MaxLen_qual_value = 10U;

static MI_CONST MI_Qualifier CIM_ManagedSystemElement_Status_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_ManagedSystemElement_Status_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ManagedSystemElement_Status_quals[] =
{
    &CIM_ManagedSystemElement_Status_Deprecated_qual,
    &CIM_ManagedSystemElement_Status_MaxLen_qual,
};

/* property CIM_ManagedSystemElement.Status */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_Status_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00737306, /* code */
    MI_T("Status"), /* name */
    CIM_ManagedSystemElement_Status_quals, /* qualifiers */
    MI_COUNT(CIM_ManagedSystemElement_Status_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, Status), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

/* property CIM_ManagedSystemElement.HealthState */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_HealthState_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0068650B, /* code */
    MI_T("HealthState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, HealthState), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

/* property CIM_ManagedSystemElement.CommunicationStatus */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_CommunicationStatus_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00637313, /* code */
    MI_T("CommunicationStatus"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, CommunicationStatus), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

/* property CIM_ManagedSystemElement.DetailedStatus */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_DetailedStatus_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0064730E, /* code */
    MI_T("DetailedStatus"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, DetailedStatus), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

/* property CIM_ManagedSystemElement.OperatingStatus */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_OperatingStatus_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F730F, /* code */
    MI_T("OperatingStatus"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, OperatingStatus), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

/* property CIM_ManagedSystemElement.PrimaryStatus */
static MI_CONST MI_PropertyDecl CIM_ManagedSystemElement_PrimaryStatus_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0070730D, /* code */
    MI_T("PrimaryStatus"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ManagedSystemElement, PrimaryStatus), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ManagedSystemElement"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_ManagedSystemElement_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_ManagedSystemElement_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
};

static MI_CONST MI_Char* CIM_ManagedSystemElement_UMLPackagePath_qual_value = MI_T("CIM::Core::CoreElements");

static MI_CONST MI_Qualifier CIM_ManagedSystemElement_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_ManagedSystemElement_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_ManagedSystemElement_Version_qual_value = MI_T("2.28.0");

static MI_CONST MI_Qualifier CIM_ManagedSystemElement_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_ManagedSystemElement_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ManagedSystemElement_quals[] =
{
    &CIM_ManagedSystemElement_UMLPackagePath_qual,
    &CIM_ManagedSystemElement_Version_qual,
};

/* class CIM_ManagedSystemElement */
MI_CONST MI_ClassDecl CIM_ManagedSystemElement_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_ABSTRACT, /* flags */
    0x00637418, /* code */
    MI_T("CIM_ManagedSystemElement"), /* name */
    CIM_ManagedSystemElement_quals, /* qualifiers */
    MI_COUNT(CIM_ManagedSystemElement_quals), /* numQualifiers */
    CIM_ManagedSystemElement_props, /* properties */
    MI_COUNT(CIM_ManagedSystemElement_props), /* numProperties */
    sizeof(CIM_ManagedSystemElement), /* size */
    MI_T("CIM_ManagedElement"), /* superClass */
    &CIM_ManagedElement_rtti, /* superClassDecl */
    NULL, /* methods */
    0, /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_LogicalElement
**
**==============================================================================
*/

static MI_PropertyDecl MI_CONST* MI_CONST CIM_LogicalElement_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_ManagedSystemElement_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
};

static MI_CONST MI_Char* CIM_LogicalElement_UMLPackagePath_qual_value = MI_T("CIM::Core::CoreElements");

static MI_CONST MI_Qualifier CIM_LogicalElement_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_LogicalElement_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_LogicalElement_Version_qual_value = MI_T("2.6.0");

static MI_CONST MI_Qualifier CIM_LogicalElement_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_LogicalElement_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_LogicalElement_quals[] =
{
    &CIM_LogicalElement_UMLPackagePath_qual,
    &CIM_LogicalElement_Version_qual,
};

/* class CIM_LogicalElement */
MI_CONST MI_ClassDecl CIM_LogicalElement_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_ABSTRACT, /* flags */
    0x00637412, /* code */
    MI_T("CIM_LogicalElement"), /* name */
    CIM_LogicalElement_quals, /* qualifiers */
    MI_COUNT(CIM_LogicalElement_quals), /* numQualifiers */
    CIM_LogicalElement_props, /* properties */
    MI_COUNT(CIM_LogicalElement_props), /* numProperties */
    sizeof(CIM_LogicalElement), /* size */
    MI_T("CIM_ManagedSystemElement"), /* superClass */
    &CIM_ManagedSystemElement_rtti, /* superClassDecl */
    NULL, /* methods */
    0, /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_Job
**
**==============================================================================
*/

/* property CIM_Job.JobStatus */
static MI_CONST MI_PropertyDecl CIM_Job_JobStatus_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006A7309, /* code */
    MI_T("JobStatus"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, JobStatus), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.TimeSubmitted */
static MI_CONST MI_PropertyDecl CIM_Job_TimeSubmitted_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0074640D, /* code */
    MI_T("TimeSubmitted"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, TimeSubmitted), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_Job_ScheduledStartTime_Deprecated_qual_data_value[] =
{
    MI_T("CIM_Job.RunMonth"),
    MI_T("CIM_Job.RunDay"),
    MI_T("CIM_Job.RunDayOfWeek"),
    MI_T("CIM_Job.RunStartInterval"),
};

static MI_CONST MI_ConstStringA CIM_Job_ScheduledStartTime_Deprecated_qual_value =
{
    CIM_Job_ScheduledStartTime_Deprecated_qual_data_value,
    MI_COUNT(CIM_Job_ScheduledStartTime_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_Job_ScheduledStartTime_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_Job_ScheduledStartTime_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_Job_ScheduledStartTime_quals[] =
{
    &CIM_Job_ScheduledStartTime_Deprecated_qual,
};

/* property CIM_Job.ScheduledStartTime */
static MI_CONST MI_PropertyDecl CIM_Job_ScheduledStartTime_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00736512, /* code */
    MI_T("ScheduledStartTime"), /* name */
    CIM_Job_ScheduledStartTime_quals, /* qualifiers */
    MI_COUNT(CIM_Job_ScheduledStartTime_quals), /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, ScheduledStartTime), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.StartTime */
static MI_CONST MI_PropertyDecl CIM_Job_StartTime_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00736509, /* code */
    MI_T("StartTime"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, StartTime), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.ElapsedTime */
static MI_CONST MI_PropertyDecl CIM_Job_ElapsedTime_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0065650B, /* code */
    MI_T("ElapsedTime"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, ElapsedTime), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_Job_JobRunTimes_value = 1U;

/* property CIM_Job.JobRunTimes */
static MI_CONST MI_PropertyDecl CIM_Job_JobRunTimes_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006A730B, /* code */
    MI_T("JobRunTimes"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, JobRunTimes), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    &CIM_Job_JobRunTimes_value,
};

/* property CIM_Job.RunMonth */
static MI_CONST MI_PropertyDecl CIM_Job_RunMonth_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00726808, /* code */
    MI_T("RunMonth"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT8, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, RunMonth), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

static MI_CONST MI_Sint64 CIM_Job_RunDay_MinValue_qual_value = -MI_LL(31);

static MI_CONST MI_Qualifier CIM_Job_RunDay_MinValue_qual =
{
    MI_T("MinValue"),
    MI_SINT64,
    0,
    &CIM_Job_RunDay_MinValue_qual_value
};

static MI_CONST MI_Sint64 CIM_Job_RunDay_MaxValue_qual_value = MI_LL(31);

static MI_CONST MI_Qualifier CIM_Job_RunDay_MaxValue_qual =
{
    MI_T("MaxValue"),
    MI_SINT64,
    0,
    &CIM_Job_RunDay_MaxValue_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_Job_RunDay_quals[] =
{
    &CIM_Job_RunDay_MinValue_qual,
    &CIM_Job_RunDay_MaxValue_qual,
};

/* property CIM_Job.RunDay */
static MI_CONST MI_PropertyDecl CIM_Job_RunDay_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00727906, /* code */
    MI_T("RunDay"), /* name */
    CIM_Job_RunDay_quals, /* qualifiers */
    MI_COUNT(CIM_Job_RunDay_quals), /* numQualifiers */
    MI_SINT8, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, RunDay), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.RunDayOfWeek */
static MI_CONST MI_PropertyDecl CIM_Job_RunDayOfWeek_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00726B0C, /* code */
    MI_T("RunDayOfWeek"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_SINT8, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, RunDayOfWeek), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.RunStartInterval */
static MI_CONST MI_PropertyDecl CIM_Job_RunStartInterval_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00726C10, /* code */
    MI_T("RunStartInterval"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, RunStartInterval), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.LocalOrUtcTime */
static MI_CONST MI_PropertyDecl CIM_Job_LocalOrUtcTime_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006C650E, /* code */
    MI_T("LocalOrUtcTime"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, LocalOrUtcTime), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.UntilTime */
static MI_CONST MI_PropertyDecl CIM_Job_UntilTime_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00756509, /* code */
    MI_T("UntilTime"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, UntilTime), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.Notify */
static MI_CONST MI_PropertyDecl CIM_Job_Notify_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E7906, /* code */
    MI_T("Notify"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, Notify), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.Owner */
static MI_CONST MI_PropertyDecl CIM_Job_Owner_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F7205, /* code */
    MI_T("Owner"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, Owner), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.Priority */
static MI_CONST MI_PropertyDecl CIM_Job_Priority_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00707908, /* code */
    MI_T("Priority"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, Priority), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_Job_PercentComplete_Units_qual_value = MI_T("Percent");

static MI_CONST MI_Qualifier CIM_Job_PercentComplete_Units_qual =
{
    MI_T("Units"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TOSUBCLASS|MI_FLAG_TRANSLATABLE,
    &CIM_Job_PercentComplete_Units_qual_value
};

static MI_CONST MI_Sint64 CIM_Job_PercentComplete_MinValue_qual_value = MI_LL(0);

static MI_CONST MI_Qualifier CIM_Job_PercentComplete_MinValue_qual =
{
    MI_T("MinValue"),
    MI_SINT64,
    0,
    &CIM_Job_PercentComplete_MinValue_qual_value
};

static MI_CONST MI_Sint64 CIM_Job_PercentComplete_MaxValue_qual_value = MI_LL(101);

static MI_CONST MI_Qualifier CIM_Job_PercentComplete_MaxValue_qual =
{
    MI_T("MaxValue"),
    MI_SINT64,
    0,
    &CIM_Job_PercentComplete_MaxValue_qual_value
};

static MI_CONST MI_Char* CIM_Job_PercentComplete_PUnit_qual_value = MI_T("percent");

static MI_CONST MI_Qualifier CIM_Job_PercentComplete_PUnit_qual =
{
    MI_T("PUnit"),
    MI_STRING,
    0,
    &CIM_Job_PercentComplete_PUnit_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_Job_PercentComplete_quals[] =
{
    &CIM_Job_PercentComplete_Units_qual,
    &CIM_Job_PercentComplete_MinValue_qual,
    &CIM_Job_PercentComplete_MaxValue_qual,
    &CIM_Job_PercentComplete_PUnit_qual,
};

/* property CIM_Job.PercentComplete */
static MI_CONST MI_PropertyDecl CIM_Job_PercentComplete_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0070650F, /* code */
    MI_T("PercentComplete"), /* name */
    CIM_Job_PercentComplete_quals, /* qualifiers */
    MI_COUNT(CIM_Job_PercentComplete_quals), /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, PercentComplete), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.DeleteOnCompletion */
static MI_CONST MI_PropertyDecl CIM_Job_DeleteOnCompletion_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00646E12, /* code */
    MI_T("DeleteOnCompletion"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_BOOLEAN, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, DeleteOnCompletion), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.ErrorCode */
static MI_CONST MI_PropertyDecl CIM_Job_ErrorCode_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00656509, /* code */
    MI_T("ErrorCode"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, ErrorCode), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.ErrorDescription */
static MI_CONST MI_PropertyDecl CIM_Job_ErrorDescription_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00656E10, /* code */
    MI_T("ErrorDescription"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, ErrorDescription), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.RecoveryAction */
static MI_CONST MI_PropertyDecl CIM_Job_RecoveryAction_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00726E0E, /* code */
    MI_T("RecoveryAction"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, RecoveryAction), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

/* property CIM_Job.OtherRecoveryAction */
static MI_CONST MI_PropertyDecl CIM_Job_OtherRecoveryAction_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F6E13, /* code */
    MI_T("OtherRecoveryAction"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job, OtherRecoveryAction), /* offset */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_Job_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_ManagedSystemElement_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
    &CIM_Job_JobStatus_prop,
    &CIM_Job_TimeSubmitted_prop,
    &CIM_Job_ScheduledStartTime_prop,
    &CIM_Job_StartTime_prop,
    &CIM_Job_ElapsedTime_prop,
    &CIM_Job_JobRunTimes_prop,
    &CIM_Job_RunMonth_prop,
    &CIM_Job_RunDay_prop,
    &CIM_Job_RunDayOfWeek_prop,
    &CIM_Job_RunStartInterval_prop,
    &CIM_Job_LocalOrUtcTime_prop,
    &CIM_Job_UntilTime_prop,
    &CIM_Job_Notify_prop,
    &CIM_Job_Owner_prop,
    &CIM_Job_Priority_prop,
    &CIM_Job_PercentComplete_prop,
    &CIM_Job_DeleteOnCompletion_prop,
    &CIM_Job_ErrorCode_prop,
    &CIM_Job_ErrorDescription_prop,
    &CIM_Job_RecoveryAction_prop,
    &CIM_Job_OtherRecoveryAction_prop,
};

static MI_CONST MI_Char* CIM_Job_KillJob_Deprecated_qual_data_value[] =
{
    MI_T("CIM_ConcreteJob.RequestStateChange()"),
};

static MI_CONST MI_ConstStringA CIM_Job_KillJob_Deprecated_qual_value =
{
    CIM_Job_KillJob_Deprecated_qual_data_value,
    MI_COUNT(CIM_Job_KillJob_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_Job_KillJob_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_Job_KillJob_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_Job_KillJob_quals[] =
{
    &CIM_Job_KillJob_Deprecated_qual,
};

/* parameter CIM_Job.KillJob(): DeleteOnKill */
static MI_CONST MI_ParameterDecl CIM_Job_KillJob_DeleteOnKill_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x00646C0C, /* code */
    MI_T("DeleteOnKill"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_BOOLEAN, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job_KillJob, DeleteOnKill), /* offset */
};

static MI_CONST MI_Char* CIM_Job_KillJob_MIReturn_Deprecated_qual_data_value[] =
{
    MI_T("CIM_ConcreteJob.RequestStateChange()"),
};

static MI_CONST MI_ConstStringA CIM_Job_KillJob_MIReturn_Deprecated_qual_value =
{
    CIM_Job_KillJob_MIReturn_Deprecated_qual_data_value,
    MI_COUNT(CIM_Job_KillJob_MIReturn_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_Job_KillJob_MIReturn_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_Job_KillJob_MIReturn_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_Job_KillJob_MIReturn_quals[] =
{
    &CIM_Job_KillJob_MIReturn_Deprecated_qual,
};

/* parameter CIM_Job.KillJob(): MIReturn */
static MI_CONST MI_ParameterDecl CIM_Job_KillJob_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    CIM_Job_KillJob_MIReturn_quals, /* qualifiers */
    MI_COUNT(CIM_Job_KillJob_MIReturn_quals), /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Job_KillJob, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST CIM_Job_KillJob_params[] =
{
    &CIM_Job_KillJob_MIReturn_param,
    &CIM_Job_KillJob_DeleteOnKill_param,
};

/* method CIM_Job.KillJob() */
MI_CONST MI_MethodDecl CIM_Job_KillJob_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x006B6207, /* code */
    MI_T("KillJob"), /* name */
    CIM_Job_KillJob_quals, /* qualifiers */
    MI_COUNT(CIM_Job_KillJob_quals), /* numQualifiers */
    CIM_Job_KillJob_params, /* parameters */
    MI_COUNT(CIM_Job_KillJob_params), /* numParameters */
    sizeof(CIM_Job_KillJob), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_Job"), /* origin */
    MI_T("CIM_Job"), /* propagator */
    &schemaDecl, /* schema */
    NULL, /* method */
};

static MI_MethodDecl MI_CONST* MI_CONST CIM_Job_meths[] =
{
    &CIM_Job_KillJob_rtti,
};

static MI_CONST MI_Char* CIM_Job_UMLPackagePath_qual_value = MI_T("CIM::Core::CoreElements");

static MI_CONST MI_Qualifier CIM_Job_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_Job_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_Job_Version_qual_value = MI_T("2.10.0");

static MI_CONST MI_Qualifier CIM_Job_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_Job_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_Job_quals[] =
{
    &CIM_Job_UMLPackagePath_qual,
    &CIM_Job_Version_qual,
};

/* class CIM_Job */
MI_CONST MI_ClassDecl CIM_Job_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_ABSTRACT, /* flags */
    0x00636207, /* code */
    MI_T("CIM_Job"), /* name */
    CIM_Job_quals, /* qualifiers */
    MI_COUNT(CIM_Job_quals), /* numQualifiers */
    CIM_Job_props, /* properties */
    MI_COUNT(CIM_Job_props), /* numProperties */
    sizeof(CIM_Job), /* size */
    MI_T("CIM_LogicalElement"), /* superClass */
    &CIM_LogicalElement_rtti, /* superClassDecl */
    CIM_Job_meths, /* methods */
    MI_COUNT(CIM_Job_meths), /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_Error
**
**==============================================================================
*/

/* property CIM_Error.ErrorType */
static MI_CONST MI_PropertyDecl CIM_Error_ErrorType_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00656509, /* code */
    MI_T("ErrorType"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, ErrorType), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.OtherErrorType */
static MI_CONST MI_PropertyDecl CIM_Error_OtherErrorType_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F650E, /* code */
    MI_T("OtherErrorType"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, OtherErrorType), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.OwningEntity */
static MI_CONST MI_PropertyDecl CIM_Error_OwningEntity_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F790C, /* code */
    MI_T("OwningEntity"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, OwningEntity), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.MessageID */
static MI_CONST MI_PropertyDecl CIM_Error_MessageID_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_REQUIRED, /* flags */
    0x006D6409, /* code */
    MI_T("MessageID"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, MessageID), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.Message */
static MI_CONST MI_PropertyDecl CIM_Error_Message_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D6507, /* code */
    MI_T("Message"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, Message), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.MessageArguments */
static MI_CONST MI_PropertyDecl CIM_Error_MessageArguments_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006D7310, /* code */
    MI_T("MessageArguments"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, MessageArguments), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.PerceivedSeverity */
static MI_CONST MI_PropertyDecl CIM_Error_PerceivedSeverity_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00707911, /* code */
    MI_T("PerceivedSeverity"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, PerceivedSeverity), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.ProbableCause */
static MI_CONST MI_PropertyDecl CIM_Error_ProbableCause_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0070650D, /* code */
    MI_T("ProbableCause"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, ProbableCause), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.ProbableCauseDescription */
static MI_CONST MI_PropertyDecl CIM_Error_ProbableCauseDescription_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00706E18, /* code */
    MI_T("ProbableCauseDescription"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, ProbableCauseDescription), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.RecommendedActions */
static MI_CONST MI_PropertyDecl CIM_Error_RecommendedActions_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00727312, /* code */
    MI_T("RecommendedActions"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, RecommendedActions), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.ErrorSource */
static MI_CONST MI_PropertyDecl CIM_Error_ErrorSource_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0065650B, /* code */
    MI_T("ErrorSource"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, ErrorSource), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint16 CIM_Error_ErrorSourceFormat_value = 0;

/* property CIM_Error.ErrorSourceFormat */
static MI_CONST MI_PropertyDecl CIM_Error_ErrorSourceFormat_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00657411, /* code */
    MI_T("ErrorSourceFormat"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, ErrorSourceFormat), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    &CIM_Error_ErrorSourceFormat_value,
};

/* property CIM_Error.OtherErrorSourceFormat */
static MI_CONST MI_PropertyDecl CIM_Error_OtherErrorSourceFormat_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F7416, /* code */
    MI_T("OtherErrorSourceFormat"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, OtherErrorSourceFormat), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.CIMStatusCode */
static MI_CONST MI_PropertyDecl CIM_Error_CIMStatusCode_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0063650D, /* code */
    MI_T("CIMStatusCode"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, CIMStatusCode), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

/* property CIM_Error.CIMStatusCodeDescription */
static MI_CONST MI_PropertyDecl CIM_Error_CIMStatusCodeDescription_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00636E18, /* code */
    MI_T("CIMStatusCodeDescription"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_Error, CIMStatusCodeDescription), /* offset */
    MI_T("CIM_Error"), /* origin */
    MI_T("CIM_Error"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_Error_props[] =
{
    &CIM_Error_ErrorType_prop,
    &CIM_Error_OtherErrorType_prop,
    &CIM_Error_OwningEntity_prop,
    &CIM_Error_MessageID_prop,
    &CIM_Error_Message_prop,
    &CIM_Error_MessageArguments_prop,
    &CIM_Error_PerceivedSeverity_prop,
    &CIM_Error_ProbableCause_prop,
    &CIM_Error_ProbableCauseDescription_prop,
    &CIM_Error_RecommendedActions_prop,
    &CIM_Error_ErrorSource_prop,
    &CIM_Error_ErrorSourceFormat_prop,
    &CIM_Error_OtherErrorSourceFormat_prop,
    &CIM_Error_CIMStatusCode_prop,
    &CIM_Error_CIMStatusCodeDescription_prop,
};

static MI_CONST MI_Char* CIM_Error_Version_qual_value = MI_T("2.22.1");

static MI_CONST MI_Qualifier CIM_Error_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_Error_Version_qual_value
};

static MI_CONST MI_Char* CIM_Error_UMLPackagePath_qual_value = MI_T("CIM::Interop");

static MI_CONST MI_Qualifier CIM_Error_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_Error_UMLPackagePath_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_Error_quals[] =
{
    &CIM_Error_Version_qual,
    &CIM_Error_UMLPackagePath_qual,
};

/* class CIM_Error */
MI_CONST MI_ClassDecl CIM_Error_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_INDICATION, /* flags */
    0x00637209, /* code */
    MI_T("CIM_Error"), /* name */
    CIM_Error_quals, /* qualifiers */
    MI_COUNT(CIM_Error_quals), /* numQualifiers */
    CIM_Error_props, /* properties */
    MI_COUNT(CIM_Error_props), /* numProperties */
    sizeof(CIM_Error), /* size */
    NULL, /* superClass */
    NULL, /* superClassDecl */
    NULL, /* methods */
    0, /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_ConcreteJob
**
**==============================================================================
*/

static MI_CONST MI_Char* CIM_ConcreteJob_InstanceID_Override_qual_value = MI_T("InstanceID");

static MI_CONST MI_Qualifier CIM_ConcreteJob_InstanceID_Override_qual =
{
    MI_T("Override"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ConcreteJob_InstanceID_Override_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ConcreteJob_InstanceID_quals[] =
{
    &CIM_ConcreteJob_InstanceID_Override_qual,
};

/* property CIM_ConcreteJob.InstanceID */
static MI_CONST MI_PropertyDecl CIM_ConcreteJob_InstanceID_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x0069640A, /* code */
    MI_T("InstanceID"), /* name */
    CIM_ConcreteJob_InstanceID_quals, /* qualifiers */
    MI_COUNT(CIM_ConcreteJob_InstanceID_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob, InstanceID), /* offset */
    MI_T("CIM_ManagedElement"), /* origin */
    MI_T("CIM_ConcreteJob"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_ConcreteJob_Name_MaxLen_qual_value = 1024U;

static MI_CONST MI_Qualifier CIM_ConcreteJob_Name_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_ConcreteJob_Name_MaxLen_qual_value
};

static MI_CONST MI_Char* CIM_ConcreteJob_Name_Override_qual_value = MI_T("Name");

static MI_CONST MI_Qualifier CIM_ConcreteJob_Name_Override_qual =
{
    MI_T("Override"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ConcreteJob_Name_Override_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ConcreteJob_Name_quals[] =
{
    &CIM_ConcreteJob_Name_MaxLen_qual,
    &CIM_ConcreteJob_Name_Override_qual,
};

/* property CIM_ConcreteJob.Name */
static MI_CONST MI_PropertyDecl CIM_ConcreteJob_Name_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_REQUIRED, /* flags */
    0x006E6504, /* code */
    MI_T("Name"), /* name */
    CIM_ConcreteJob_Name_quals, /* qualifiers */
    MI_COUNT(CIM_ConcreteJob_Name_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob, Name), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_ConcreteJob"), /* propagator */
    NULL,
};

/* property CIM_ConcreteJob.JobState */
static MI_CONST MI_PropertyDecl CIM_ConcreteJob_JobState_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006A6508, /* code */
    MI_T("JobState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob, JobState), /* offset */
    MI_T("CIM_ConcreteJob"), /* origin */
    MI_T("CIM_ConcreteJob"), /* propagator */
    NULL,
};

/* property CIM_ConcreteJob.TimeOfLastStateChange */
static MI_CONST MI_PropertyDecl CIM_ConcreteJob_TimeOfLastStateChange_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00746515, /* code */
    MI_T("TimeOfLastStateChange"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob, TimeOfLastStateChange), /* offset */
    MI_T("CIM_ConcreteJob"), /* origin */
    MI_T("CIM_ConcreteJob"), /* propagator */
    NULL,
};

static MI_CONST MI_Datetime CIM_ConcreteJob_TimeBeforeRemoval_value = {0,{{0,0,5,0,0}}};

/* property CIM_ConcreteJob.TimeBeforeRemoval */
static MI_CONST MI_PropertyDecl CIM_ConcreteJob_TimeBeforeRemoval_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_REQUIRED, /* flags */
    0x00746C11, /* code */
    MI_T("TimeBeforeRemoval"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob, TimeBeforeRemoval), /* offset */
    MI_T("CIM_ConcreteJob"), /* origin */
    MI_T("CIM_ConcreteJob"), /* propagator */
    &CIM_ConcreteJob_TimeBeforeRemoval_value,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_ConcreteJob_props[] =
{
    &CIM_ConcreteJob_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_ConcreteJob_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
    &CIM_Job_JobStatus_prop,
    &CIM_Job_TimeSubmitted_prop,
    &CIM_Job_ScheduledStartTime_prop,
    &CIM_Job_StartTime_prop,
    &CIM_Job_ElapsedTime_prop,
    &CIM_Job_JobRunTimes_prop,
    &CIM_Job_RunMonth_prop,
    &CIM_Job_RunDay_prop,
    &CIM_Job_RunDayOfWeek_prop,
    &CIM_Job_RunStartInterval_prop,
    &CIM_Job_LocalOrUtcTime_prop,
    &CIM_Job_UntilTime_prop,
    &CIM_Job_Notify_prop,
    &CIM_Job_Owner_prop,
    &CIM_Job_Priority_prop,
    &CIM_Job_PercentComplete_prop,
    &CIM_Job_DeleteOnCompletion_prop,
    &CIM_Job_ErrorCode_prop,
    &CIM_Job_ErrorDescription_prop,
    &CIM_Job_RecoveryAction_prop,
    &CIM_Job_OtherRecoveryAction_prop,
    &CIM_ConcreteJob_JobState_prop,
    &CIM_ConcreteJob_TimeOfLastStateChange_prop,
    &CIM_ConcreteJob_TimeBeforeRemoval_prop,
};

/* parameter CIM_ConcreteJob.RequestStateChange(): RequestedState */
static MI_CONST MI_ParameterDecl CIM_ConcreteJob_RequestStateChange_RequestedState_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x0072650E, /* code */
    MI_T("RequestedState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob_RequestStateChange, RequestedState), /* offset */
};

/* parameter CIM_ConcreteJob.RequestStateChange(): TimeoutPeriod */
static MI_CONST MI_ParameterDecl CIM_ConcreteJob_RequestStateChange_TimeoutPeriod_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x0074640D, /* code */
    MI_T("TimeoutPeriod"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob_RequestStateChange, TimeoutPeriod), /* offset */
};

/* parameter CIM_ConcreteJob.RequestStateChange(): MIReturn */
static MI_CONST MI_ParameterDecl CIM_ConcreteJob_RequestStateChange_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob_RequestStateChange, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST CIM_ConcreteJob_RequestStateChange_params[] =
{
    &CIM_ConcreteJob_RequestStateChange_MIReturn_param,
    &CIM_ConcreteJob_RequestStateChange_RequestedState_param,
    &CIM_ConcreteJob_RequestStateChange_TimeoutPeriod_param,
};

/* method CIM_ConcreteJob.RequestStateChange() */
MI_CONST MI_MethodDecl CIM_ConcreteJob_RequestStateChange_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x00726512, /* code */
    MI_T("RequestStateChange"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    CIM_ConcreteJob_RequestStateChange_params, /* parameters */
    MI_COUNT(CIM_ConcreteJob_RequestStateChange_params), /* numParameters */
    sizeof(CIM_ConcreteJob_RequestStateChange), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_ConcreteJob"), /* origin */
    MI_T("CIM_ConcreteJob"), /* propagator */
    &schemaDecl, /* schema */
    NULL, /* method */
};

static MI_CONST MI_Char* CIM_ConcreteJob_GetError_Deprecated_qual_data_value[] =
{
    MI_T("CIM_ConcreteJob.GetErrors"),
};

static MI_CONST MI_ConstStringA CIM_ConcreteJob_GetError_Deprecated_qual_value =
{
    CIM_ConcreteJob_GetError_Deprecated_qual_data_value,
    MI_COUNT(CIM_ConcreteJob_GetError_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_ConcreteJob_GetError_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ConcreteJob_GetError_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ConcreteJob_GetError_quals[] =
{
    &CIM_ConcreteJob_GetError_Deprecated_qual,
};

static MI_CONST MI_Char* CIM_ConcreteJob_GetError_Error_EmbeddedInstance_qual_value = MI_T("CIM_Error");

static MI_CONST MI_Qualifier CIM_ConcreteJob_GetError_Error_EmbeddedInstance_qual =
{
    MI_T("EmbeddedInstance"),
    MI_STRING,
    0,
    &CIM_ConcreteJob_GetError_Error_EmbeddedInstance_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ConcreteJob_GetError_Error_quals[] =
{
    &CIM_ConcreteJob_GetError_Error_EmbeddedInstance_qual,
};

/* parameter CIM_ConcreteJob.GetError(): Error */
static MI_CONST MI_ParameterDecl CIM_ConcreteJob_GetError_Error_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x00657205, /* code */
    MI_T("Error"), /* name */
    CIM_ConcreteJob_GetError_Error_quals, /* qualifiers */
    MI_COUNT(CIM_ConcreteJob_GetError_Error_quals), /* numQualifiers */
    MI_INSTANCE, /* type */
    MI_T("CIM_Error"), /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob_GetError, Error), /* offset */
};

static MI_CONST MI_Char* CIM_ConcreteJob_GetError_MIReturn_Deprecated_qual_data_value[] =
{
    MI_T("CIM_ConcreteJob.GetErrors"),
};

static MI_CONST MI_ConstStringA CIM_ConcreteJob_GetError_MIReturn_Deprecated_qual_value =
{
    CIM_ConcreteJob_GetError_MIReturn_Deprecated_qual_data_value,
    MI_COUNT(CIM_ConcreteJob_GetError_MIReturn_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_ConcreteJob_GetError_MIReturn_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ConcreteJob_GetError_MIReturn_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ConcreteJob_GetError_MIReturn_quals[] =
{
    &CIM_ConcreteJob_GetError_MIReturn_Deprecated_qual,
};

/* parameter CIM_ConcreteJob.GetError(): MIReturn */
static MI_CONST MI_ParameterDecl CIM_ConcreteJob_GetError_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    CIM_ConcreteJob_GetError_MIReturn_quals, /* qualifiers */
    MI_COUNT(CIM_ConcreteJob_GetError_MIReturn_quals), /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob_GetError, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST CIM_ConcreteJob_GetError_params[] =
{
    &CIM_ConcreteJob_GetError_MIReturn_param,
    &CIM_ConcreteJob_GetError_Error_param,
};

/* method CIM_ConcreteJob.GetError() */
MI_CONST MI_MethodDecl CIM_ConcreteJob_GetError_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x00677208, /* code */
    MI_T("GetError"), /* name */
    CIM_ConcreteJob_GetError_quals, /* qualifiers */
    MI_COUNT(CIM_ConcreteJob_GetError_quals), /* numQualifiers */
    CIM_ConcreteJob_GetError_params, /* parameters */
    MI_COUNT(CIM_ConcreteJob_GetError_params), /* numParameters */
    sizeof(CIM_ConcreteJob_GetError), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_ConcreteJob"), /* origin */
    MI_T("CIM_ConcreteJob"), /* propagator */
    &schemaDecl, /* schema */
    NULL, /* method */
};

static MI_CONST MI_Char* CIM_ConcreteJob_GetErrors_Errors_EmbeddedInstance_qual_value = MI_T("CIM_Error");

static MI_CONST MI_Qualifier CIM_ConcreteJob_GetErrors_Errors_EmbeddedInstance_qual =
{
    MI_T("EmbeddedInstance"),
    MI_STRING,
    0,
    &CIM_ConcreteJob_GetErrors_Errors_EmbeddedInstance_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ConcreteJob_GetErrors_Errors_quals[] =
{
    &CIM_ConcreteJob_GetErrors_Errors_EmbeddedInstance_qual,
};

/* parameter CIM_ConcreteJob.GetErrors(): Errors */
static MI_CONST MI_ParameterDecl CIM_ConcreteJob_GetErrors_Errors_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x00657306, /* code */
    MI_T("Errors"), /* name */
    CIM_ConcreteJob_GetErrors_Errors_quals, /* qualifiers */
    MI_COUNT(CIM_ConcreteJob_GetErrors_Errors_quals), /* numQualifiers */
    MI_INSTANCEA, /* type */
    MI_T("CIM_Error"), /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob_GetErrors, Errors), /* offset */
};

/* parameter CIM_ConcreteJob.GetErrors(): MIReturn */
static MI_CONST MI_ParameterDecl CIM_ConcreteJob_GetErrors_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ConcreteJob_GetErrors, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST CIM_ConcreteJob_GetErrors_params[] =
{
    &CIM_ConcreteJob_GetErrors_MIReturn_param,
    &CIM_ConcreteJob_GetErrors_Errors_param,
};

/* method CIM_ConcreteJob.GetErrors() */
MI_CONST MI_MethodDecl CIM_ConcreteJob_GetErrors_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x00677309, /* code */
    MI_T("GetErrors"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    CIM_ConcreteJob_GetErrors_params, /* parameters */
    MI_COUNT(CIM_ConcreteJob_GetErrors_params), /* numParameters */
    sizeof(CIM_ConcreteJob_GetErrors), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_ConcreteJob"), /* origin */
    MI_T("CIM_ConcreteJob"), /* propagator */
    &schemaDecl, /* schema */
    NULL, /* method */
};

static MI_MethodDecl MI_CONST* MI_CONST CIM_ConcreteJob_meths[] =
{
    &CIM_Job_KillJob_rtti,
    &CIM_ConcreteJob_RequestStateChange_rtti,
    &CIM_ConcreteJob_GetError_rtti,
    &CIM_ConcreteJob_GetErrors_rtti,
};

static MI_CONST MI_Char* CIM_ConcreteJob_UMLPackagePath_qual_value = MI_T("CIM::Core::CoreElements");

static MI_CONST MI_Qualifier CIM_ConcreteJob_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_ConcreteJob_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_ConcreteJob_Deprecated_qual_data_value[] =
{
    MI_T("CIM_ConcreteJob.GetErrors"),
};

static MI_CONST MI_ConstStringA CIM_ConcreteJob_Deprecated_qual_value =
{
    CIM_ConcreteJob_Deprecated_qual_data_value,
    MI_COUNT(CIM_ConcreteJob_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_ConcreteJob_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ConcreteJob_Deprecated_qual_value
};

static MI_CONST MI_Char* CIM_ConcreteJob_Version_qual_value = MI_T("2.31.1");

static MI_CONST MI_Qualifier CIM_ConcreteJob_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_ConcreteJob_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ConcreteJob_quals[] =
{
    &CIM_ConcreteJob_UMLPackagePath_qual,
    &CIM_ConcreteJob_Deprecated_qual,
    &CIM_ConcreteJob_Version_qual,
};

/* class CIM_ConcreteJob */
MI_CONST MI_ClassDecl CIM_ConcreteJob_rtti =
{
    MI_FLAG_CLASS, /* flags */
    0x0063620F, /* code */
    MI_T("CIM_ConcreteJob"), /* name */
    CIM_ConcreteJob_quals, /* qualifiers */
    MI_COUNT(CIM_ConcreteJob_quals), /* numQualifiers */
    CIM_ConcreteJob_props, /* properties */
    MI_COUNT(CIM_ConcreteJob_props), /* numProperties */
    sizeof(CIM_ConcreteJob), /* size */
    MI_T("CIM_Job"), /* superClass */
    &CIM_Job_rtti, /* superClassDecl */
    CIM_ConcreteJob_meths, /* methods */
    MI_COUNT(CIM_ConcreteJob_meths), /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_EnabledLogicalElement
**
**==============================================================================
*/

static MI_CONST MI_Uint16 CIM_EnabledLogicalElement_EnabledState_value = 5;

/* property CIM_EnabledLogicalElement.EnabledState */
static MI_CONST MI_PropertyDecl CIM_EnabledLogicalElement_EnabledState_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0065650C, /* code */
    MI_T("EnabledState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement, EnabledState), /* offset */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    &CIM_EnabledLogicalElement_EnabledState_value,
};

/* property CIM_EnabledLogicalElement.OtherEnabledState */
static MI_CONST MI_PropertyDecl CIM_EnabledLogicalElement_OtherEnabledState_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F6511, /* code */
    MI_T("OtherEnabledState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement, OtherEnabledState), /* offset */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint16 CIM_EnabledLogicalElement_RequestedState_value = 12;

/* property CIM_EnabledLogicalElement.RequestedState */
static MI_CONST MI_PropertyDecl CIM_EnabledLogicalElement_RequestedState_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0072650E, /* code */
    MI_T("RequestedState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement, RequestedState), /* offset */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    &CIM_EnabledLogicalElement_RequestedState_value,
};

static MI_CONST MI_Uint16 CIM_EnabledLogicalElement_EnabledDefault_value = 2;

/* property CIM_EnabledLogicalElement.EnabledDefault */
static MI_CONST MI_PropertyDecl CIM_EnabledLogicalElement_EnabledDefault_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0065740E, /* code */
    MI_T("EnabledDefault"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement, EnabledDefault), /* offset */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    &CIM_EnabledLogicalElement_EnabledDefault_value,
};

/* property CIM_EnabledLogicalElement.TimeOfLastStateChange */
static MI_CONST MI_PropertyDecl CIM_EnabledLogicalElement_TimeOfLastStateChange_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00746515, /* code */
    MI_T("TimeOfLastStateChange"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement, TimeOfLastStateChange), /* offset */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    NULL,
};

/* property CIM_EnabledLogicalElement.AvailableRequestedStates */
static MI_CONST MI_PropertyDecl CIM_EnabledLogicalElement_AvailableRequestedStates_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00617318, /* code */
    MI_T("AvailableRequestedStates"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16A, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement, AvailableRequestedStates), /* offset */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint16 CIM_EnabledLogicalElement_TransitioningToState_value = 12;

/* property CIM_EnabledLogicalElement.TransitioningToState */
static MI_CONST MI_PropertyDecl CIM_EnabledLogicalElement_TransitioningToState_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00746514, /* code */
    MI_T("TransitioningToState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement, TransitioningToState), /* offset */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    &CIM_EnabledLogicalElement_TransitioningToState_value,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_EnabledLogicalElement_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_ManagedSystemElement_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
    &CIM_EnabledLogicalElement_EnabledState_prop,
    &CIM_EnabledLogicalElement_OtherEnabledState_prop,
    &CIM_EnabledLogicalElement_RequestedState_prop,
    &CIM_EnabledLogicalElement_EnabledDefault_prop,
    &CIM_EnabledLogicalElement_TimeOfLastStateChange_prop,
    &CIM_EnabledLogicalElement_AvailableRequestedStates_prop,
    &CIM_EnabledLogicalElement_TransitioningToState_prop,
};

/* parameter CIM_EnabledLogicalElement.RequestStateChange(): RequestedState */
static MI_CONST MI_ParameterDecl CIM_EnabledLogicalElement_RequestStateChange_RequestedState_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x0072650E, /* code */
    MI_T("RequestedState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement_RequestStateChange, RequestedState), /* offset */
};

/* parameter CIM_EnabledLogicalElement.RequestStateChange(): Job */
static MI_CONST MI_ParameterDecl CIM_EnabledLogicalElement_RequestStateChange_Job_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006A6203, /* code */
    MI_T("Job"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_REFERENCE, /* type */
    MI_T("CIM_ConcreteJob"), /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement_RequestStateChange, Job), /* offset */
};

/* parameter CIM_EnabledLogicalElement.RequestStateChange(): TimeoutPeriod */
static MI_CONST MI_ParameterDecl CIM_EnabledLogicalElement_RequestStateChange_TimeoutPeriod_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x0074640D, /* code */
    MI_T("TimeoutPeriod"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement_RequestStateChange, TimeoutPeriod), /* offset */
};

/* parameter CIM_EnabledLogicalElement.RequestStateChange(): MIReturn */
static MI_CONST MI_ParameterDecl CIM_EnabledLogicalElement_RequestStateChange_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_EnabledLogicalElement_RequestStateChange, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST CIM_EnabledLogicalElement_RequestStateChange_params[] =
{
    &CIM_EnabledLogicalElement_RequestStateChange_MIReturn_param,
    &CIM_EnabledLogicalElement_RequestStateChange_RequestedState_param,
    &CIM_EnabledLogicalElement_RequestStateChange_Job_param,
    &CIM_EnabledLogicalElement_RequestStateChange_TimeoutPeriod_param,
};

/* method CIM_EnabledLogicalElement.RequestStateChange() */
MI_CONST MI_MethodDecl CIM_EnabledLogicalElement_RequestStateChange_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x00726512, /* code */
    MI_T("RequestStateChange"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    CIM_EnabledLogicalElement_RequestStateChange_params, /* parameters */
    MI_COUNT(CIM_EnabledLogicalElement_RequestStateChange_params), /* numParameters */
    sizeof(CIM_EnabledLogicalElement_RequestStateChange), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    &schemaDecl, /* schema */
    NULL, /* method */
};

static MI_MethodDecl MI_CONST* MI_CONST CIM_EnabledLogicalElement_meths[] =
{
    &CIM_EnabledLogicalElement_RequestStateChange_rtti,
};

static MI_CONST MI_Char* CIM_EnabledLogicalElement_UMLPackagePath_qual_value = MI_T("CIM::Core::CoreElements");

static MI_CONST MI_Qualifier CIM_EnabledLogicalElement_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_EnabledLogicalElement_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_EnabledLogicalElement_Version_qual_value = MI_T("2.22.0");

static MI_CONST MI_Qualifier CIM_EnabledLogicalElement_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_EnabledLogicalElement_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_EnabledLogicalElement_quals[] =
{
    &CIM_EnabledLogicalElement_UMLPackagePath_qual,
    &CIM_EnabledLogicalElement_Version_qual,
};

/* class CIM_EnabledLogicalElement */
MI_CONST MI_ClassDecl CIM_EnabledLogicalElement_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_ABSTRACT, /* flags */
    0x00637419, /* code */
    MI_T("CIM_EnabledLogicalElement"), /* name */
    CIM_EnabledLogicalElement_quals, /* qualifiers */
    MI_COUNT(CIM_EnabledLogicalElement_quals), /* numQualifiers */
    CIM_EnabledLogicalElement_props, /* properties */
    MI_COUNT(CIM_EnabledLogicalElement_props), /* numProperties */
    sizeof(CIM_EnabledLogicalElement), /* size */
    MI_T("CIM_LogicalElement"), /* superClass */
    &CIM_LogicalElement_rtti, /* superClassDecl */
    CIM_EnabledLogicalElement_meths, /* methods */
    MI_COUNT(CIM_EnabledLogicalElement_meths), /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_System
**
**==============================================================================
*/

static MI_CONST MI_Uint32 CIM_System_Name_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_System_Name_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_System_Name_MaxLen_qual_value
};

static MI_CONST MI_Char* CIM_System_Name_Override_qual_value = MI_T("Name");

static MI_CONST MI_Qualifier CIM_System_Name_Override_qual =
{
    MI_T("Override"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_System_Name_Override_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_System_Name_quals[] =
{
    &CIM_System_Name_MaxLen_qual,
    &CIM_System_Name_Override_qual,
};

/* property CIM_System.Name */
static MI_CONST MI_PropertyDecl CIM_System_Name_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x006E6504, /* code */
    MI_T("Name"), /* name */
    CIM_System_Name_quals, /* qualifiers */
    MI_COUNT(CIM_System_Name_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_System, Name), /* offset */
    MI_T("CIM_ManagedSystemElement"), /* origin */
    MI_T("CIM_System"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_System_CreationClassName_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_System_CreationClassName_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_System_CreationClassName_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_System_CreationClassName_quals[] =
{
    &CIM_System_CreationClassName_MaxLen_qual,
};

/* property CIM_System.CreationClassName */
static MI_CONST MI_PropertyDecl CIM_System_CreationClassName_prop =
{
    MI_FLAG_PROPERTY|MI_FLAG_KEY, /* flags */
    0x00636511, /* code */
    MI_T("CreationClassName"), /* name */
    CIM_System_CreationClassName_quals, /* qualifiers */
    MI_COUNT(CIM_System_CreationClassName_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_System, CreationClassName), /* offset */
    MI_T("CIM_System"), /* origin */
    MI_T("CIM_System"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_System_NameFormat_MaxLen_qual_value = 64U;

static MI_CONST MI_Qualifier CIM_System_NameFormat_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_System_NameFormat_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_System_NameFormat_quals[] =
{
    &CIM_System_NameFormat_MaxLen_qual,
};

/* property CIM_System.NameFormat */
static MI_CONST MI_PropertyDecl CIM_System_NameFormat_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E740A, /* code */
    MI_T("NameFormat"), /* name */
    CIM_System_NameFormat_quals, /* qualifiers */
    MI_COUNT(CIM_System_NameFormat_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_System, NameFormat), /* offset */
    MI_T("CIM_System"), /* origin */
    MI_T("CIM_System"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_System_PrimaryOwnerName_MaxLen_qual_value = 64U;

static MI_CONST MI_Qualifier CIM_System_PrimaryOwnerName_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_System_PrimaryOwnerName_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_System_PrimaryOwnerName_quals[] =
{
    &CIM_System_PrimaryOwnerName_MaxLen_qual,
};

/* property CIM_System.PrimaryOwnerName */
static MI_CONST MI_PropertyDecl CIM_System_PrimaryOwnerName_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00706510, /* code */
    MI_T("PrimaryOwnerName"), /* name */
    CIM_System_PrimaryOwnerName_quals, /* qualifiers */
    MI_COUNT(CIM_System_PrimaryOwnerName_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_System, PrimaryOwnerName), /* offset */
    MI_T("CIM_System"), /* origin */
    MI_T("CIM_System"), /* propagator */
    NULL,
};

static MI_CONST MI_Uint32 CIM_System_PrimaryOwnerContact_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_System_PrimaryOwnerContact_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_System_PrimaryOwnerContact_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_System_PrimaryOwnerContact_quals[] =
{
    &CIM_System_PrimaryOwnerContact_MaxLen_qual,
};

/* property CIM_System.PrimaryOwnerContact */
static MI_CONST MI_PropertyDecl CIM_System_PrimaryOwnerContact_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00707413, /* code */
    MI_T("PrimaryOwnerContact"), /* name */
    CIM_System_PrimaryOwnerContact_quals, /* qualifiers */
    MI_COUNT(CIM_System_PrimaryOwnerContact_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_System, PrimaryOwnerContact), /* offset */
    MI_T("CIM_System"), /* origin */
    MI_T("CIM_System"), /* propagator */
    NULL,
};

/* property CIM_System.Roles */
static MI_CONST MI_PropertyDecl CIM_System_Roles_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00727305, /* code */
    MI_T("Roles"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_System, Roles), /* offset */
    MI_T("CIM_System"), /* origin */
    MI_T("CIM_System"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_System_OtherIdentifyingInfo_ArrayType_qual_value = MI_T("Indexed");

static MI_CONST MI_Qualifier CIM_System_OtherIdentifyingInfo_ArrayType_qual =
{
    MI_T("ArrayType"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_System_OtherIdentifyingInfo_ArrayType_qual_value
};

static MI_CONST MI_Uint32 CIM_System_OtherIdentifyingInfo_MaxLen_qual_value = 256U;

static MI_CONST MI_Qualifier CIM_System_OtherIdentifyingInfo_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_System_OtherIdentifyingInfo_MaxLen_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_System_OtherIdentifyingInfo_quals[] =
{
    &CIM_System_OtherIdentifyingInfo_ArrayType_qual,
    &CIM_System_OtherIdentifyingInfo_MaxLen_qual,
};

/* property CIM_System.OtherIdentifyingInfo */
static MI_CONST MI_PropertyDecl CIM_System_OtherIdentifyingInfo_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F6F14, /* code */
    MI_T("OtherIdentifyingInfo"), /* name */
    CIM_System_OtherIdentifyingInfo_quals, /* qualifiers */
    MI_COUNT(CIM_System_OtherIdentifyingInfo_quals), /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_System, OtherIdentifyingInfo), /* offset */
    MI_T("CIM_System"), /* origin */
    MI_T("CIM_System"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_System_IdentifyingDescriptions_ArrayType_qual_value = MI_T("Indexed");

static MI_CONST MI_Qualifier CIM_System_IdentifyingDescriptions_ArrayType_qual =
{
    MI_T("ArrayType"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_System_IdentifyingDescriptions_ArrayType_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_System_IdentifyingDescriptions_quals[] =
{
    &CIM_System_IdentifyingDescriptions_ArrayType_qual,
};

/* property CIM_System.IdentifyingDescriptions */
static MI_CONST MI_PropertyDecl CIM_System_IdentifyingDescriptions_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00697317, /* code */
    MI_T("IdentifyingDescriptions"), /* name */
    CIM_System_IdentifyingDescriptions_quals, /* qualifiers */
    MI_COUNT(CIM_System_IdentifyingDescriptions_quals), /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_System, IdentifyingDescriptions), /* offset */
    MI_T("CIM_System"), /* origin */
    MI_T("CIM_System"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_System_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_System_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
    &CIM_EnabledLogicalElement_EnabledState_prop,
    &CIM_EnabledLogicalElement_OtherEnabledState_prop,
    &CIM_EnabledLogicalElement_RequestedState_prop,
    &CIM_EnabledLogicalElement_EnabledDefault_prop,
    &CIM_EnabledLogicalElement_TimeOfLastStateChange_prop,
    &CIM_EnabledLogicalElement_AvailableRequestedStates_prop,
    &CIM_EnabledLogicalElement_TransitioningToState_prop,
    &CIM_System_CreationClassName_prop,
    &CIM_System_NameFormat_prop,
    &CIM_System_PrimaryOwnerName_prop,
    &CIM_System_PrimaryOwnerContact_prop,
    &CIM_System_Roles_prop,
    &CIM_System_OtherIdentifyingInfo_prop,
    &CIM_System_IdentifyingDescriptions_prop,
};

static MI_MethodDecl MI_CONST* MI_CONST CIM_System_meths[] =
{
    &CIM_EnabledLogicalElement_RequestStateChange_rtti,
};

static MI_CONST MI_Char* CIM_System_UMLPackagePath_qual_value = MI_T("CIM::Core::CoreElements");

static MI_CONST MI_Qualifier CIM_System_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_System_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_System_Version_qual_value = MI_T("2.15.0");

static MI_CONST MI_Qualifier CIM_System_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_System_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_System_quals[] =
{
    &CIM_System_UMLPackagePath_qual,
    &CIM_System_Version_qual,
};

/* class CIM_System */
MI_CONST MI_ClassDecl CIM_System_rtti =
{
    MI_FLAG_CLASS|MI_FLAG_ABSTRACT, /* flags */
    0x00636D0A, /* code */
    MI_T("CIM_System"), /* name */
    CIM_System_quals, /* qualifiers */
    MI_COUNT(CIM_System_quals), /* numQualifiers */
    CIM_System_props, /* properties */
    MI_COUNT(CIM_System_props), /* numProperties */
    sizeof(CIM_System), /* size */
    MI_T("CIM_EnabledLogicalElement"), /* superClass */
    &CIM_EnabledLogicalElement_rtti, /* superClassDecl */
    CIM_System_meths, /* methods */
    MI_COUNT(CIM_System_meths), /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_ComputerSystem
**
**==============================================================================
*/

static MI_CONST MI_Uint32 CIM_ComputerSystem_NameFormat_MaxLen_qual_value = 64U;

static MI_CONST MI_Qualifier CIM_ComputerSystem_NameFormat_MaxLen_qual =
{
    MI_T("MaxLen"),
    MI_UINT32,
    0,
    &CIM_ComputerSystem_NameFormat_MaxLen_qual_value
};

static MI_CONST MI_Char* CIM_ComputerSystem_NameFormat_Override_qual_value = MI_T("NameFormat");

static MI_CONST MI_Qualifier CIM_ComputerSystem_NameFormat_Override_qual =
{
    MI_T("Override"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ComputerSystem_NameFormat_Override_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ComputerSystem_NameFormat_quals[] =
{
    &CIM_ComputerSystem_NameFormat_MaxLen_qual,
    &CIM_ComputerSystem_NameFormat_Override_qual,
};

/* property CIM_ComputerSystem.NameFormat */
static MI_CONST MI_PropertyDecl CIM_ComputerSystem_NameFormat_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006E740A, /* code */
    MI_T("NameFormat"), /* name */
    CIM_ComputerSystem_NameFormat_quals, /* qualifiers */
    MI_COUNT(CIM_ComputerSystem_NameFormat_quals), /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ComputerSystem, NameFormat), /* offset */
    MI_T("CIM_System"), /* origin */
    MI_T("CIM_ComputerSystem"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_ComputerSystem_Dedicated_ArrayType_qual_value = MI_T("Indexed");

static MI_CONST MI_Qualifier CIM_ComputerSystem_Dedicated_ArrayType_qual =
{
    MI_T("ArrayType"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_ComputerSystem_Dedicated_ArrayType_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ComputerSystem_Dedicated_quals[] =
{
    &CIM_ComputerSystem_Dedicated_ArrayType_qual,
};

/* property CIM_ComputerSystem.Dedicated */
static MI_CONST MI_PropertyDecl CIM_ComputerSystem_Dedicated_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00646409, /* code */
    MI_T("Dedicated"), /* name */
    CIM_ComputerSystem_Dedicated_quals, /* qualifiers */
    MI_COUNT(CIM_ComputerSystem_Dedicated_quals), /* numQualifiers */
    MI_UINT16A, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ComputerSystem, Dedicated), /* offset */
    MI_T("CIM_ComputerSystem"), /* origin */
    MI_T("CIM_ComputerSystem"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_ComputerSystem_OtherDedicatedDescriptions_ArrayType_qual_value = MI_T("Indexed");

static MI_CONST MI_Qualifier CIM_ComputerSystem_OtherDedicatedDescriptions_ArrayType_qual =
{
    MI_T("ArrayType"),
    MI_STRING,
    MI_FLAG_DISABLEOVERRIDE|MI_FLAG_TOSUBCLASS,
    &CIM_ComputerSystem_OtherDedicatedDescriptions_ArrayType_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ComputerSystem_OtherDedicatedDescriptions_quals[] =
{
    &CIM_ComputerSystem_OtherDedicatedDescriptions_ArrayType_qual,
};

/* property CIM_ComputerSystem.OtherDedicatedDescriptions */
static MI_CONST MI_PropertyDecl CIM_ComputerSystem_OtherDedicatedDescriptions_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x006F731A, /* code */
    MI_T("OtherDedicatedDescriptions"), /* name */
    CIM_ComputerSystem_OtherDedicatedDescriptions_quals, /* qualifiers */
    MI_COUNT(CIM_ComputerSystem_OtherDedicatedDescriptions_quals), /* numQualifiers */
    MI_STRINGA, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ComputerSystem, OtherDedicatedDescriptions), /* offset */
    MI_T("CIM_ComputerSystem"), /* origin */
    MI_T("CIM_ComputerSystem"), /* propagator */
    NULL,
};

/* property CIM_ComputerSystem.ResetCapability */
static MI_CONST MI_PropertyDecl CIM_ComputerSystem_ResetCapability_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0072790F, /* code */
    MI_T("ResetCapability"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ComputerSystem, ResetCapability), /* offset */
    MI_T("CIM_ComputerSystem"), /* origin */
    MI_T("CIM_ComputerSystem"), /* propagator */
    NULL,
};

static MI_CONST MI_Char* CIM_ComputerSystem_PowerManagementCapabilities_Deprecated_qual_data_value[] =
{
    MI_T("CIM_PowerManagementCapabilities.PowerCapabilities"),
};

static MI_CONST MI_ConstStringA CIM_ComputerSystem_PowerManagementCapabilities_Deprecated_qual_value =
{
    CIM_ComputerSystem_PowerManagementCapabilities_Deprecated_qual_data_value,
    MI_COUNT(CIM_ComputerSystem_PowerManagementCapabilities_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_ComputerSystem_PowerManagementCapabilities_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ComputerSystem_PowerManagementCapabilities_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ComputerSystem_PowerManagementCapabilities_quals[] =
{
    &CIM_ComputerSystem_PowerManagementCapabilities_Deprecated_qual,
};

/* property CIM_ComputerSystem.PowerManagementCapabilities */
static MI_CONST MI_PropertyDecl CIM_ComputerSystem_PowerManagementCapabilities_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0070731B, /* code */
    MI_T("PowerManagementCapabilities"), /* name */
    CIM_ComputerSystem_PowerManagementCapabilities_quals, /* qualifiers */
    MI_COUNT(CIM_ComputerSystem_PowerManagementCapabilities_quals), /* numQualifiers */
    MI_UINT16A, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ComputerSystem, PowerManagementCapabilities), /* offset */
    MI_T("CIM_ComputerSystem"), /* origin */
    MI_T("CIM_ComputerSystem"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_ComputerSystem_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_System_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
    &CIM_EnabledLogicalElement_EnabledState_prop,
    &CIM_EnabledLogicalElement_OtherEnabledState_prop,
    &CIM_EnabledLogicalElement_RequestedState_prop,
    &CIM_EnabledLogicalElement_EnabledDefault_prop,
    &CIM_EnabledLogicalElement_TimeOfLastStateChange_prop,
    &CIM_EnabledLogicalElement_AvailableRequestedStates_prop,
    &CIM_EnabledLogicalElement_TransitioningToState_prop,
    &CIM_System_CreationClassName_prop,
    &CIM_ComputerSystem_NameFormat_prop,
    &CIM_System_PrimaryOwnerName_prop,
    &CIM_System_PrimaryOwnerContact_prop,
    &CIM_System_Roles_prop,
    &CIM_System_OtherIdentifyingInfo_prop,
    &CIM_System_IdentifyingDescriptions_prop,
    &CIM_ComputerSystem_Dedicated_prop,
    &CIM_ComputerSystem_OtherDedicatedDescriptions_prop,
    &CIM_ComputerSystem_ResetCapability_prop,
    &CIM_ComputerSystem_PowerManagementCapabilities_prop,
};

static MI_CONST MI_Char* CIM_ComputerSystem_SetPowerState_Deprecated_qual_data_value[] =
{
    MI_T("CIM_PowerManagementService.SetPowerState"),
};

static MI_CONST MI_ConstStringA CIM_ComputerSystem_SetPowerState_Deprecated_qual_value =
{
    CIM_ComputerSystem_SetPowerState_Deprecated_qual_data_value,
    MI_COUNT(CIM_ComputerSystem_SetPowerState_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_ComputerSystem_SetPowerState_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ComputerSystem_SetPowerState_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ComputerSystem_SetPowerState_quals[] =
{
    &CIM_ComputerSystem_SetPowerState_Deprecated_qual,
};

/* parameter CIM_ComputerSystem.SetPowerState(): PowerState */
static MI_CONST MI_ParameterDecl CIM_ComputerSystem_SetPowerState_PowerState_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x0070650A, /* code */
    MI_T("PowerState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ComputerSystem_SetPowerState, PowerState), /* offset */
};

/* parameter CIM_ComputerSystem.SetPowerState(): Time */
static MI_CONST MI_ParameterDecl CIM_ComputerSystem_SetPowerState_Time_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x00746504, /* code */
    MI_T("Time"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ComputerSystem_SetPowerState, Time), /* offset */
};

static MI_CONST MI_Char* CIM_ComputerSystem_SetPowerState_MIReturn_Deprecated_qual_data_value[] =
{
    MI_T("CIM_PowerManagementService.SetPowerState"),
};

static MI_CONST MI_ConstStringA CIM_ComputerSystem_SetPowerState_MIReturn_Deprecated_qual_value =
{
    CIM_ComputerSystem_SetPowerState_MIReturn_Deprecated_qual_data_value,
    MI_COUNT(CIM_ComputerSystem_SetPowerState_MIReturn_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier CIM_ComputerSystem_SetPowerState_MIReturn_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &CIM_ComputerSystem_SetPowerState_MIReturn_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ComputerSystem_SetPowerState_MIReturn_quals[] =
{
    &CIM_ComputerSystem_SetPowerState_MIReturn_Deprecated_qual,
};

/* parameter CIM_ComputerSystem.SetPowerState(): MIReturn */
static MI_CONST MI_ParameterDecl CIM_ComputerSystem_SetPowerState_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    CIM_ComputerSystem_SetPowerState_MIReturn_quals, /* qualifiers */
    MI_COUNT(CIM_ComputerSystem_SetPowerState_MIReturn_quals), /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_ComputerSystem_SetPowerState, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST CIM_ComputerSystem_SetPowerState_params[] =
{
    &CIM_ComputerSystem_SetPowerState_MIReturn_param,
    &CIM_ComputerSystem_SetPowerState_PowerState_param,
    &CIM_ComputerSystem_SetPowerState_Time_param,
};

/* method CIM_ComputerSystem.SetPowerState() */
MI_CONST MI_MethodDecl CIM_ComputerSystem_SetPowerState_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x0073650D, /* code */
    MI_T("SetPowerState"), /* name */
    CIM_ComputerSystem_SetPowerState_quals, /* qualifiers */
    MI_COUNT(CIM_ComputerSystem_SetPowerState_quals), /* numQualifiers */
    CIM_ComputerSystem_SetPowerState_params, /* parameters */
    MI_COUNT(CIM_ComputerSystem_SetPowerState_params), /* numParameters */
    sizeof(CIM_ComputerSystem_SetPowerState), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_ComputerSystem"), /* origin */
    MI_T("CIM_ComputerSystem"), /* propagator */
    &schemaDecl, /* schema */
    NULL, /* method */
};

static MI_MethodDecl MI_CONST* MI_CONST CIM_ComputerSystem_meths[] =
{
    &CIM_EnabledLogicalElement_RequestStateChange_rtti,
    &CIM_ComputerSystem_SetPowerState_rtti,
};

static MI_CONST MI_Char* CIM_ComputerSystem_UMLPackagePath_qual_value = MI_T("CIM::System::SystemElements");

static MI_CONST MI_Qualifier CIM_ComputerSystem_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_ComputerSystem_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_ComputerSystem_Version_qual_value = MI_T("2.28.0");

static MI_CONST MI_Qualifier CIM_ComputerSystem_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_ComputerSystem_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_ComputerSystem_quals[] =
{
    &CIM_ComputerSystem_UMLPackagePath_qual,
    &CIM_ComputerSystem_Version_qual,
};

/* class CIM_ComputerSystem */
MI_CONST MI_ClassDecl CIM_ComputerSystem_rtti =
{
    MI_FLAG_CLASS, /* flags */
    0x00636D12, /* code */
    MI_T("CIM_ComputerSystem"), /* name */
    CIM_ComputerSystem_quals, /* qualifiers */
    MI_COUNT(CIM_ComputerSystem_quals), /* numQualifiers */
    CIM_ComputerSystem_props, /* properties */
    MI_COUNT(CIM_ComputerSystem_props), /* numProperties */
    sizeof(CIM_ComputerSystem), /* size */
    MI_T("CIM_System"), /* superClass */
    &CIM_System_rtti, /* superClassDecl */
    CIM_ComputerSystem_meths, /* methods */
    MI_COUNT(CIM_ComputerSystem_meths), /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** CIM_VirtualComputerSystem
**
**==============================================================================
*/

/* property CIM_VirtualComputerSystem.VirtualSystem */
static MI_CONST MI_PropertyDecl CIM_VirtualComputerSystem_VirtualSystem_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00766D0D, /* code */
    MI_T("VirtualSystem"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(CIM_VirtualComputerSystem, VirtualSystem), /* offset */
    MI_T("CIM_VirtualComputerSystem"), /* origin */
    MI_T("CIM_VirtualComputerSystem"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST CIM_VirtualComputerSystem_props[] =
{
    &CIM_ManagedElement_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_System_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
    &CIM_EnabledLogicalElement_EnabledState_prop,
    &CIM_EnabledLogicalElement_OtherEnabledState_prop,
    &CIM_EnabledLogicalElement_RequestedState_prop,
    &CIM_EnabledLogicalElement_EnabledDefault_prop,
    &CIM_EnabledLogicalElement_TimeOfLastStateChange_prop,
    &CIM_EnabledLogicalElement_AvailableRequestedStates_prop,
    &CIM_EnabledLogicalElement_TransitioningToState_prop,
    &CIM_System_CreationClassName_prop,
    &CIM_ComputerSystem_NameFormat_prop,
    &CIM_System_PrimaryOwnerName_prop,
    &CIM_System_PrimaryOwnerContact_prop,
    &CIM_System_Roles_prop,
    &CIM_System_OtherIdentifyingInfo_prop,
    &CIM_System_IdentifyingDescriptions_prop,
    &CIM_ComputerSystem_Dedicated_prop,
    &CIM_ComputerSystem_OtherDedicatedDescriptions_prop,
    &CIM_ComputerSystem_ResetCapability_prop,
    &CIM_ComputerSystem_PowerManagementCapabilities_prop,
    &CIM_VirtualComputerSystem_VirtualSystem_prop,
};

static MI_MethodDecl MI_CONST* MI_CONST CIM_VirtualComputerSystem_meths[] =
{
    &CIM_EnabledLogicalElement_RequestStateChange_rtti,
    &CIM_ComputerSystem_SetPowerState_rtti,
};

static MI_CONST MI_Char* CIM_VirtualComputerSystem_UMLPackagePath_qual_value = MI_T("CIM::System::SystemElements");

static MI_CONST MI_Qualifier CIM_VirtualComputerSystem_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &CIM_VirtualComputerSystem_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* CIM_VirtualComputerSystem_Version_qual_value = MI_T("2.6.0");

static MI_CONST MI_Qualifier CIM_VirtualComputerSystem_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &CIM_VirtualComputerSystem_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST CIM_VirtualComputerSystem_quals[] =
{
    &CIM_VirtualComputerSystem_UMLPackagePath_qual,
    &CIM_VirtualComputerSystem_Version_qual,
};

/* class CIM_VirtualComputerSystem */
MI_CONST MI_ClassDecl CIM_VirtualComputerSystem_rtti =
{
    MI_FLAG_CLASS, /* flags */
    0x00636D19, /* code */
    MI_T("CIM_VirtualComputerSystem"), /* name */
    CIM_VirtualComputerSystem_quals, /* qualifiers */
    MI_COUNT(CIM_VirtualComputerSystem_quals), /* numQualifiers */
    CIM_VirtualComputerSystem_props, /* properties */
    MI_COUNT(CIM_VirtualComputerSystem_props), /* numProperties */
    sizeof(CIM_VirtualComputerSystem), /* size */
    MI_T("CIM_ComputerSystem"), /* superClass */
    &CIM_ComputerSystem_rtti, /* superClassDecl */
    CIM_VirtualComputerSystem_meths, /* methods */
    MI_COUNT(CIM_VirtualComputerSystem_meths), /* numMethods */
    &schemaDecl, /* schema */
    NULL, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** Docker_Container
**
**==============================================================================
*/

/* property Docker_Container.InstanceID */
static MI_CONST MI_PropertyDecl Docker_Container_InstanceID_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x0069640A, /* code */
    MI_T("InstanceID"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container, InstanceID), /* offset */
    MI_T("CIM_ManagedElement"), /* origin */
    MI_T("Docker_Container"), /* propagator */
    NULL,
};

/* property Docker_Container.Ports */
static MI_CONST MI_PropertyDecl Docker_Container_Ports_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00707305, /* code */
    MI_T("Ports"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container, Ports), /* offset */
    MI_T("Docker_Container"), /* origin */
    MI_T("Docker_Container"), /* propagator */
    NULL,
};

/* property Docker_Container.Command */
static MI_CONST MI_PropertyDecl Docker_Container_Command_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00636407, /* code */
    MI_T("Command"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container, Command), /* offset */
    MI_T("Docker_Container"), /* origin */
    MI_T("Docker_Container"), /* propagator */
    NULL,
};

/* property Docker_Container.Image */
static MI_CONST MI_PropertyDecl Docker_Container_Image_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00696505, /* code */
    MI_T("Image"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container, Image), /* offset */
    MI_T("Docker_Container"), /* origin */
    MI_T("Docker_Container"), /* propagator */
    NULL,
};

/* property Docker_Container.SizeRW */
static MI_CONST MI_PropertyDecl Docker_Container_SizeRW_prop =
{
    MI_FLAG_PROPERTY, /* flags */
    0x00737706, /* code */
    MI_T("SizeRW"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_STRING, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container, SizeRW), /* offset */
    MI_T("Docker_Container"), /* origin */
    MI_T("Docker_Container"), /* propagator */
    NULL,
};

static MI_PropertyDecl MI_CONST* MI_CONST Docker_Container_props[] =
{
    &Docker_Container_InstanceID_prop,
    &CIM_ManagedElement_Caption_prop,
    &CIM_ManagedElement_Description_prop,
    &CIM_ManagedElement_ElementName_prop,
    &CIM_ManagedSystemElement_InstallDate_prop,
    &CIM_System_Name_prop,
    &CIM_ManagedSystemElement_OperationalStatus_prop,
    &CIM_ManagedSystemElement_StatusDescriptions_prop,
    &CIM_ManagedSystemElement_Status_prop,
    &CIM_ManagedSystemElement_HealthState_prop,
    &CIM_ManagedSystemElement_CommunicationStatus_prop,
    &CIM_ManagedSystemElement_DetailedStatus_prop,
    &CIM_ManagedSystemElement_OperatingStatus_prop,
    &CIM_ManagedSystemElement_PrimaryStatus_prop,
    &CIM_EnabledLogicalElement_EnabledState_prop,
    &CIM_EnabledLogicalElement_OtherEnabledState_prop,
    &CIM_EnabledLogicalElement_RequestedState_prop,
    &CIM_EnabledLogicalElement_EnabledDefault_prop,
    &CIM_EnabledLogicalElement_TimeOfLastStateChange_prop,
    &CIM_EnabledLogicalElement_AvailableRequestedStates_prop,
    &CIM_EnabledLogicalElement_TransitioningToState_prop,
    &CIM_System_CreationClassName_prop,
    &CIM_ComputerSystem_NameFormat_prop,
    &CIM_System_PrimaryOwnerName_prop,
    &CIM_System_PrimaryOwnerContact_prop,
    &CIM_System_Roles_prop,
    &CIM_System_OtherIdentifyingInfo_prop,
    &CIM_System_IdentifyingDescriptions_prop,
    &CIM_ComputerSystem_Dedicated_prop,
    &CIM_ComputerSystem_OtherDedicatedDescriptions_prop,
    &CIM_ComputerSystem_ResetCapability_prop,
    &CIM_ComputerSystem_PowerManagementCapabilities_prop,
    &CIM_VirtualComputerSystem_VirtualSystem_prop,
    &Docker_Container_Ports_prop,
    &Docker_Container_Command_prop,
    &Docker_Container_Image_prop,
    &Docker_Container_SizeRW_prop,
};

/* parameter Docker_Container.RequestStateChange(): RequestedState */
static MI_CONST MI_ParameterDecl Docker_Container_RequestStateChange_RequestedState_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x0072650E, /* code */
    MI_T("RequestedState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT16, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container_RequestStateChange, RequestedState), /* offset */
};

/* parameter Docker_Container.RequestStateChange(): Job */
static MI_CONST MI_ParameterDecl Docker_Container_RequestStateChange_Job_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006A6203, /* code */
    MI_T("Job"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_REFERENCE, /* type */
    MI_T("CIM_ConcreteJob"), /* className */
    0, /* subscript */
    offsetof(Docker_Container_RequestStateChange, Job), /* offset */
};

/* parameter Docker_Container.RequestStateChange(): TimeoutPeriod */
static MI_CONST MI_ParameterDecl Docker_Container_RequestStateChange_TimeoutPeriod_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x0074640D, /* code */
    MI_T("TimeoutPeriod"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container_RequestStateChange, TimeoutPeriod), /* offset */
};

/* parameter Docker_Container.RequestStateChange(): MIReturn */
static MI_CONST MI_ParameterDecl Docker_Container_RequestStateChange_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container_RequestStateChange, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST Docker_Container_RequestStateChange_params[] =
{
    &Docker_Container_RequestStateChange_MIReturn_param,
    &Docker_Container_RequestStateChange_RequestedState_param,
    &Docker_Container_RequestStateChange_Job_param,
    &Docker_Container_RequestStateChange_TimeoutPeriod_param,
};

/* method Docker_Container.RequestStateChange() */
MI_CONST MI_MethodDecl Docker_Container_RequestStateChange_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x00726512, /* code */
    MI_T("RequestStateChange"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    Docker_Container_RequestStateChange_params, /* parameters */
    MI_COUNT(Docker_Container_RequestStateChange_params), /* numParameters */
    sizeof(Docker_Container_RequestStateChange), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_EnabledLogicalElement"), /* origin */
    MI_T("CIM_EnabledLogicalElement"), /* propagator */
    &schemaDecl, /* schema */
    (MI_ProviderFT_Invoke)Docker_Container_Invoke_RequestStateChange, /* method */
};

static MI_CONST MI_Char* Docker_Container_SetPowerState_Deprecated_qual_data_value[] =
{
    MI_T("CIM_PowerManagementService.SetPowerState"),
};

static MI_CONST MI_ConstStringA Docker_Container_SetPowerState_Deprecated_qual_value =
{
    Docker_Container_SetPowerState_Deprecated_qual_data_value,
    MI_COUNT(Docker_Container_SetPowerState_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier Docker_Container_SetPowerState_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &Docker_Container_SetPowerState_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST Docker_Container_SetPowerState_quals[] =
{
    &Docker_Container_SetPowerState_Deprecated_qual,
};

/* parameter Docker_Container.SetPowerState(): PowerState */
static MI_CONST MI_ParameterDecl Docker_Container_SetPowerState_PowerState_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x0070650A, /* code */
    MI_T("PowerState"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container_SetPowerState, PowerState), /* offset */
};

/* parameter Docker_Container.SetPowerState(): Time */
static MI_CONST MI_ParameterDecl Docker_Container_SetPowerState_Time_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_IN, /* flags */
    0x00746504, /* code */
    MI_T("Time"), /* name */
    NULL, /* qualifiers */
    0, /* numQualifiers */
    MI_DATETIME, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container_SetPowerState, Time), /* offset */
};

static MI_CONST MI_Char* Docker_Container_SetPowerState_MIReturn_Deprecated_qual_data_value[] =
{
    MI_T("CIM_PowerManagementService.SetPowerState"),
};

static MI_CONST MI_ConstStringA Docker_Container_SetPowerState_MIReturn_Deprecated_qual_value =
{
    Docker_Container_SetPowerState_MIReturn_Deprecated_qual_data_value,
    MI_COUNT(Docker_Container_SetPowerState_MIReturn_Deprecated_qual_data_value),
};

static MI_CONST MI_Qualifier Docker_Container_SetPowerState_MIReturn_Deprecated_qual =
{
    MI_T("Deprecated"),
    MI_STRINGA,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_RESTRICTED,
    &Docker_Container_SetPowerState_MIReturn_Deprecated_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST Docker_Container_SetPowerState_MIReturn_quals[] =
{
    &Docker_Container_SetPowerState_MIReturn_Deprecated_qual,
};

/* parameter Docker_Container.SetPowerState(): MIReturn */
static MI_CONST MI_ParameterDecl Docker_Container_SetPowerState_MIReturn_param =
{
    MI_FLAG_PARAMETER|MI_FLAG_OUT, /* flags */
    0x006D6E08, /* code */
    MI_T("MIReturn"), /* name */
    Docker_Container_SetPowerState_MIReturn_quals, /* qualifiers */
    MI_COUNT(Docker_Container_SetPowerState_MIReturn_quals), /* numQualifiers */
    MI_UINT32, /* type */
    NULL, /* className */
    0, /* subscript */
    offsetof(Docker_Container_SetPowerState, MIReturn), /* offset */
};

static MI_ParameterDecl MI_CONST* MI_CONST Docker_Container_SetPowerState_params[] =
{
    &Docker_Container_SetPowerState_MIReturn_param,
    &Docker_Container_SetPowerState_PowerState_param,
    &Docker_Container_SetPowerState_Time_param,
};

/* method Docker_Container.SetPowerState() */
MI_CONST MI_MethodDecl Docker_Container_SetPowerState_rtti =
{
    MI_FLAG_METHOD, /* flags */
    0x0073650D, /* code */
    MI_T("SetPowerState"), /* name */
    Docker_Container_SetPowerState_quals, /* qualifiers */
    MI_COUNT(Docker_Container_SetPowerState_quals), /* numQualifiers */
    Docker_Container_SetPowerState_params, /* parameters */
    MI_COUNT(Docker_Container_SetPowerState_params), /* numParameters */
    sizeof(Docker_Container_SetPowerState), /* size */
    MI_UINT32, /* returnType */
    MI_T("CIM_ComputerSystem"), /* origin */
    MI_T("CIM_ComputerSystem"), /* propagator */
    &schemaDecl, /* schema */
    (MI_ProviderFT_Invoke)Docker_Container_Invoke_SetPowerState, /* method */
};

static MI_MethodDecl MI_CONST* MI_CONST Docker_Container_meths[] =
{
    &Docker_Container_RequestStateChange_rtti,
    &Docker_Container_SetPowerState_rtti,
};

static MI_CONST MI_ProviderFT Docker_Container_funcs =
{
  (MI_ProviderFT_Load)Docker_Container_Load,
  (MI_ProviderFT_Unload)Docker_Container_Unload,
  (MI_ProviderFT_GetInstance)Docker_Container_GetInstance,
  (MI_ProviderFT_EnumerateInstances)Docker_Container_EnumerateInstances,
  (MI_ProviderFT_CreateInstance)Docker_Container_CreateInstance,
  (MI_ProviderFT_ModifyInstance)Docker_Container_ModifyInstance,
  (MI_ProviderFT_DeleteInstance)Docker_Container_DeleteInstance,
  (MI_ProviderFT_AssociatorInstances)NULL,
  (MI_ProviderFT_ReferenceInstances)NULL,
  (MI_ProviderFT_EnableIndications)NULL,
  (MI_ProviderFT_DisableIndications)NULL,
  (MI_ProviderFT_Subscribe)NULL,
  (MI_ProviderFT_Unsubscribe)NULL,
  (MI_ProviderFT_Invoke)NULL,
};

static MI_CONST MI_Char* Docker_Container_UMLPackagePath_qual_value = MI_T("CIM::System::SystemElements");

static MI_CONST MI_Qualifier Docker_Container_UMLPackagePath_qual =
{
    MI_T("UMLPackagePath"),
    MI_STRING,
    0,
    &Docker_Container_UMLPackagePath_qual_value
};

static MI_CONST MI_Char* Docker_Container_Version_qual_value = MI_T("1.0.0");

static MI_CONST MI_Qualifier Docker_Container_Version_qual =
{
    MI_T("Version"),
    MI_STRING,
    MI_FLAG_ENABLEOVERRIDE|MI_FLAG_TRANSLATABLE|MI_FLAG_RESTRICTED,
    &Docker_Container_Version_qual_value
};

static MI_Qualifier MI_CONST* MI_CONST Docker_Container_quals[] =
{
    &Docker_Container_UMLPackagePath_qual,
    &Docker_Container_Version_qual,
};

/* class Docker_Container */
MI_CONST MI_ClassDecl Docker_Container_rtti =
{
    MI_FLAG_CLASS, /* flags */
    0x00647210, /* code */
    MI_T("Docker_Container"), /* name */
    Docker_Container_quals, /* qualifiers */
    MI_COUNT(Docker_Container_quals), /* numQualifiers */
    Docker_Container_props, /* properties */
    MI_COUNT(Docker_Container_props), /* numProperties */
    sizeof(Docker_Container), /* size */
    MI_T("CIM_VirtualComputerSystem"), /* superClass */
    &CIM_VirtualComputerSystem_rtti, /* superClassDecl */
    Docker_Container_meths, /* methods */
    MI_COUNT(Docker_Container_meths), /* numMethods */
    &schemaDecl, /* schema */
    &Docker_Container_funcs, /* functions */
    NULL, /* owningClass */
};

/*
**==============================================================================
**
** __mi_server
**
**==============================================================================
*/

MI_Server* __mi_server;
/*
**==============================================================================
**
** Schema
**
**==============================================================================
*/

static MI_ClassDecl MI_CONST* MI_CONST classes[] =
{
    &CIM_Collection_rtti,
    &CIM_ComputerSystem_rtti,
    &CIM_ConcreteJob_rtti,
    &CIM_EnabledLogicalElement_rtti,
    &CIM_Error_rtti,
    &CIM_InstalledProduct_rtti,
    &CIM_Job_rtti,
    &CIM_LogicalElement_rtti,
    &CIM_ManagedElement_rtti,
    &CIM_ManagedSystemElement_rtti,
    &CIM_StatisticalData_rtti,
    &CIM_System_rtti,
    &CIM_VirtualComputerSystem_rtti,
    &Docker_Container_rtti,
    &Docker_ContainerProcessorStatistics_rtti,
    &Docker_ContainerStatistics_rtti,
    &Docker_Server_rtti,
};

MI_SchemaDecl schemaDecl =
{
    NULL, /* qualifierDecls */
    0, /* numQualifierDecls */
    classes, /* classDecls */
    MI_COUNT(classes), /* classDecls */
};

/*
**==============================================================================
**
** MI_Server Methods
**
**==============================================================================
*/

MI_Result MI_CALL MI_Server_GetVersion(
    MI_Uint32* version){
    return __mi_server->serverFT->GetVersion(version);
}

MI_Result MI_CALL MI_Server_GetSystemName(
    const MI_Char** systemName)
{
    return __mi_server->serverFT->GetSystemName(systemName);
}

