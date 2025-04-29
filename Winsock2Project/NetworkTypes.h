#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

#include <winsock2.h>

struct Player {
    SOCKET socket;
    sockaddr_in address;
    char username[32];
};

#endif // NETWORK_TYPES_H