#include "HTTPClient.h"

int main(int argc, char const *argv[]) {

	// TODO eventually read this stuff in from command line args
	char *serverURL = "127.0.0.1";
	char *portNum = "34905";

	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;

	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int status;
	if ((status = getaddrinfo(serverURL, portNum, &hints, &results)) != 0) {
		fprintf(stderr, "Error in getaddrinfo (client): %s\n",
				gai_strerror(status));
		return (EXIT_FAILURE);
	}

	// make a socket
	if ((sockFD = socket(results->ai_family, results->ai_socktype, results->ai_protocol)) < 0) {
		perror("Error in socket (client)");
		return (EXIT_FAILURE);
	}

	// connect to the server
	if ((connect(sockFD, results->ai_addr, results->ai_addrlen)) < 0) {
		perror("Error in connect (client)");
		return (EXIT_FAILURE);
	}

	// testing connection
	char *message = "Hello from Client";
	long valRead;
	char buffer[1024] = { 0 };

	send(sockFD, message, strlen(message), 0);

	printf("Message sent from client\n");
	valRead = read( sockFD , buffer, 1024);
    printf("%s\n",buffer );

	return 0;
}
