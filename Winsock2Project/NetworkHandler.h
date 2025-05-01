#ifndef NETWORK_HANDLER_H
#define NETWORK_HANDLER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <mutex>
#include "PlayerManager.h"
#include "Player.h"

class NetworkHandler {
private:
    std::vector<SOCKET> connectedClients;  // List of connected client sockets
    std::mutex clientMutex;  // Ensure thread safety

public:
    NetworkHandler();
    ~NetworkHandler();

    void addClient(SOCKET client);
    void removeClient(SOCKET client);
    void broadcastMessage(const std::string& message);

    // Handles shutting down client connections cleanly
    bool shutdownServer();

    void processNetworkMessages();  // Processes incoming client messages
    void synchronizeClients();  // Synchronizes game data between clients

    void sendMessage(SOCKET client, const std::string& message);
};

#endif // NETWORK_HANDLER_H