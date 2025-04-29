/*#include <openssl/ssl.h>
#include "openssl/err.h"

void initializeTLS() {
    SSL_library_init();
    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());

    if (!ctx) {
        std::cerr << "Failed to initialize TLS context!" << std::endl;
        return;
    }

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, serverSocket);
    SSL_accept(ssl);
}
*/