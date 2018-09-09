/*
 ** showip.c -- show IP addresses for a host given on the command line
 */

#include <cstdio>
#include <string>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

	string fileName = "path/test.txt";

	if (access(fileName.c_str(), F_OK) == 0) {

		cout << "file found" << endl;
	}
	else {
		cout << "not found" << endl;
	}
}
