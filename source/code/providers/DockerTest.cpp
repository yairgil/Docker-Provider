#include"cJSON_Extend.h"
#include"Docker_Server_Class_Provider.h"
#include"Docker_Container_Class_Provider.h"
#include"Docker_ContainerProcessorStatistics_Class_Provider.h"
#include"Docker_ContainerStatistics_Class_Provider.h"
#include<vector>
#include<iostream>
#include<map>
using std::vector;
using std::map;
using std::cout;
using std::endl;
namespace mi{
  void Docker_Server_Class_set(Docker_Server_Class& inst, cJSON* version, cJSON* info);
  void Docker_Container_Class_Provider_set(Docker_Container_Class& inst,cJSON* response) ;
  vector<Docker_ContainerProcessorStatistics_Class> Docker_ContainerProcessorStatistics_Class_get(cJSON* r, string id);
  void Docker_ContainerStatistics_Class_set(Docker_ContainerStatistics_Class& stats, cJSON* data, map<string, Docker_ContainerStatistics_Class>& map);
}

void test_Docker_Server_Class_set()
{
  string server_info ="{\
    \"Containers\": 11,\
    \"CpuCfsPeriod\": true,\
    \"CpuCfsQuota\": true,\
    \"Debug\": false,\
    \"DockerRootDir\": \"/var/lib/docker\",\
    \"Driver\": \"btrfs\",\
    \"DriverStatus\": [[\"\"]],\
    \"ExecutionDriver\": \"native-0.1\",\
    \"ExperimentalBuild\": false,\
    \"HttpProxy\": \"http://test:test@localhost:8080\",\
    \"HttpsProxy\": \"https://test:test@localhost:8080\",\
    \"ID\": \"7TRN:IPZB:QYBB:VPBQ:UMPP:KARE:6ZNR:XE6T:7EWV:PKF4:ZOJD:TPYS\",\
    \"IPv4Forwarding\": true,\
    \"Images\": 16,\
    \"IndexServerAddress\": \"https://index.docker.io/v1/\",\
    \"InitPath\": \"/usr/bin/docker\",\
    \"InitSha1\": \"\",\
    \"KernelVersion\": \"3.12.0-1-amd64\",\
    \"Labels\": [\
        \"storage=ssd\"\
    ],\
    \"MemTotal\": 2099236864,\
    \"MemoryLimit\": true,\
    \"NCPU\": 1,\
    \"NEventsListener\": 0,\
    \"NFd\": 11,\
    \"NGoroutines\": 21,\
    \"Name\": \"prod-server-42\",\
    \"NoProxy\": \"9.81.1.160\",\
    \"OomKillDisable\": true,\
    \"OperatingSystem\": \"Boot2Docker\",\
    \"RegistryConfig\": {\
        \"IndexConfigs\": {\
            \"docker.io\": {\
                \"Mirrors\": null,\
                \"Name\": \"docker.io\",\
                \"Official\": true,\
                \"Secure\": true\
            }\
        },\
        \"InsecureRegistryCIDRs\": [\
            \"127.0.0.0/8\"\
        ]\
    },\
    \"SwapLimit\": false,\
    \"SystemTime\": \"2015-03-10T11:11:23.730591467-07:00\"\
}"; 
  string server_version = "{\
     \"Version\": \"1.5.0\",\
     \"Os\": \"linux\",\
     \"KernelVersion\": \"3.18.5-tinycore64\",\
     \"GoVersion\": \"go1.4.1\",\
     \"GitCommit\": \"a8a31ef\",\
     \"Arch\": \"amd64\",\
     \"ApiVersion\": \"1.19\"\
}"; 

   mi::Docker_Server_Class inst;
   mi::Docker_Server_Class_set(inst,cJSON_Parse(server_version.c_str()),cJSON_Parse(server_info.c_str()));
   cout<<inst.Name().value.Str()<<endl;

}






int main()
{
 test_Docker_Server_Class_set();
}

