# Multi-Client TCP Echo Server

A simple multi-threaded TCP server in C that handles up to 10 concurrent clients, and 5 pending client connection request using POSIX threads and spinlock.

## Features

- Handles up to 10 simultaneous clients
- Echo server (sends back received data)
- Thread-safe client slot management using TestAndSet spinlock

## Files

- `server.c` - Multi-threaded TCP server
- `client.c` - Simple TCP client


## Configuration

- **Port:** 8040 (defined in both files)
- **Max clients:** 10
- **Buffer size:** 500 bytes

## Architecture
![](https://i.postimg.cc/m2cQgg5S/Screenshot-2026-01-14-022259.png)

### Server
- Main thread creaates a socket binds it which listens on **0.0.0.0:8040** accepts connections and assigns free slots to a client.
- Each client handled by a detached POSIX thread
- Slots tracked with `active` boolean flag
- Spinlock protects shared `clients[]` array

### Client
- Connects to localhost:8040
- Reads user input and sends to server
- Displays echoed response
- Exits on "exit" command or server disconnect


## Limitations

The system uses a textbook **TestAndSet** lock to ensure sycnhronization and prevent racing , but is not truly atomic.