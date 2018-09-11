#ifndef HTTPServer_hpp
#define HTTPServer_hpp

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<iostream>
#include <sstream>

#include <unistd.h>
#include <netdb.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

std::string parsePathFromGETRequest(std::string httpGETRequest);
std::string getFileName(std::string fromPath);
void sendGETResponse(int newSockFD, std::string path);

#endif
