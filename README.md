# Network Programming & Multithreading (C++)

This repository contains my experiments and learning implementations of **network programming and concurrency in C++ using POSIX system calls and C++ threads**.

The goal of this project is to understand how low-level servers work by implementing different networking models and multithreading patterns from scratch.

## Topics Covered

### Local Sockets (Unix Domain Sockets)
Location: `LocalSocket/`

- Client–Server communication using `AF_UNIX`
- Socket file binding
- IPC between processes on the same machine

### Remote TCP Servers
Location: `RemoteSockets/`

**Iterative Server**
- Handles one client at a time

**Concurrent Server**
- Uses `fork()` to handle multiple clients

**Select Server**
- Uses `select()` for I/O multiplexing

### UDP Sockets
- Datagram communication using `sendto()` and `recvfrom()`
- Stateless server communication

### Multithreading
Location: `Multithreading/`

Examples demonstrating:

- Thread creation (`std::thread`)
- Race conditions
- Mutex synchronization
- Producer–Consumer problem
- Condition variables
- Basic thread pool architecture

## Build

Compile using `g++`.

Example:

```bash
g++ program.cpp -o program