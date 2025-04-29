#ifndef TCP_LOGIN_SERVER_H
#define TCP_LOGIN_SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

class TCPLoginServer {
public:
    TCPLoginServer();
    ~TCPLoginServer();

    void start();
    void acceptClient();
    void handleLogin(SOCKET clientSocket);

private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
    int addrLen;
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    void cleanup(); // Releases resources properly
};

#endif // TCP_LOGIN_SERVER_H