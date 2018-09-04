//
//  TCPClient.c
//  SocketProgramming
//
//  Created by Kenneth Morton Jr on 9/3/18.
//  Copyright © 2018 Kenneth Morton Jr. All rights reserved.
//

#include "TCPClient.h"

int main(int argc, char const *argv[])
{
    // for testing
    char *message = "Hello from Client";
    long valRead;
    char buffer[1024] = {0};
    
    // read this stuff in from command line args
    char *serverURL = "www.google.com";
    char *portNum = "80";

    struct addrinfo hints;
    struct addrinfo *results;
    int sockfd;
    
    // first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    int status;
    if ((status = getaddrinfo(serverURL, portNum, &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return(EXIT_FAILURE);
    }
    
    // make a socket
    if((sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol)) < 0) {
        printf("\nError creating socket\n");
        return(EXIT_FAILURE);
    }
    
    // connect to the server
    if((connect(sockfd, results->ai_addr, results->ai_addrlen)) < 0) {
        printf("\nError connecting to server\n");
        return(EXIT_FAILURE);
    }

    send(sockfd, message, strlen(message), 0);
    printf("Message sent to server\n");
    //valread = read( sock , buffer, 1024);
    //printf("%s\n",buffer );
    return 0;
}

