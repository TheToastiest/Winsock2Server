#ifndef UDP_GAME_SERVER_H
#define UDP_GAME_SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Logger.h"
#include <string>
#include <vector>
#include <thread>

class UDPGameServer {
private:
    bool running;
    std::vector<std::thread> workerThreads; // Pool for threads

    SOCKET gameplaySocket, movementSocket, diagnosticsSocket; // Dedicated sockets for different communication types
    sockaddr_in gameplayAddr, movementAddr, diagnosticsAddr;

    Logger* logger; 
    void setupSocket(SOCKET& sock, sockaddr_in& addr, int port); // Private helper for socket setup
    void receivePacket(SOCKET sock); // receives udp packets
    void sendPacket(const sockaddr_in& clientAddr, const std::string& data, SOCKET sock); // Sends UDP packets
public: 
    explicit UDPGameServer(Logger* loggerInstance);
    ~UDPGameServer();

    void start(); // Starts the server loop
    void stop();  // Stops the server loop

    void sendData(const sockaddr_in& clientAddr, const std::string& data, SOCKET sock);

    void cleanup(); // Releases sockets and resources
};

#endif // UDP_GAME_SERVER_H