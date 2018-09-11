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

std::string createGETRequest(std::string hostName, std::string pathName);
std::string parseAddress(std::string urlToParse);
std::string parsePath(std::string urlToParse);

#endif
