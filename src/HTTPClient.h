#ifndef HTTPClient_h
#define HTTPClient_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>

char *parseAddress(char *toParse);
char *parsePath(char *toParse);
char *formGET(char *hostName, char *pathName);

#endif
