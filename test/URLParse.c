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

int main(int argc, char *argv[])
{
    char *url = "www.google.com/index.html";
    char *complicated = "www.google.com/path/to/index.html";
    char *withHTTP = "http://www.google.com";
    char *noPath = "www.google.com";
    
    char *parsed = strstr(url, "/");
    char *parsedcomplicated = strstr(complicated, "/");
    char *parsedWithHTTP = strstr(withHTTP, "/");
    char * parsedNoPath = strstr(noPath, "/");
    
    printf("%s -> %s\n", url, parsed);
    printf("%s -> %s\n", complicated, parsedcomplicated);
    printf("%s -> %s\n", withHTTP, parsedWithHTTP);
    printf("%s -> %s\n", noPath, parsedNoPath);

    
    char str[] ="www.google.com/index.html";
    char * pch;
    printf ("Splitting string \"%s\" into tokens:\n",str);
    pch = strtok (str,"/");
    while (pch != NULL)
    {
        printf ("%s\n",pch);
        pch = strtok (NULL, "/");
    }
    
    
    
    return 0;
}
