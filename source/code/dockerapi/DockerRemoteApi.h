#ifndef DOCKERREMOTEAPI_H
#define DOCKERREMOTEAPI_H
#include<string>
#include<vector>
#include"cJSON.h"
using std::string;
using std::vector;

vector<string> listContainer(bool all=false );
vector<cJSON*> getResponse(vector<string>& request, bool isMultiJson);
#endif
