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
    
    struct addrinfo hints;
    struct addrinfo *results;
    
    
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    
    int status;
    if ((status = getaddrinfo("www.google.com/path", "80", &hints, &results)) != 0) {
        fprintf(stderr, "Error in getaddrinfo (client): %s\n",
                gai_strerror(status));
        return (1);
    }
    
    return 0;
}
