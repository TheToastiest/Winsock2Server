#include "TCPInventoryServer.h"
#include <iostream>

TCPInventoryServer::TCPInventoryServer() {
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
}

void TCPInventoryServer::start(int port) {
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, SOMAXCONN);

    std::cout << "TCPInventoryServer running on port " << port << std::endl;

    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket != INVALID_SOCKET) {
            connectedClients.push_back(clientSocket);
            workerThreads.emplace_back(&TCPInventoryServer::handleClient, this, clientSocket);
        }
    }
}

void TCPInventoryServer::handleClient(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;

        messageQueue.pushMessage(std::string(buffer, bytesReceived));  // Store inventory request
    }
}

void TCPInventoryServer::processInventoryUpdates() {
    while (true) {
        std::string itemUpdate = messageQueue.popMessage();
        if (!itemUpdate.empty()) {
            std::cout << "Processing inventory update: " << itemUpdate << std::endl;
            for (SOCKET client : connectedClients) {
                send(client, itemUpdate.c_str(), itemUpdate.length(), 0);
            }
        }
    }
}

TCPInventoryServer::~TCPInventoryServer() {
    closesocket(listenSocket);
    for (auto& thread : workerThreads) {
        if (thread.joinable()) thread.join();
    }
}