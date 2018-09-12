This is an implementation of the HTTP 1.1 protocol. It contains both 
client and server code.

To build the project:
make all

To run the project:
./HTTPServer portNumber

---and/or---

./HTTPClient [-options] serverUrl portNumber
Specify -p as an option to print the Round Trip Time (RTT)

Start HTTPServer in a terminal window. Then, connect to it by
running HTMLClient in another terminal window/web browser