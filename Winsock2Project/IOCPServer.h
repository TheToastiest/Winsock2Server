#ifndef IOCP_SERVER_H
#define IOCP_SERVER_H

// IOCPServer.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <vector>
#include <thread>

class IOCPServer {
private:
    HANDLE iocpHandle;  // IOCP Handle
    std::vector<std::thread> workerThreads;  // Thread pool for IOCP

public:
    IOCPServer();  // Constructor
    ~IOCPServer(); // Destructor

    void initializeIOCP(int numThreads);   // Setup IOCP & worker threads
    void associateSocket(SOCKET socket);   // Bind socket to IOCP
    void workerThreadFunction();           // Handles network events

};

#endif // IOCP_SERVER_H