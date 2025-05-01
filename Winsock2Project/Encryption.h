#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <sodium.h>
#include <string>
#include <vector>

class Encryption {
public:
    static bool initialize();  // Call once at startup
    static std::vector<unsigned char> encrypt(const std::string& plaintext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& nonce);
    static std::string decrypt(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& nonce);
};

#endif // ENCRYPTION_H