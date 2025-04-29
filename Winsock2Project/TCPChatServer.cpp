#include "TCPChatServer.h"
#include <iostream>

TCPChatServer::TCPChatServer() {
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
}

void TCPChatServer::start(int port) {
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, SOMAXCONN);

    std::cout << "TCPChatServer running on port " << port << std::endl;

    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket != INVALID_SOCKET) {
            connectedClients.push_back(clientSocket);
            workerThreads.emplace_back(&TCPChatServer::handleClient, this, clientSocket);
        }
    }
}

void TCPChatServer::handleClient(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;

        messageQueue.pushMessage(std::string(buffer, bytesReceived));  // Store chat message
    }
}

void TCPChatServer::processMessages() {
    while (true) {
        std::string msg = messageQueue.popMessage();
        if (!msg.empty()) {
            std::cout << "Broadcasting chat message: " << msg << std::endl;
            for (SOCKET client : connectedClients) {
                send(client, msg.c_str(), msg.length(), 0);
            }
        }
    }
}

TCPChatServer::~TCPChatServer() {
    closesocket(listenSocket);
    for (auto& thread : workerThreads) {
        if (thread.joinable()) thread.join();
    }
}