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

std::string extractPathFromGET(std::string httpGET);
void sendResponseToGET(int newSockFD, std::string path);
std::string getFileName(std::string fromPath);

#endif
