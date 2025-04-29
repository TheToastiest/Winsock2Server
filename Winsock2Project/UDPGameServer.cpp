#include "UDPGameServer.h"
#include "Encryption.h"     // Only if you need encryption; remove if not using currently.
#include <iostream>
#include <string>
#include <cstdlib>          // For exit()
#include <thread>           // For std::thread

#pragma comment(lib, "ws2_32.lib")

// Demo encryption key; if you're using encryption.
const std::string encryptionKey = "0123456789abcdef0123456789abcdef";

UDPGameServer::UDPGameServer()
    : running(true)  // Set the running flag to true
{
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create UDP socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(6000);

    // Bind socket
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }
}

UDPGameServer::~UDPGameServer() {
    cleanup();
}

void UDPGameServer::cleanup() {
    std::cout << "Cleaning up UDP Game Server resources...\n";
    // Close the socket if it is still open.
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
    WSACleanup();
}

void UDPGameServer::stop() {
    // Stop the server by setting running to false.
    running = false;
    // Optionally, you can call cleanup() to force the socket to close.
    cleanup();
}

void UDPGameServer::start() {
    std::cout << "UDP Game Server running on port 6000...\n";
    std::cout << "Type \"exit\" (or \"quit\") to end the server.\n";

    // Launch a thread to listen for console input.
    std::thread inputThread([this]() {
        std::string command;
        while (std::getline(std::cin, command)) {
            if (command == "exit" || command == "quit") {
                std::cout << "Shutdown command received. Ending server...\n";
                stop();
                break;
            }
        }
        });

    // Main server loop: keep receiving packets until 'running' is false.
    while (running) {
        receivePacket();
    }

    if (inputThread.joinable()) {
        inputThread.join();
    }
}

void UDPGameServer::receivePacket() {
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    // Increase buffer size if using encryption-related overhead.
    char buffer[2048] = { 0 };

    // This call will block until a packet is received or until the socket is closed.
    bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0,
        reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
    if (bytesReceived == SOCKET_ERROR) {
        // When we call stop(), the socket is closed, and recvfrom will fail.
        if (running) {
            std::cerr << "Failed to receive packet: " << WSAGetLastError() << std::endl;
        }
        return;
    }

    // Here you may include decryption if encryption is active.
    std::string receivedData(buffer, bytesReceived);
    std::cout << "Received packet: " << receivedData << "\n";
}

void UDPGameServer::sendPacket(const sockaddr_in& clientAddr, const std::string& data) {
    // If encryption is desired, encrypt 'data' before sending.
    // For now, we'll just send the plaintext.
    bytesSent = sendto(serverSocket, data.c_str(), static_cast<int>(data.size()), 0,
        reinterpret_cast<const sockaddr*>(&clientAddr), sizeof(clientAddr));
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Failed to send packet: " << WSAGetLastError() << std::endl;
    }
    else {
        std::cout << "Sent packet to client.\n";
    }
}