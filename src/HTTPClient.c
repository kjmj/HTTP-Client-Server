#include "HTTPClient.h"

int main(int argc, char *argv[]) {

	char *serverURL;
	char *portNum;
	bool computeRTT;
	const int bufferSize = 128; // TODO buffer size?
	char buffer[bufferSize] = { 0 };

	if(argc == 3) {
		computeRTT = false;
		serverURL = argv[1];
		portNum = argv[2];
	}
	else if (argc == 4) {
		computeRTT = true;
		serverURL = argv[2];
		portNum = argv[3];
	}
	else {
		printf("USAGE: ./HTTPClient [-options] server_url port_number\n");
		return (EXIT_FAILURE);
	}
	// localhost: "127.0.0.1";

	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;

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

//	char *s1 = "Host: ";
//
//	printf("before: %s", s1);
//
//	char *s2 = "www.example.com";
//
//	size_t len1 = strlen(s1), len2 = strlen(s2);
//	char *concat = (char*) malloc(len1 + len2 + 1);
//
//	memcpy(concat, s1, len1);
//	memcpy(concat+len1, s2, len2+1);
//	strcat(s1, s2);

//	char *s3 = "\r\n";
//	strcat(s1, s3);
//	printf("after: %s", s1);

	char *GETTEST = "GET / HTTP/1.1\r\n"
			"Host: www.example.com\r\n"
			"Connection: keep-alive\r\n"
			"\r\n";
	//char *message = "Hello from Client";

	//send(sockFD, message, strlen(message), 0);
	send(sockFD, GETTEST, strlen(GETTEST), 0);

	printf("Request sent from client\n");


	while (recv(sockFD, buffer, bufferSize, 0) > 0) {
		fputs(buffer, stdout);
		memset(buffer, 0, bufferSize);
	}

	close(sockFD);

	return 0;
}

