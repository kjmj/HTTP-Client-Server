#include "HTTPServer.hpp"
using namespace std;

int main(int argc, char const *argv[]) {

	const int backlog = 10; // how many pending connections the queue will hold

	// TODO read from command line
	string portNum = argv[1]; // the port users will be connecting to

	struct sockaddr_storage clientAddress;
	socklen_t clientAddressSize;

	struct addrinfo hints;
	struct addrinfo *results;
	int sockFD;

	int newSockFD;

	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof(hints));
	//hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me

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
	while (1) {
		clientAddressSize = sizeof(clientAddress);

		if ((newSockFD = accept(sockFD, (struct sockaddr *) &clientAddress,
				&clientAddressSize)) < 0) {
			perror("Error in accept (server)");
			exit(EXIT_FAILURE);
		}

		// testing connection
		long valRead;

		const int bufferSize = 1024; // TODO buffer size, does it matter?
		char buffer[bufferSize] = { 0 };

		//string message = "Hello From Server";
		string message =
				"HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

		int recvd;
		string entireBuffer;
		if (recv(newSockFD, buffer, bufferSize, 0) < 0) { // TODO make this more robust
			perror("Error in read (server)");
			exit(EXIT_FAILURE);
		}
		entireBuffer = buffer;

//		while ((recvd = recv(newSockFD, buffer, bufferSize, 0)) > 0) {
//
//			entireBuffer.append(buffer);
//			//cout << entireBuffer << endl;
//			cout << entireBuffer.length() << endl;
//
//
////			if(entireBuffer.find('\r\n\r\n')) {
////				cout << "end of HTTP request" << endl;
////				//break;
////			}
//
////			if(strstr(entireBuffer.c_str(), "\r\n\r\n")) {
////
////				cout << "FOUND IT Eh" << endl;
////				break;
////			}
//
////			if(entireBuffer.length() > 4) {
////				if((entireBuffer.substr( entireBuffer.length() - 4 )) == "\r\n\r\n") {
////					cout << "Found it" << endl;
////					break;
////				}
////			}
//
//			//fputs(buffer, stdout);
//			memset(buffer, 0, bufferSize);
//			//cout << "Bytes: " << recvd << endl;
//		}

		cout << entireBuffer << endl;
		cout << entireBuffer.length() << endl;
		string path = extractPath(entireBuffer);
		cout << "Path: " << path << endl;

		sendResponse(newSockFD, path);

		//write(newSockFD, message.c_str(), message.length());
		printf("Message sent from server\n");
		close(newSockFD);
	}

	// TODO free stuff
	return 0;
}

// extract the requested path from the http get request
string extractPath(string httpGET) {

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

void sendResponse(int newSockFD, string path) {
	string message;
	int pos = path.find_last_of("/");
	string fileName;

	cout << "POS: " <<  pos << endl;
	if (path.length() >= pos + 1) {
		fileName = path.substr(pos + 1);
	}
	cout << "got file name successfully: " << fileName  << endl;



	  if (access(fileName.c_str(), F_OK) == 0 && access(fileName.c_str(), R_OK) == 0) {
		  // file exists and has read access
		  cout << "file found with read access" << endl;
		  const int bufferSize = 1024; // TODO buffer size, does it matter?
		  		char buffer[bufferSize] = { 0 };

		  		FILE *file = fopen(fileName.c_str(), "r");

		  		while (fgets(buffer, bufferSize, file)) {
		  			send(newSockFD, buffer, strlen(buffer), 0);
		  			memset(buffer, 0, bufferSize);
		  		}
	  }
	  else {
		  cout << "File does not exist" << endl;
		  message = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\n";
		  		write(newSockFD, message.c_str(), message.length());
	  }
}
