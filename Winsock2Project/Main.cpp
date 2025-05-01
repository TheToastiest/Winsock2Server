#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Logger.h"
#include "UDPGameServer.h" // Handles UDP socket management
#include "NetworkHandler.h"
#include "GameLogic.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

#pragma comment(lib, "Ws2_32.lib")

std::atomic<bool> gameRunning(true);

int main() {
    // Set up and initialize logging instance
    Logger* logger = new Logger();  // Create the logger once
    UDPGameServer udpGameServer(logger);  // Pass the logger instance

    logger->info("Logger started successfully.");
    logger->flush();  // Ensures immediate writing

    // Set console code page to UTF8
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Starting MMO server...\n";

    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        logger->error("WSAStartup failed", WSAGetLastError());
        logger->flush();
        return -1;
    }
    logger->info("WSAStartup succeeded.");

    // Initialize UDPGameServer (handles socket binding and UDP communication)
    /*UDPGameServer udpGameServer;*/
    udpGameServer.start(); // Now handles ports internally

    // Initialize NetworkHandler (client message management)
    NetworkHandler networkHandler;

    logger->info("Server successfully started.");
    logger->flush();

    // Launch the game loop on a separate thread
    std::thread gameThread([&networkHandler, logger]() {
        constexpr int tickRate = 90;  // Standard MMO tick rate (frames per second)
        while (gameRunning) {
            auto startTime = std::chrono::steady_clock::now();

            logger->debug("Processing network messages...");
            networkHandler.processNetworkMessages();

            logger->debug("Updating game world...");
            updateGameWorld();  // Assuming this is defined in GameLogic.h/CPP

            logger->debug("Synchronizing clients...");
            networkHandler.synchronizeClients();

            auto endTime = std::chrono::steady_clock::now();
            auto elapsed = endTime - startTime;
            auto targetDuration = std::chrono::milliseconds(1000 / tickRate);

            if (elapsed < targetDuration) {
                std::this_thread::sleep_for(targetDuration - elapsed);
            }
            else {
                logger->warn("Game loop is running slower than target tick rate");
            }
        }
        logger->info("Gameplay loop exited.");
        });

    // Main thread waits for manual shutdown via console input
    logger->info("Type a message to broadcast or 'exit' to close the server.");
    while (gameRunning) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit") {
            logger->warn("Shutdown command received. Stopping server...");
            gameRunning = false;
        }
        else if (!input.empty()) {
            logger->info("Broadcasting message: \"{}\"", input);
            networkHandler.broadcastMessage(input);
        }

        logger->info("Waiting for game thread to finish...");
        gameThread.join();

        // Shut down the networkHandler
        if (!networkHandler.shutdownServer()) {
            logger->error("Error occurred during shutdown of one or more client sockets.");
        }
        logger->info("Server shutting down.");

        // Clean up UDP sockets
        udpGameServer.cleanup();

        WSACleanup();
        logger->info("WSACleanup completed. Exiting program.");

        // Cleanup logger instance
        delete logger;

        return 0;
    }
}