# HTTP-Client-Server
This is an implementation of the HTTP 1.1 protocol. It contains both client and server code.

## Setup
### To build the project:

`cd src`

`make all`

### First, start the server:

`./HTTPServer portNumber`

### Then, launch the (simple) client:

`./HTTPClient [-options] serverUrl portNumber`

Specify -p as an option to print the Round Trip Time (RTT)

### Alternatively, use your favorite web browser as a client:

`http://localhost:portNumber/index.html`

Just replace `portNumber` with the port you started the server with.

## To clean
`cd src`

`make clean`
