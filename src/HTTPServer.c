#include "HTTPServer.h"

int main(int argc, char const *argv[]) {

	const int backlog = 10; // how many pending connections the queue will hold

	// TODO read from command line
	char *portNum = "34905"; // the port users will be connecting to

	struct sockaddr_storage clientAddress;
	socklen_t clientAddressSize;

	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;

	int newSocketFD;

	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me

	int status;
	if ((status = getaddrinfo(NULL, portNum, &hints, &results)) != 0) {
		fprintf(stderr, "Error in getaddrinfo (server): %s\n",
				gai_strerror(status));
		return (EXIT_FAILURE);
	}

	// make a socket
	if ((sockFD = socket(results->ai_family, results->ai_socktype, results->ai_protocol)) < 0) {
		perror("Error in socket (server)");
		return (EXIT_FAILURE);
	}

	// try to bind
	if (bind(sockFD, results->ai_addr, results->ai_addrlen) < 0) {
		perror("Error in bind (server)");
		return (EXIT_FAILURE);
	}

	// listen
	if (listen(sockFD, backlog) < 0) {
		perror("Error in listen (server)");
		return (EXIT_FAILURE);
	}

	// accept incoming connections
	while(1) {
		clientAddressSize = sizeof(clientAddress);

		if((newSocketFD = accept(sockFD, (struct sockaddr *) &clientAddress,
				&clientAddressSize)) < 0) {
			perror("Error in accept (server)");
			exit(EXIT_FAILURE);
		}


		// testing connection
		long valRead;
		char buffer[30000] = {0};
		char * message = "Hello From Server";

		valRead = read( newSocketFD , buffer, 30000);

		printf("%s\n",buffer);
		write(newSocketFD , message , strlen(message));
		printf("Message sent from server\n");
		close(newSocketFD);
	}

	return 0;
}
