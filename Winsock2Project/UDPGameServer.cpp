#include <thread>
#include "UDPGameServer.h"
#include <iostream>
#include <chrono>

// -------------------- SERVER INITIALIZATION --------------------

UDPGameServer::UDPGameServer(Logger* loggerInstance) : running(true), logger(loggerInstance) {
    logger->info("Initializing UDPGameServer...");

    // Start Windows socket API
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logger->error("WSAStartup failed - Error Code: {}", WSAGetLastError()); // ✅ Correct error formatting
        exit(EXIT_FAILURE);
    }

    // Setup each communication socket for different types of traffic
    setupSocket(gameplaySocket, gameplayAddr, 6000);
    setupSocket(movementSocket, movementAddr, 6001);
    setupSocket(diagnosticsSocket, diagnosticsAddr, 6002);

    logger->info("UDPGameServer started successfully.");
}

UDPGameServer::~UDPGameServer() {
    logger->info("UDPGameServer shutting down...");
    cleanup();
}

// -------------------- SERVER LIFECYCLE --------------------

void UDPGameServer::start() {
    logger->info("Starting UDPGameServer...");
    running = true;

    // ✅ Launch worker threads for packet handling (non-blocking)
    workerThreads.emplace_back([this]() { while (running) receivePacket(gameplaySocket); });
    workerThreads.emplace_back([this]() { while (running) receivePacket(movementSocket); });
    workerThreads.emplace_back([this]() { while (running) receivePacket(diagnosticsSocket); });

    // ✅ Keep the server running until stop() is called
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Prevent CPU overload
    }

    // ✅ Ensure proper shutdown & thread cleanup
    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    logger->info("UDPGameServer stopping...");
    cleanup();
}

    /*
    while (running) {
        logger->trace("Listening for UDP packets...");

        // Process packets from each socket
        receivePacket(gameplaySocket);
        receivePacket(movementSocket);
        receivePacket(diagnosticsSocket);

        // Debug message to track server health
        logger->debug("Server is still running...");
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Prevent CPU overuse
    }

    logger->info("UDPGameServer stopping...");
    cleanup();
}*/

void UDPGameServer::stop() {
    running = false;
    cleanup();
}

// -------------------- SOCKET SETUP --------------------

void UDPGameServer::setupSocket(SOCKET& sock, sockaddr_in& addr, int port) {
    logger->debug("Setting up socket for port {}", port);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        logger->error("Socket creation failed for port {} - Error Code: {}", port, WSAGetLastError()); // ✅ Error with code
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        logger->error("Bind failed for port {} - Error Code: {}", port, WSAGetLastError());  // ❌ Only log error if binding fails
        exit(EXIT_FAILURE);
    }
    else {
        logger->info("Successfully bound to port {}", port);  // ✅ No error code in success logs
    }

    logger->info("Server listening on port {}", port);
}

// -------------------- PACKET HANDLING --------------------

void UDPGameServer::receivePacket(SOCKET sock) {
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[2048] = { 0 };

    while (running) {  // ✅ Keep thread alive while the server is running
        int bytesReceived = recvfrom(sock, buffer, sizeof(buffer), 0,
            reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);

        if (bytesReceived == SOCKET_ERROR) {
            logger->warn("Failed to receive packet - Error Code: {}", WSAGetLastError());
            continue;
        }

        std::string receivedData(buffer, bytesReceived);
        logger->trace("Received UDP packet: {}", receivedData);
    }
}

//    logger->trace("Waiting for UDP packet...");
//
//    int bytesReceived = recvfrom(sock, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
//    if (bytesReceived == SOCKET_ERROR) {
//        logger->warn("Failed to receive packet - Error Code: {}", WSAGetLastError()); // ✅ Correct warning format
//        return;
//    }
//
//    std::string receivedData(buffer, bytesReceived);
//    logger->trace("Received UDP packet: {}", receivedData);
//}
    // Allows private access to sendPacket through sendData
void UDPGameServer::sendData(const sockaddr_in& clientAddr, const std::string& data, SOCKET sock) {
    logger->debug("Sending packet to client...");
    sendPacket(clientAddr, data, sock);  // ✅ Calls private method safely
}

    // ✅ Still private—only accessible via `sendData()`
    void UDPGameServer::sendPacket(const sockaddr_in& clientAddr, const std::string& data, SOCKET sock) {
        int bytesSent = sendto(sock, data.c_str(), static_cast<int>(data.size()), 0,
            reinterpret_cast<const sockaddr*>(&clientAddr), sizeof(clientAddr));

        if (bytesSent == SOCKET_ERROR) {
            logger->warn("Failed to send packet - Error Code: {}", WSAGetLastError());
        }
        else {
            logger->debug("Sent packet successfully: {}", data);
        }
    }



// -------------------- SERVER CLEANUP --------------------

void UDPGameServer::cleanup() {
    logger->info("Cleaning up UDPGameServer resources...");

    // Close all sockets
    closesocket(gameplaySocket);
    closesocket(movementSocket);
    closesocket(diagnosticsSocket);

    WSACleanup();
    logger->info("Server shutdown complete.");
}