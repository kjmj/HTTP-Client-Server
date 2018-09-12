# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall

# the build target executable:
TARGET = HTTPClient HTTPServer

all: $(TARGET)

HTTPClient: HTTPClient.cpp
	$(CC) $(CFLAGS) -o HTTPClient HTTPClient.cpp
	
HTTPServer: HTTPServer.cpp
	$(CC) $(CFLAGS) -o HTTPServer HTTPServer.cpp	

clean:
	$(RM) $(TARGET)