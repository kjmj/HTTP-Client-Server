/*
 ** trying to use this instead of setting a set buffer size
 */

#include <cstdio>
#include <string>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
//            while ((recvd = recv(newSockFD, buffer, bufferSize, 0)) > 0) {
//
//                entireBuffer.append(buffer);
//                //cout << entireBuffer << endl;
//                cout << entireBuffer.length() << endl;
//
//
//                if(entireBuffer.find('\r\n\r\n')) {
//                    cout << "end of HTTP request" << endl;
//                    //break;
//                }
//
//                if(strstr(entireBuffer.c_str(), "\r\n\r\n")) {
//
//                    cout << "FOUND IT Eh" << endl;
//                    break;
//                }
//
//                if(entireBuffer.length() > 4) {
//                    if((entireBuffer.substr( entireBuffer.length() - 4 )) == "\r\n\r\n") {
//                        cout << "Found it" << endl;
//                        break;
//                    }
//                }
//
//                //fputs(buffer, stdout);
//                memset(buffer, 0, bufferSize);
//                //cout << "Bytes: " << recvd << endl;
//            }
}
