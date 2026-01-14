# TCP Client–Server Echo Chat 

A simple TCP-based client–server chat application written in C using POSIX socket APIs.
The server listens for incoming client connections and echoes back any message sent by the client.

## Events

#### **Server**:
1. Create a socket
2. Bind it to a server Address ( here the server socket is listening on **0.0.0.0 : 9099** )
3. Server socket listens on that IP Port for any incoming client connection request, the server has also has the maximum number of pending connections in the kernel buffer configured.
4. Server accepts one of the incoming client connection requeest , Opens a dedicated socket to communicate with the client and serves the client.
5. Reads data from the socket into the server buffer and writes the same data back to the client ( the read is blocking in nature and once the clients closes its connection with the server, it returns 0).
6. Once communication is over ,the communicating socket is closed, but the server listening socket remains open.

#### **Client**:
1. Opens a socket.
2. Connects to a IP address the server socket is listening to and to the port where the server socket is listening . So the Client connects to **127.0.0.1 : 9099**
3. Takes input from user, writes it through the socket and reads it back from server into the client buffer. The client closses connection when user enters Exit.
4. Closes the socket.


### Executing server:
```bsh
    gcc server.c -o server.exe
    ./sever.exe
```

### Executing client:
```bsh
    gcc client.c -o client.exe
    ./client.exe
```
