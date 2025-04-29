#include "TCPLoginServer.h"
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

TCPLoginServer::TCPLoginServer() {
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(5000);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }
}

TCPLoginServer::~TCPLoginServer() {
    cleanup();
}

void TCPLoginServer::cleanup() {
    std::cout << "Cleaning up TCP Login Server resources...\n";
    closesocket(serverSocket);
    WSACleanup();
}

void TCPLoginServer::start() {
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "TCP Login Server listening on port 5000...\n";

    while (true) {
        acceptClient();
    }
}

void TCPLoginServer::acceptClient() {
    addrLen = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Client connected!\n";

    std::thread loginThread(&TCPLoginServer::handleLogin, this, clientSocket);
    loginThread.detach();
}

void TCPLoginServer::handleLogin(SOCKET clientSocket) {
    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived > 0) {
        std::string username(buffer);
        std::cout << "Login request from: " << username << std::endl;

        if (username == "player123") {
            std::string successMsg = "Login Accepted";
            send(clientSocket, successMsg.c_str(), successMsg.size(), 0);
        }
        else {
            std::string failureMsg = "Login Failed";
            send(clientSocket, failureMsg.c_str(), failureMsg.size(), 0);
        }
    }

    closesocket(clientSocket);
}