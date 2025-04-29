/*
#pragma once
#include <cryptopp/chacha.h>
#include "cryptopp/cryptlib.h"
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include "cryptopp/osrng.h"
#include "cryptopp/filters.h"
#include "cryptopp/hex.h"
#include <string>

// Encrypt and decrypt packets
std::string encryptPacket(const std::string& data, const std::string& key);
std::string decryptPacket(const std::string& encryptedPacket, const std::string& key);

// HMAC functions for integrity verification
std::string generateHMAC(const std::string& data, const std::string& key);
bool verifyHMAC(const std::string& data, const std::string& key, const std::string& expectedHMAC);

// Assertion utility function
void assertValid(bool condition, const char* message);
*/