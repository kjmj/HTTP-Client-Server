
#ifndef HTTPClient_hpp
#define HTTPClient_hpp


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<iostream>

#include <unistd.h>
#include <netdb.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



std::string parseAddress(std::string toParse);
std::string parsePath(std::string toParse);
std::string formGET(std::string hostName, std::string pathName);
bool isValidIpAddress(char *ipAddress);

#endif
