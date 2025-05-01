#pragma once
#include <vector>
#include "Logger.h"
#include "ServerModule.h"  // Ensure ServerModule is properly included
#include <memory>  // ✅ Include smart pointers

class ServerManager {
private:private:
    Logger* logger;  // ✅ Store the logger instance
    std::vector<std::unique_ptr<ServerModule>> servers;  // ✅ Smart pointer for ownership
public:
    explicit ServerManager(Logger* loggerInstance);
    void registerServer(std::unique_ptr<ServerModule> server);  // ✅ Accept smart pointer
    void startAll();
    void stopAll();
    void cleanup();
};