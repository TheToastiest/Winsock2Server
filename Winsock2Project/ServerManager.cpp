#include "ServerManager.h"
#include "Logger.h"  // ✅ Include your custom logging system

ServerManager::ServerManager(Logger* loggerInstance) : logger(loggerInstance) {
    logger->info("ServerManager initialized.");
}// ✅ Assume global or passed-in logger instance

void ServerManager::registerServer(std::unique_ptr<ServerModule> server) {
    logger->info("Registering a new server...");
    servers.push_back(std::move(server));  // ✅ Transfers ownership safely
}

void ServerManager::startAll() {
    logger->info("Starting all registered servers...");
    for (auto& server : servers) {  // ✅ Use `auto&` to avoid raw pointer issues
        server->start();
    }
}

void ServerManager::stopAll() {
    logger->warn("Stopping all registered servers...");
    for (auto& server : servers) {  // ✅ No need to use `auto*`
        server->stop();
    }
}

void ServerManager::cleanup() {
    logger->info("Cleaning up server resources...");
    for (auto& server : servers) {
        server->cleanup();
    }
    servers.clear();  // ✅ Smart pointers automatically clean up memory
    logger->info("Server cleanup complete.");
}