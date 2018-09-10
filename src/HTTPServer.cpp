#include "HTTPServer.hpp"
using namespace std;

int main(int argc, char const *argv[]) {

	if (argc != 2) {
		cout << "USAGE: ./HTTPServer port_number" << endl;
		return (EXIT_FAILURE);
	}
	string portNum = argv[1]; // the port users will be connecting to
	const int backlog = 10; // how many pending connections the queue will hold

	struct sockaddr_storage clientAddress;
	socklen_t clientAddressSize;

	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;
	int newSockFD; // for when the client connects

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
		clientAddressSize = sizeof(clientAddress);

		if ((newSockFD = accept(sockFD, (struct sockaddr *) &clientAddress,
				&clientAddressSize)) < 0) {
			perror("Error in accept (server)");
			exit(EXIT_FAILURE);
		}

		const int bufferSize = 1024;
		char buffer[bufferSize] = { 0 };

		string entireBuffer;
		if (recv(newSockFD, buffer, bufferSize, 0) < 0) { // TODO make this more robust
			perror("Error in read (server)");
			exit(EXIT_FAILURE);
		}
		entireBuffer = buffer;

		string path = extractPathFromGET(entireBuffer);
		cout << "Path: " << path << endl;

		sendResponseToGET(newSockFD, path);
		//memset(buffer, 0, bufferSize);

		printf("Message sent from server\n");
		close(newSockFD);
	}

	// TODO free stuff
	return 0;
}

// extract the requested path from the http GET request
string extractPathFromGET(string httpGET) {

	istringstream iss(httpGET);
	string token;

	while (getline(iss, token)) {

		int pos = token.find("GET ");
		if (pos == 0) {
			//cout << "Found get" << endl;
			string firstDelim = "GET ";
			string stopDelim = " HTTP/1.1";
			unsigned firstDelimPos = token.find(firstDelim) + 1;
			unsigned endOfFirstDelim = firstDelimPos + firstDelim.length();
			unsigned lastDelimPos = token.find(stopDelim);

			return token.substr(endOfFirstDelim, lastDelimPos - endOfFirstDelim);
		}
	}
	return "";
}

// send a response to the GET request
void sendResponseToGET(int newSockFD, string path) {
	string header;
	string endOfResponse = "\r\n\r\n";

	//string fileName = getFileName(path);
	//cout << "FileName: " << fileName << endl;

	if (access(path.c_str(), F_OK) != 0) {
		cout << "Resource does not exist" << endl;
		header = "HTTP/1.1 404 Not Found\r\n"
				 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
				 "HTTP 404 Not Found. The resource does not exist.\r\n\r\n";

		write(newSockFD, header.c_str(), header.length());
	} else if (access(path.c_str(), R_OK) != 0) {
		cout << "Path exists without read access" << endl;
		header = "HTTP/1.1 403 Forbidden\r\n"
				 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
				 "HTTP 403 Forbidden. The resource exists, but access is forbidden.\r\n\r\n";

		write(newSockFD, header.c_str(), header.length());
	} else if (access(path.c_str(), F_OK) == 0
			&& access(path.c_str(), R_OK) == 0) {
		cout << "Path exists with read access" << endl;
		header = "HTTP/1.1 200 OK\r\n\r\n";
		write(newSockFD, header.c_str(), header.length());

		// open and send the file
		const int bufferSize = 1024;
		char buffer[bufferSize] = { 0 };

		FILE *file = fopen(path.c_str(), "r");
		while (fgets(buffer, bufferSize, file)) {
			send(newSockFD, buffer, strlen(buffer), 0);
			memset(buffer, 0, bufferSize);
		}

		write(newSockFD, endOfResponse.c_str(), endOfResponse.length());
	}
}

// get the file name from a path
// path must be specified as "path/to/the/file.txt"
string getFileName(string fromPath) {

	string fileName;
	unsigned int pos = fromPath.find_last_of("/");

	// the path is the file name
	if (pos == string::npos) {
		fileName = fromPath;
	} else if (fromPath.length() >= pos + 1) {
		fileName = fromPath.substr(pos + 1);
	}

	return fileName;
}
