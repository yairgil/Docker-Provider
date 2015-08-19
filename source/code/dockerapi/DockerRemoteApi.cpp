#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include "cJSON_Extend.h"
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include "DockerRemoteApi.h"
#define docker_socket "/var/run/docker.sock"
#define api_list_container string("GET /containers/json HTTP/1.1\r\n\r\n")
#define api_list_all_container string("GET /containers/json?all=1 HTTP/1.1\r\n\r\n")
/**
*  read the size data from socket, support timeout
*  return the size of data readed
*/
size_t readSocket(int fd, char* buf, int size, int timeout) {
    fd_set rfds;
    struct timeval tv;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    if (retval == -1)
        return -1;
    else if (FD_ISSET(fd, &rfds))
        return read(fd, buf, size);
    else
        return 0;
}
/**
*   create n connection to docker_socket
*   vector<int> fds need to be empty when pass it
*   will throw string excption if fail
*/
void createConnection(unsigned int n, vector<int>& fds) {
    fds.clear();
    char socket_path[] = docker_socket;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    memcpy(addr.sun_path, socket_path, sizeof(socket_path));
    for (unsigned int i = 0; i < n; i++) {
        int fd = 0;
        if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) <= 0) {
            break;
        }
        if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            close(fd);
            break;
        }
        fds.push_back(fd);
    }
    if (fds.size() < n) {
        for (unsigned int i = 0; i < fds.size(); i++) {
            close(fds[i]);
        }
        throw string("create Connection to ") + socket_path + " failed";
    }
}

/**
*  Give multi request to docker remote api,
*  return multi cJSON* response
*/
void  getResponseInBatch(vector<string>& request, vector<cJSON*>& response, unsigned int start, unsigned int end) {
    const int bufferSize = 4096;
    const int timeoutSecond = 5;
    vector<int> sockfd;
    end = (end == 0 || end > request.size()) ? request.size() : end;
    int n = end - start;
    createConnection(n, sockfd);
    for (int i = 0; i < n; i++) {
        size_t r = write(sockfd[i], request[start + i].c_str(), request[start + i].length());
        if (r != request[start + i].length()) {
            throw string("write socket error");
        }
    }
    for (int i = 0; i < n; i++) {
        char readBuf[bufferSize + 1];
        size_t read_n = 0;
        string raw_response;
        response.push_back(NULL);
        while ((read_n = readSocket(sockfd[i], readBuf, bufferSize, timeoutSecond))>0) {
            readBuf[read_n] = 0;
            raw_response.append(readBuf);
            int json_begin = raw_response.find("\r\n{\""); //Json object
            if (json_begin == (int)std::string::npos) {
                json_begin = raw_response.find("\r\n["); //Json array
            }
            if (json_begin != (int)std::string::npos) {
                cJSON* json = cJSON_Parse(raw_response.c_str() + json_begin);
                if (json) {
                    response[start + i] = json;
                    break;
                }
            }
        }
        close(sockfd[i]);
        if (raw_response.length() > 0 && response[start + i] == NULL) {
            throw string("Fail to parse data:`" + raw_response + "` to json" + " \n Request :" + request[start + i]);
        }
    }

}

/*
response should be empty when pass to this function
*/
vector<cJSON*> getResponse(vector<string>& request) {
    vector<cJSON*> response;
    
    for (unsigned int i = 0; i < request.size(); i += 100) {
        getResponseInBatch(request, response, i, i + 100);
    }
    return response;
}
/*
*  return currently running container ids
*  if find a container not in g_container_map, then crate ContainerData for it
*/
vector<string> listContainer( bool all) {
    vector<string > request(1, all ? api_list_all_container : api_list_container);
    vector<string> ids;
    vector<cJSON*> response = getResponse(request);
    if (response[0] == NULL) {
        return ids;
    }
    int n = cJSON_GetArraySize(response[0]);
    if (n == 0) {
        cJSON_Delete(response[0]);
        return ids;
    }
    for (int i = 0; i < n; i++) {
        cJSON* container = cJSON_GetArrayItem(response[0], i);
        std::string id = cJSON_Get(container, "Id")->valuestring;
        ids.push_back(id);
    }
    cJSON_Delete(response[0]);
    return ids;
}



