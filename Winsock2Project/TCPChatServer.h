#ifndef TCP_CHAT_SERVER_H
#define TCP_CHAT_SERVER_H

#include "NetworkHandler.h"
#include "MessageQueue.h"
#include <winsock2.h>
#include <vector>
#include <thread>

class TCPChatServer {
private:
    SOCKET listenSocket;
    std::vector<SOCKET> connectedClients;
    MessageQueue messageQueue;
    std::vector<std::thread> workerThreads;

    void handleClient(SOCKET clientSocket);  // Thread function for each client
    void processMessages();  // Handles queued chat messages

public:
    TCPChatServer();
    ~TCPChatServer();
    void start(int port);
};

#endif // TCP_CHAT_SERVER_H