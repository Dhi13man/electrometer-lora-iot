#include "services/crypto.hpp"

void main() {
    Crypto crypto("key");
    const String plainText = "Hello World!";
    const String encrypted = crypto.encrypt(plainText);
    const String decrypted = crypto.decrypt(encrypted);
    assert(plainText == decrypted);
}