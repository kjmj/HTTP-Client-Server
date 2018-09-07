/*
 ** showip.c -- show IP addresses for a host given on the command line
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdlib.h>


char *parseAddress(char *toParse) {
	char *path = strstr(toParse, "/");

	if(path == NULL) {
		return toParse;
	}

	int diff = path - toParse;

	char *address = (char *) malloc(strlen(toParse) + 1);
	strncpy(address, toParse, diff);

	return address;
}

int main(int argc, char *argv[])
{
    char *url = "www.google.com/index.html";
    char *complicated = "www.google.com/path/to/index.html";
    char *withHTTP = "http://www.google.com";
    char *noPath = "www.google.com";
    
    char *parsed = parseAddress(url);
    printf("%s -> %s\n", url, parsed);

    char *parsedcomplicated = parseAddress(complicated);
    printf("%s -> %s\n", complicated, parsedcomplicated);

    char *parsedWithHTTP = parseAddress(withHTTP);
    printf("%s -> %s\n", withHTTP, parsedWithHTTP);

    char * parsedNoPath = parseAddress(noPath);
    printf("%s -> %s\n", noPath, parsedNoPath);
    
    
    return 0;
}



