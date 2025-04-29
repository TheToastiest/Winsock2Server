/*
#include "Encryption.h"
#include <stdexcept>
#include <string>
#include <cryptopp/chachapoly.h>  // For ChaChaTLS
#include <cryptopp/osrng.h>        // For AutoSeededRandomPool
#include <cryptopp/filters.h>      // For StringSource and StreamTransformationFilter
#include <cryptopp/hex.h>          // For HexEncoder
#include <cryptopp/hmac.h>         // For HMAC
#include <cryptopp/sha.h>          // For SHA256

// Assertion utility
void assertValid(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// Encrypt data using ChaChaTLS
std::string encryptPacket(const std::string& data, const std::string& key) {
    CryptoPP::ChaChaTLS::Encryption chachaEncryptor;

    // Create a 12-byte IV.
    std::string iv(12, 0);
    CryptoPP::AutoSeededRandomPool rng;
    rng.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(&iv[0]), iv.size());

    // Check for a 256-bit key (32 bytes).
    assertValid(key.size() == 32, "ChaChaTLS requires a 256-bit (32-byte) key.");

    chachaEncryptor.SetKeyWithIV(
        reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size(),
        reinterpret_cast<const CryptoPP::byte*>(iv.data()), iv.size()
    );

    std::string encryptedData;
    CryptoPP::StringSource(data, true,
        new CryptoPP::StreamTransformationFilter(chachaEncryptor,
            new CryptoPP::StringSink(encryptedData)
        )
    );

    // Prepend the IV to the encrypted data.
    return iv + encryptedData;
}

// Decrypt data using ChaChaTLS
std::string decryptPacket(const std::string& encryptedPacket, const std::string& key) {
    CryptoPP::ChaChaTLS::Decryption chachaDecryptor;

    // Extract the 12-byte IV.
    std::string iv = encryptedPacket.substr(0, 12);
    std::string encryptedData = encryptedPacket.substr(12);

    // Check for a 256-bit key (32 bytes).
    assertValid(key.size() == 32, "ChaChaTLS requires a 256-bit (32-byte) key.");

    chachaDecryptor.SetKeyWithIV(
        reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size(),
        reinterpret_cast<const CryptoPP::byte*>(iv.data()), iv.size()
    );

    std::string decryptedData;
    CryptoPP::StringSource(encryptedData, true,
        new CryptoPP::StreamTransformationFilter(chachaDecryptor,
            new CryptoPP::StringSink(decryptedData)
        )
    );

    return decryptedData;
}

// ======================================================================
// HMAC functions for integrity verification using HMAC-SHA256
// ======================================================================

// Generates an HMAC from data using the provided key. The result is in hexadecimal.
std::string generateHMAC(const std::string& data, const std::string& key) {
    std::string mac;
    // Create an HMAC object with SHA256.
    CryptoPP::HMAC<CryptoPP::SHA256> hmac(
        reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size()
    );

    // Process the data and encode the result as hexadecimal.
    CryptoPP::StringSource ss(data, true,
        new CryptoPP::HashFilter(hmac,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(mac), false // false -> lowercase hex output
            )
        )
    );

    return mac;
}

// Verifies that the HMAC computed from data with key matches the expected HMAC.
bool verifyHMAC(const std::string& data, const std::string& key, const std::string& expectedHMAC) {
    std::string computedHMAC = generateHMAC(data, key);
    return computedHMAC == expectedHMAC;
}
*/