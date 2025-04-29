#include "MessageQueue.h"

void MessageQueue::pushMessage(const std::string& msg) {
    std::lock_guard<std::mutex> lock(queueMutex);
    messages.push(msg);
}

std::string MessageQueue::popMessage() {
    std::lock_guard<std::mutex> lock(queueMutex);
    if (messages.empty()) return "";

    std::string msg = messages.front();
    messages.pop();
    return msg;
}

bool MessageQueue::isEmpty() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return messages.empty();
}