#ifndef HTTPServer_hpp
#define HTTPServer_hpp

#include <cstdio>
#include <cstdlib>
#include <string>
#include<iostream>
#include <sstream>


#include <unistd.h>
#include <netdb.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

std::string extractPath(std::string httpGET);
void sendResponse(int newSockFD, std::string path);

#endif
