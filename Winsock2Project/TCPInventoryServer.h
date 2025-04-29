#ifndef TCP_INVENTORY_SERVER_H
#define TCP_INVENTORY_SERVER_H

#include "NetworkHandler.h"
#include "MessageQueue.h"
#include <winsock2.h>
#include <vector>
#include <thread>

class TCPInventoryServer {
private:
    SOCKET listenSocket;
    std::vector<SOCKET> connectedClients;
    MessageQueue messageQueue;
    std::vector<std::thread> workerThreads;

    void handleClient(SOCKET clientSocket);
    void processInventoryUpdates();

public:
    TCPInventoryServer();
    ~TCPInventoryServer();
    void start(int port);
};

#endif // TCP_INVENTORY_SERVER_H