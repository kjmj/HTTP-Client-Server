#include "HTTPClient.h"

int main(int argc, char *argv[]) {

	char *serverURL;
	char *portNum;
	bool computeRTT;
	const int bufferSize = 1; // TODO buffer size?
	char buffer[bufferSize] = { 0 };

	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;

	// read in command line args
	if (argc == 3) {
		computeRTT = false;
		serverURL = argv[1];
		portNum = argv[2];
	} else if (argc == 4) {
		computeRTT = true;
		serverURL = argv[2];
		portNum = argv[3];
	} else {
		printf("USAGE: ./HTTPClient [-options] server_url port_number\n");
		return (EXIT_FAILURE);
	}
	// localhost: "127.0.0.1";

	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof(hints));

	// TODO UNSPEC or INET
	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6
	//hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status;
	if ((status = getaddrinfo(serverURL, portNum, &hints, &results)) != 0) {
		fprintf(stderr, "Error in getaddrinfo (client): %s\n",
				gai_strerror(status));
		return (EXIT_FAILURE);
	}

	// make a socket
	if ((sockFD = socket(results->ai_family, results->ai_socktype,
			results->ai_protocol)) < 0) {
		perror("Error in socket (client)");
		return (EXIT_FAILURE);
	}

	// connect to the server
	if ((connect(sockFD, results->ai_addr, results->ai_addrlen)) < 0) {
		perror("Error in connect (client)");
		return (EXIT_FAILURE);
	}

	// testing connection

	char *GET = "GET / HTTP/1.1\r\n"
			"Host: www.example.com\r\n"
			"Connection: close\r\n"
			"\r\n";

	send(sockFD, GET, strlen(GET), 0);
	printf("Request sent from client\n");

	while (recv(sockFD, buffer, bufferSize, 0) > 0) {
		fputs(buffer, stdout);
		memset(buffer, 0, bufferSize);
	}

	close(sockFD);

	return 0;
}

