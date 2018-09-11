#include "HTTPClient.hpp"

using namespace std;

int main(int argc, char *argv[]) {

	// user specified information
	bool computeRTT; // does the user want to compute round trip time (RTT)
	string serverURL;
	string portNum;

	// read in command line args
	if (argc == 3) {
		computeRTT = false;
		serverURL = argv[1];
		portNum = argv[2];
	} else if (argc == 4) {
		string foo = argv[1];
		if (foo == "-p") {
			computeRTT = true;
			serverURL = argv[2];
			portNum = argv[3];
		} else {
			cout << "USAGE: ./HTTPClient [-p] serverUrl portNumber" << endl;
			return (EXIT_FAILURE);
		}
	} else {
		cout << "USAGE: ./HTTPClient [-p] serverUrl portNumber" << endl;
		return (EXIT_FAILURE);
	}

	// used to build our socket
	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;

	// for forming our HTTP GET request
	string GET; // will contain the HTTP GET request
	string hostName = parseAddress(serverURL);
	string pathName = parsePath(serverURL);

	// for receiving info from the server
	const int bufferSize = 1; // TODO change this to a larger number
	char buffer[bufferSize] = { 0 };

	cout << "Host Name: " << hostName << endl;
	cout << "Path Name: " << pathName << endl;

	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_family = AF_INET; // TODO can i use AF_UNSPEC for this project?

	// try to resolve the address from a URL
	int status;
	if ((status = getaddrinfo(hostName.c_str(), portNum.c_str(), &hints,
			&results)) != 0) {
		cout << stderr << "Error in getaddrinfo (client): "
				<< gai_strerror(status) << endl;
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

	GET = createGETRequest(hostName, pathName);
	cout << "GET Request:\n" << GET << endl;

	// send the get request to the server
	if (send(sockFD, GET.c_str(), GET.length(), 0) < 0) {
		perror("Error in send (client)");
	}
	cout << "Request sent from client" << endl;

	// recieve data until the server closes the connection
	while (recv(sockFD, buffer, bufferSize, 0) > 0) {
		fputs(buffer, stdout);
		memset(buffer, 0, bufferSize);
	}

	// close the socket
	close(sockFD);

	return 0;
}

/**
 * take in a host name and path name to form the HTTP GET request
 * note the path should not have a leading "/"
 * i.e. specify a path as "path/to/file.foo"
 */
string createGETRequest(string hostName, string pathName) {
	string GET = "GET /";
	GET = GET.append(pathName);
	GET = GET.append(" HTTP/1.1\r\n");

	GET = GET.append("Host: ");
	GET = GET.append(hostName);
	GET = GET.append("\r\n");
	GET = GET.append("\r\n");

	return GET;
}

/**
 * returns the address portion of a url
 * the parsed address is everything up until the start of the path
 */
string parseAddress(string urlToParse) {
	return urlToParse.substr(0, urlToParse.find("/"));
}

/**
 * returns the path portion of a url
 * the parsed path does not include a leading "/"
 */
string parsePath(string urlToParse) {
	size_t pos = urlToParse.find("/");

	if (pos == string::npos) {
		return "";
	}

	return urlToParse.substr(pos + 1); // everything after "/"
}
