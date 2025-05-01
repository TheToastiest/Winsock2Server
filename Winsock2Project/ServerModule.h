#ifndef SERVER_MODULE_H
#define SERVER_MODULE_H

#pragma once

class ServerModule {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void cleanup() = 0;
    virtual ~ServerModule() = default;
};

#endif // SERVER_MODULE_H