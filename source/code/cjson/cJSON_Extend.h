#ifndef CJSON_EXTEND_H
#define CJSON_EXTEND_H 
#include <string>
#include"cJSON.h"
using std::string;
cJSON* cJSON_Get(cJSON* json, string  name) ;
string cJSON_GetChildName(cJSON* cjson);
string cJSON_GetArray(cJSON* json,string name);
string cJSON_Get_Int_As_String(cJSON* json,string name);
//string to_string(double v);
#endif
