/*
 ** testing file access
 */

#include <cstdio>
#include <string>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

	string path = "testFolder/test.txt";

	if (access(path.c_str(), F_OK) == 0) {
		cout << "file found" << endl;
	}
	else {
		cout << "not found" << endl;
	}
}
