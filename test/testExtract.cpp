/*
 ** testing extract path function
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<iostream>
#include <sstream>

#include <unistd.h>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[])
{
    string line = "GET /path/to/fooVar.txt HTTP/1.1";
    
    int pos = line.find("GET ");
    if (pos == 0) {
        string firstDelim = "GET ";
        string stopDelim = " HTTP/1.1";
        
        unsigned firstDelimPos = line.find(firstDelim) + 1;
        cout << "First delim position: " << firstDelimPos << endl;
        
        unsigned endOfFirstDelim = firstDelimPos + firstDelim.length();
        cout << "End of first delim position: " << endOfFirstDelim << endl;

        unsigned lastDelimPos = line.find(stopDelim);
        cout << "Last delim position: " << lastDelimPos << endl;

        // get the path
        string path = line.substr(endOfFirstDelim, lastDelimPos - endOfFirstDelim);
        cout << "Path: " << path << endl;

    }
}
