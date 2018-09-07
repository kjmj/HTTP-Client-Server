#include "HTTPClient.h"

int main(int argc, char *argv[]) {

	char *serverURL;
	char *portNum;
	bool computeRTT;

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

	char *hostName = parseAddress(serverURL);
	char *pathName = parsePath(serverURL);

	const int bufferSize = 1; // TODO buffer size, does it matter?
	char buffer[bufferSize] = { 0 };

	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;

	// localhost: "127.0.0.1";

	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6
	//hints.ai_family = AF_INET; // TODO can i use AF_UNSPEC?
	hints.ai_socktype = SOCK_STREAM;

	int status;
	if ((status = getaddrinfo(hostName, portNum, &hints, &results)) != 0) {
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


	// try to send GET request
	strncat(hostName, pathName, strlen(hostName) + strlen(pathName) + 1);
	printf("%s\n", hostName);



	char *GET = "GET / HTTP/1.1\r\n"
			"Host: 127.0.0.1\r\n"
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


// TODO this wont parse an address with a "/" before the path
// for example http://www.google.com/path
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

/**
 * returns the path part of a url
 */
char *parsePath(char *toParse) {

    char *parsed = strstr(toParse, "/");

    if(parsed == NULL) {
    	return "";
    }

    char *withoutForwardSlash = (char*) malloc(strlen(parsed) + 1);
    strncpy(withoutForwardSlash, parsed + 1, strlen(parsed) - 1);

    return withoutForwardSlash;
}

