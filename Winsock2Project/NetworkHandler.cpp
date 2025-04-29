#include "NetworkHandler.h"
#include <spdlog/spdlog.h>
#include <iostream>

NetworkHandler::NetworkHandler() {}

void NetworkHandler::addClient(SOCKET client) {
    std::lock_guard<std::mutex> lock(clientMutex);
    connectedClients.push_back(client);
}

void NetworkHandler::removeClient(SOCKET client) {
    std::lock_guard<std::mutex> lock(clientMutex);
    connectedClients.erase(std::remove(connectedClients.begin(), connectedClients.end(), client), connectedClients.end());
}

void NetworkHandler::broadcastMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(clientMutex);
    for (SOCKET client : connectedClients) {
        sendMessage(client, message);
    }
}

void NetworkHandler::sendMessage(SOCKET client, const std::string& message) {
    send(client, message.c_str(), message.length(), 0);
}

NetworkHandler::~NetworkHandler() {
    std::lock_guard<std::mutex> lock(clientMutex);
    for (SOCKET client : connectedClients) {
        closesocket(client);
    }
}

// implement startServer and shutdownServer methods
bool NetworkHandler::startServer() {
	// Placeholder for server start logic
    spdlog::info("Server started.");
    return true;
}

bool NetworkHandler::shutdownServer() {
	// Placeholder for server shutdown logic
    return true;
    spdlog::info("Server shut down.");
}

// Placeholder for processing network messages
void NetworkHandler::processNetworkMessages() {
	// This is a stub. You would normally read from sockets and handle messages here.
	spdlog::trace("Processing network messages...");
}
// Placeholder for synchronizing clients
void NetworkHandler::synchronizeClients() {
	// This is a stub. You would normally send updates to clients here.
	spdlog::trace("Synchronizing clients...");
}
// Sends a game update for the provided player.


// Stub implementation: Logs the update details using spdlog.
void sendGameUpdate(const Player& player) {
    spdlog::trace("Sending update for player ID {} at position ({}, {})",
        player.getID(), player.getX(), player.getY());
    // Here, you would normally serialize the player's state and send it over your network.
}
