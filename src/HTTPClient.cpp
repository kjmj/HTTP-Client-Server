#include "HTTPClient.hpp"

using namespace std;

int main(int argc, char *argv[]) {

	bool computeRTT;
	string serverURL;
	string portNum;

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
		cout << "USAGE: ./HTTPClient [-options] server_url port_number" << endl;
		return (EXIT_FAILURE);
	}

	string hostName = parseAddress(serverURL);
	string pathName = parsePath(serverURL);
	cout << "Host Name: " << hostName <<endl;
	cout << "Path Name: " << pathName <<endl;

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
	if ((status = getaddrinfo(hostName.c_str(), portNum.c_str(), &hints,
			&results)) != 0) {
		cout << stderr << "Error in getaddrinfo (client): " << gai_strerror(status) << endl;
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

	string GET = formGET(hostName, pathName);
	cout << GET << endl;

	send(sockFD, GET.c_str(), GET.length(), 0);
	cout << "Request sent from client" << endl;

	while (recv(sockFD, buffer, bufferSize, 0) > 0) {
		fputs(buffer, stdout);
		memset(buffer, 0, bufferSize);
	}

	close(sockFD);

	return 0;
}

string formGET(string hostName, string pathName) {
	string GET = "GET /";
	GET = GET.append(pathName);
	GET = GET.append(" HTTP/1.1\r\n");

	GET = GET.append("Host: ");
	GET = GET.append(hostName);
	GET = GET.append("\r\n");
	GET = GET.append("\r\n");

	return GET;

}
// enter an address in the form www.example.com/path/to/whatever
// and the function returns example.com
string parseAddress(string toParse) {
	return toParse.substr(0, toParse.find("/"));
}

/**
 * returns the path part of a url
 */
string parsePath(string toParse) {
	int pos = toParse.find("/");

	if(pos == string::npos) {
		return "";
	}

	return toParse.substr(pos + 1); // everything after "/"
}


