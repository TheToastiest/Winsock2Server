#ifndef NETWORK_HANDLER_H
#define NETWORK_HANDLER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <mutex>
#include "Player.h"

// Sends a game update for the provided player.
// This is a stub or placeholder function—you can later fill in real networking code.
void sendGameUpdate(const Player& player);

class NetworkHandler {
private:
    std::vector<SOCKET> connectedClients;
    std::mutex clientMutex;  // Ensure thread safety

public:
    NetworkHandler();
    ~NetworkHandler();
    bool startServer();
    void addClient(SOCKET client);
    void removeClient(SOCKET client);
    void broadcastMessage(const std::string& message);

    // Changed shutdownServer() from void to bool.
    // Returns true if all client sockets were shut down successfully, false otherwise.
	bool shutdownServer(); // Placeholder for server shutdown logic

    void processNetworkMessages(); // Placeholder for processing incoming messages
    void synchronizeClients();       // Placeholder for synchronizing clients

    void sendMessage(SOCKET client, const std::string& message);
};

#endif // NETWORK_HANDLER_H