#include"cJSON_Extend.h"
#include<sstream>
#include<vector>
using std::vector;

/**
*  given name "a.b.c.d" to find {"a":{"b":{"c"{"d":1}}}
*  return {"d":1}
*/
cJSON* cJSON_Get(cJSON* json, string  name) {
    vector<int> dots ;
    int begin = 0;
    dots.push_back(-1);
    while(true) {
        begin = name.find(".", begin);
        if (begin == (int)std::string::npos) {
            break;
        }
        dots.push_back(begin);
        begin++;
    }
    dots.push_back(name.length());
    cJSON* cjson = json;
    for (unsigned int j=0; j<dots.size()-1; j++) {
        string p = name.substr(dots[j]+1, dots[j+1]-dots[j]-1);
        cjson = cJSON_GetObjectItem(cjson, p.c_str());
        if (cjson == NULL) {
            throw string("Fail to read property `"+name)+"` From "+cJSON_Print(json);
        }
    }
    return cjson;
}



string cJSON_GetChildName(cJSON* cjson) {
    cJSON* child = cjson->child;
    string ret = "";
    while (child != NULL) {
        ret = ret + child->string + " ";
        child = child->next;
    }
    return ret;
}


string to_string(double v) {
    std::ostringstream o;
    o << v;
    return o.str();
}

string cJSON_Get_Int_As_String(cJSON* json, string  name){
   return to_string(cJSON_Get(json,name)->valueint);
}
string cJSON_GetArray(cJSON* json, string name = "") {
    if (name.size() > 0) {
        json = cJSON_Get(json, name);
    }
    string s = "";
    for (int i = 0; i < cJSON_GetArraySize(json); i++) {
        char* vstring = cJSON_GetArrayItem(json, i)->valuestring;
        double d = cJSON_GetArrayItem(json, i)->valuedouble;
        s = vstring ? s + string(vstring) + " " : s + to_string(d) + " ";
    }
    return s;
}


