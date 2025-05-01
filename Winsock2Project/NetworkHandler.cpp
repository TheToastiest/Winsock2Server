#include "Encryption.h"
#include "NetworkHandler.h"
#include <iostream>
#include <spdlog/spdlog.h>

NetworkHandler::NetworkHandler() {
    spdlog::info("NetworkHandler initialized.");
}

NetworkHandler::~NetworkHandler() {
    shutdownServer();
}

void NetworkHandler::addClient(SOCKET client) {
    std::lock_guard<std::mutex> lock(clientMutex);
    connectedClients.push_back(client);
    spdlog::info("Client added. Total clients: {}", connectedClients.size());
}

void NetworkHandler::removeClient(SOCKET client) {
    std::lock_guard<std::mutex> lock(clientMutex);
    connectedClients.erase(std::remove(connectedClients.begin(), connectedClients.end(), client), connectedClients.end());
    spdlog::info("Client removed. Total clients: {}", connectedClients.size());
}

void NetworkHandler::broadcastMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(clientMutex);
    spdlog::trace("Broadcasting message: {}", message);

    for (SOCKET client : connectedClients) {
        sendMessage(client, message);
    }
}

void NetworkHandler::sendMessage(SOCKET client, const std::string& message) {
    int bytesSent = send(client, message.c_str(), static_cast<int>(message.size()), 0);

    if (bytesSent == SOCKET_ERROR) {
        spdlog::warn("Failed to send message to client: {}", WSAGetLastError());
    }
    else {
        spdlog::info("Message sent to client: {}", message);
    }
}

void NetworkHandler::processNetworkMessages() {
    spdlog::trace("Processing network messages...");
    // Placeholder: Implement client message handling here
}

void NetworkHandler::synchronizeClients() {
    spdlog::trace("Synchronizing clients...");
    // Placeholder: Implement synchronization logic here
}

bool NetworkHandler::shutdownServer() {
    std::lock_guard<std::mutex> lock(clientMutex);
    bool success = true;

    for (SOCKET client : connectedClients) {
        if (closesocket(client) == SOCKET_ERROR) {
            spdlog::error("Error shutting down client socket: {}", WSAGetLastError());
            success = false;
        }
    }

    connectedClients.clear();
    spdlog::info("Server shutdown completed.");
    return success;
}