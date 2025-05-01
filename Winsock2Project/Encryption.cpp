#include "Encryption.h"
#include <iostream>

bool Encryption::initialize() {
    return sodium_init() >= 0;
}

std::vector<unsigned char> Encryption::encrypt(const std::string& plaintext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& nonce) {
    if (key.size() != crypto_aead_chacha20poly1305_IETF_KEYBYTES || nonce.size() != crypto_aead_chacha20poly1305_IETF_NPUBBYTES) {
        throw std::invalid_argument("Invalid key or nonce size.");
    }

    std::vector<unsigned char> ciphertext(plaintext.size() + crypto_aead_chacha20poly1305_IETF_ABYTES);
    unsigned long long ciphertext_len;

    crypto_aead_chacha20poly1305_ietf_encrypt(
        ciphertext.data(), &ciphertext_len,
        reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size(),
        NULL, 0, NULL, nonce.data(), key.data());

    ciphertext.resize(ciphertext_len); // Trim unused buffer space
    return ciphertext;
}

std::string Encryption::decrypt(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& nonce) {
    if (key.size() != crypto_aead_chacha20poly1305_IETF_KEYBYTES || nonce.size() != crypto_aead_chacha20poly1305_IETF_NPUBBYTES) {
        throw std::invalid_argument("Invalid key or nonce size.");
    }

    std::vector<unsigned char> plaintext(ciphertext.size());
    unsigned long long plaintext_len;

    if (crypto_aead_chacha20poly1305_ietf_decrypt(
        plaintext.data(), &plaintext_len, NULL,
        ciphertext.data(), ciphertext.size(),
        NULL, 0, nonce.data(), key.data()) != 0) {
        throw std::runtime_error("Decryption failed.");
    }

    return std::string(plaintext.begin(), plaintext.begin() + plaintext_len);
}