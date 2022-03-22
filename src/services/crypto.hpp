/**
 * @file crypto.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains a Crypto Service.
 * @version 0.1
 * @date 2022-03-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <WString.h>

#include <aes.hpp>

/**
 * @brief A utility class to easily handle encryption/decryption of String for security.
 * 
 */
class Crypto {
    private:
        /// The context of the AES encryption.
        AES_ctx *ctx;

        /// Whether encryption context has been initialized.
        bool initialized;

        /**
         * @brief Initializes the AES context.
         * 
         * @param key The key to use for encryption. If the key is null,
         * the context will not be initialized.
         */
        void initCtx(const char* key = nullptr) {
            if (key != nullptr) {
                AES_init_ctx(ctx, (uint8_t*)key);
                initialized = true;
            }
        }

    public:
        /**
         * @brief Construct a new Crypto object
         * 
         * @param key 
         */
        Crypto(const char* key) {
            initialized = false;
            ctx = new AES_ctx();
            initCtx(key);
        }

        /**
         * @brief Encrypts the given string.
         * 
         * @param plainText The string to encrypt.
         * @param key The key to use for encryption (if needed to be overriden)
         * @return String The encrypted string.
         * @throws Error if the context is not initialized, i.e. key was never provided to class.
         */
        String encrypt(String plainText, char* key = nullptr) {
            initCtx(key);
            // Ensure that encryption context is initialized
            if (!initialized) {
                throw "Crypto context not initialized";
            }
            // Extract plainText to char*
            char* plaintext_c = new char[plainText.length() + 1];
            plainText.toCharArray(plaintext_c, plainText.length() + 1);
            // Encrypt
            AES_CTR_xcrypt_buffer(ctx, (uint8_t*)plaintext_c, plainText.length());
            // Remake String and free memory
            const String out = String(plaintext_c);
            delete plaintext_c;
            return out;
        }

        /**
         * @brief Encrypts the given string.
         * 
         * @param plainText The string to encrypt.
         * @param key The key to use for encryption (if needed to be overriden)
         * @return String The decrypted string.
         * @throws Error if the context is not initialized, i.e. key was never provided to class.
         */
        String decrypt(String cipherText, char* key = nullptr) {
            initCtx(key);
            // Ensure that encryption context is initialized
            if (!initialized) {
                throw "Crypto context not initialized";
            }
            // Extract cipherText to char*
            char* ciphertext_c = new char[cipherText.length() + 1];
            cipherText.toCharArray(ciphertext_c, cipherText.length() + 1);
            // Decrypt
            AES_CTR_xcrypt_buffer(ctx, (uint8_t*)ciphertext_c, cipherText.length());
            // Remake String and free memory
            const String out = String(ciphertext_c);
            delete ciphertext_c;
            return out;
        }

        /**
         * @brief Check if the service is ready.
         * 
         * @return bool Whether the service is ready.
         */
        bool isReady() {
            return initialized;
        }

        /**
         * @brief Destroy the Crypto object
         * 
         */
        ~Crypto() {
            delete ctx;
        }
};