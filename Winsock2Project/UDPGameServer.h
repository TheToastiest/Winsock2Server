#ifndef UDP_GAME_SERVER_H
#define UDP_GAME_SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
//#include "Encryption.h"


class UDPGameServer {
private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
    char buffer[1024];
    int bytesReceived;
    int bytesSent;
    const std::string encryptionKey = "0123456789abcdef0123456789abcdef"; // 32-byte key demo
    bool running;
public:
    UDPGameServer();  // Constructor
    ~UDPGameServer(); // Destructor
    void start(); // Start the server loop
    void stop(); // End the server loop
    void receivePacket(); // Receive UDP packet
    void sendPacket(const sockaddr_in& clientAddr, const std::string& data);
	void cleanup(); // Releases resources properly

};

#endif // UDP_GAME_SERVER_H