# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -g -Wall

all: HTTPClient HTTPServer

# rules for making HTTPClient
HTTPClient: HTTPClient.o
	CC $(CFLAGS) HTTPClient.o -o HTTPClient

HTTPClient.o: HTTPClient.c
	CC $(CFLAGS) -c HTTPClient.c

# rules for making HTTPServer
HTTPServer: HTTPServer.o
	CC $(CFLAGS) HTTPServer.o -o HTTPServer

HTTPServer.o: HTTPServer.c
	CC $(CFLAGS) -c HTTPServer.c

clean:
	rm -f *.o $(all) HTTPClient HTTPServer
	rm -f -r html latex

# Give command "make docs" to create documentation.    
docs:
	doxygen
