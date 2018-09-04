# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -g -Wall

all: TCPClient TCPServer

# rules for making TCPClient
TCPClient: TCPClient.o
	CC $(CFLAGS) TCPClient.o -o TCPClient

TCPClient.o: TCPClient.c
	CC $(CFLAGS) -c TCPClient.c

# rules for making TCPServer
TCPServer: TCPServer.o
	CC $(CFLAGS) TCPServer.o -o TCPServer

TCPServer.o: TCPServer.c
	CC $(CFLAGS) -c TCPServer.c

clean:
	rm -f *.o $(all) TCPClient TCPServer
	rm -f -r html latex

# Give command "make docs" to create documentation.    
docs:
	doxygen
