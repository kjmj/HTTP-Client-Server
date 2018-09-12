#include "HTTPServer.hpp"
using namespace std;

int main(int argc, char const *argv[]) {

	// process command line args
	if (argc != 2) {
		cout << "USAGE: ./HTTPServer portNumber" << endl;
		return (EXIT_FAILURE);
	}

	// used to build our socket
	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;
	int newSockFD; // assigned when the client connects

	string portNum = argv[1]; // the port users will be connecting to
	const int backlog = 10; // how many pending connections the queue will hold

	struct sockaddr_storage clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);

	// for receiving client requests
	const int bufferSize = 2048;
	char buffer[bufferSize] = { 0 };

	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6
	//hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	int status;
	if ((status = getaddrinfo(NULL, portNum.c_str(), &hints, &results)) != 0) {
		cout << stderr << "Error in getaddrinfo (server): "
				<< gai_strerror(status) << endl;
		return (EXIT_FAILURE);
	}

	// make a socket
	if ((sockFD = socket(results->ai_family, results->ai_socktype,
			results->ai_protocol)) < 0) {
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
	while (true) {

		if ((newSockFD = accept(sockFD, (struct sockaddr *) &clientAddress,
				&clientAddressSize)) < 0) {
			perror("Error in accept (server)");
			exit(EXIT_FAILURE);
		}

		string GETRequest;
		if (recv(newSockFD, buffer, bufferSize, 0) < 0) {
			perror("Error in read (server)");
			exit(EXIT_FAILURE);
		}
		GETRequest = buffer;

		string path = parsePathFromGETRequest(GETRequest);
		cout << "Path: " << path << endl;

		sendGETResponse(newSockFD, path);
		cout << endl;

		close(newSockFD);
	}

	return 0;
}

/**
 * returns the path from the HTTP GET request
 * the extracted path does not include a leading "/"
 *
 */
string parsePathFromGETRequest(string httpGETRequest) {

	istringstream iss(httpGETRequest);
	string line;

	// read line by line
	while (getline(iss, line)) {

		int pos = line.find("GET ");
		if (pos == 0) {
			string firstDelim = "GET ";
			string stopDelim = " HTTP/1.1";
			unsigned firstDelimPos = line.find(firstDelim) + 1;
			unsigned endOfFirstDelim = firstDelimPos + firstDelim.length();
			unsigned lastDelimPos = line.find(stopDelim);

			// get the path
			return line.substr(endOfFirstDelim, lastDelimPos - endOfFirstDelim);
		}
	}

	// otherwise, there was no path specified
	return "";
}

/**
 * send a response to the GET request
 */
void sendGETResponse(int newSockFD, string path) {
	string header;
	string body;
	string response;
	string endOfResponse = "\r\n\r\n";

	// send them the default page
	if (path == "") {
		cout << "Empty path specified" << endl;
		response = "HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n\r\n"
				"Default page, path not specified.\r\n\r\n";

		write(newSockFD, response.c_str(), response.length());
	}
	// resource doesn't exist
	else if (access(path.c_str(), F_OK) != 0) {
		cout << "Resource does not exist" << endl;
		response = "HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n\r\n"
				"HTTP 404 Not Found. The resource does not exist.\r\n\r\n";

		write(newSockFD, response.c_str(), response.length());
	}
	// resource exists, but we don't have read access
	else if (access(path.c_str(), R_OK) != 0) {
		cout << "Resource exists without read access" << endl;
		response =
				"HTTP/1.1 403 Forbidden\r\n"
						"Content-Type: text/html; charset=UTF-8\r\n\r\n"
						"HTTP 403 Forbidden. The resource exists, but access is forbidden.\r\n\r\n";

		write(newSockFD, response.c_str(), response.length());
	}
	// resource exists and we have read access
	else if (access(path.c_str(), F_OK) == 0
			&& access(path.c_str(), R_OK) == 0) {
		cout << "Path exists with read access" << endl;
		header = "HTTP/1.1 200 OK\r\n\r\n";

		// open and send the file as the body
		const int bufferSize = 1024;
		char buffer[bufferSize] = { 0 };

		int test = 0;
		FILE *file = fopen(path.c_str(), "r");
		while (fgets(buffer, bufferSize, file)) {
			test = test + bufferSize;
			body = body.append(buffer);
			memset(buffer, 0, bufferSize);
		}

		// form the response
		response = header.append(body).append(endOfResponse);

		write(newSockFD, response.c_str(), response.length());
	}
}

/**
 * returns the file name extracted from a path
 * the path should not include a leading "/"
 * i.e specify the path as "path/to/whatever.foo"
 */
string getFileName(string fromPath) {
	string fileName;
	size_t pos = fromPath.find_last_of("/");

	// the path is the file name
	if (pos == string::npos) {
		fileName = fromPath;
	} else if (fromPath.length() >= pos + 1) {
		fileName = fromPath.substr(pos + 1);
	}

	return fileName;
}
