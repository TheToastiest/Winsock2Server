#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Logger.h"
#include "NetworkHandler.h"
#include "GameLogic.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#pragma comment(lib, "Ws2_32.lib")

bool gameRunning = true;

int main() {
    // Set logging levels and pattern.
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e %l : %v");
    auto Logger = spdlog::basic_logger_mt("basic_logger", "logs/server_log.txt");
    spdlog::set_default_logger(Logger);

    // Set console code page to UTF8.
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Starting MMO server...\n";

    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        spdlog::error("WSAStartup failed: {}", WSAGetLastError());
        return -1;
    }
    spdlog::info("WSAStartup succeeded.");

    // Initialize NetworkHandler.
    NetworkHandler networkHandler;
    if (!networkHandler.startServer()) {
        spdlog::error("Failed to start server.");
        return -1;
    }
    spdlog::info("Server started successfully. Launching game loop...");

    // Launch the game loop on a separate thread.
    // Capture networkHandler by reference so that we can call its member functions.
    std::thread gameThread([&networkHandler]() {
        constexpr int tickRate = 90;  // Standard MMO tick rate (frames per second)
        while (gameRunning) {
            auto startTime = std::chrono::steady_clock::now();

            spdlog::trace("Processing network messages...");
            networkHandler.processNetworkMessages();

            spdlog::trace("Updating game world...");
            updateGameWorld();  // Assuming this is defined in GameLogic.h/CPP

            spdlog::trace("Synchronizing clients...");
            networkHandler.synchronizeClients();

            // Sleep to maintain the desired tick rate.
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / tickRate));
        }
        spdlog::info("Gameplay loop exited.");
        });

    // Main thread waits for manual shutdown via console input.
    while (gameRunning) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit") {
            spdlog::info("Shutdown command received. Stopping server...");
            gameRunning = false;
        }
    }

    spdlog::info("Waiting for game thread to finish...");
    gameThread.join();

    // Shut down the networkHandler (returns a bool if you choose so, here we ignore its return value).
    if (!networkHandler.shutdownServer()) {
        spdlog::error("Error occurred during shutdown of one or more client sockets.");
    }
    spdlog::info("Server shutting down.");

    WSACleanup();
    spdlog::info("WSACleanup completed. Exiting program.");

    return 0;
}