#include "HTTPServer.h"

int main(int argc, char const *argv[]) {

	int backlog = 10; // how many pending connections queue will hold

	// read from command line
	char *portNum = "3490"; // the port users will be connecting to

    struct sockaddr_storage clientAddress;
    socklen_t clientAddressSize;
    struct addrinfo hints;
	struct addrinfo *results;
    int sockFD;
	int connectionFD;

    // first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    int status;
    if((status = getaddrinfo(NULL, portNum, &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return(EXIT_FAILURE);
    }

    // make a socket
    if((sockFD = socket(results->ai_family, results->ai_socktype, results->ai_protocol)) < 0) {
        printf("\nError creating socket\n");
        return(EXIT_FAILURE);
    }

    // try to bind
    if(bind(sockFD, results->ai_addr, results->ai_addrlen) < 0) {
        //printf("%s\n", "Error binding");
        perror("Error binding");
        return(EXIT_FAILURE);
    }

    printf("Bind correctly\n");

    listen(sockFD, backlog);

    // now accept an incoming connection:

    clientAddressSize = sizeof clientAddress;
    connectionFD = accept(sockFD, (struct sockaddr *)&clientAddress, &clientAddressSize);

    // ready to communicate on socket descriptor new_fd!


	printf("end of server");
	return 0;
}
