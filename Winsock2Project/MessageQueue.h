#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <queue>
#include <mutex>
#include <string>

class MessageQueue {
private:
    std::queue<std::string> messages;
    std::mutex queueMutex;

public:
    void pushMessage(const std::string& msg);
    std::string popMessage();
    bool isEmpty();
};

#endif // MESSAGEQUEUE_H#pragma once
