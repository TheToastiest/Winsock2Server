#include "IOCPServer.h"
#include <iostream>

IOCPServer::IOCPServer() {
    iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (!iocpHandle) {
        std::cerr << "Failed to create IOCP handle: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void IOCPServer::initializeIOCP(int numThreads) {
    for (int i = 0; i < numThreads; ++i) {
        workerThreads.emplace_back(&IOCPServer::workerThreadFunction, this);
    }
}

void IOCPServer::associateSocket(SOCKET socket) {
    if (!CreateIoCompletionPort((HANDLE)socket, iocpHandle, (ULONG_PTR)socket, 0)) {
        std::cerr << "Failed to associate socket with IOCP: " << GetLastError() << std::endl;
    }
}

void IOCPServer::workerThreadFunction() {
    DWORD bytesTransferred;
    ULONG_PTR completionKey;
    OVERLAPPED* overlapped;

    while (true) {
        BOOL success = GetQueuedCompletionStatus(iocpHandle, &bytesTransferred, &completionKey, &overlapped, INFINITE);
        if (!success) {
            std::cerr << "Error in worker thread: " << GetLastError() << std::endl;
            break;
        }

        // Handle received data here...
    }
}

IOCPServer::~IOCPServer() {
    for (auto& thread : workerThreads) {
        if (thread.joinable()) thread.join();
    }
    CloseHandle(iocpHandle);
}