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

#include <Arduino.h>

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
        void initCtx(String key = "") {
            if (key != nullptr && key != "") {
                AES_init_ctx_iv(ctx, (uint8_t*)key.c_str(), (uint8_t*)"1234567890ABCDEF1234567890ABCDEF");
                initialized = true;
            }
        }

    public:
        /**
         * @brief Construct a new Crypto object
         * 
         * @param key 
         */
        Crypto(String key="") {
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
        String encrypt(String plainText, String key="") {
            initCtx(key);
            // Ensure that encryption context is initialized
            if (!initialized) {
                throw "Crypto context not initialized";
            }
            // Extract plainText to char*
            char textChar[plainText.length() + 1] = {'\0'};
            plainText.toCharArray(textChar, plainText.length());
            // Encrypt
            AES_ECB_encrypt(ctx, (uint8_t*)textChar);
            // Remake String
            return String(textChar);
        }

        /**
         * @brief Encrypts the given string.
         * 
         * @param plainText The string to encrypt.
         * @param key The key to use for encryption (if needed to be overriden)
         * @return String The decrypted string.
         * @throws Error if the context is not initialized, i.e. key was never provided to class.
         */
        String decrypt(String cipherText, String key="") {
            initCtx(key);
            // Ensure that encryption context is initialized
            if (!initialized) {
                throw "Crypto context not initialized";
            }
            // Extract cipherText to char*
            char textChar[cipherText.length() + 1] = {'\0'};
            cipherText.toCharArray(textChar, cipherText.length());
            // Decrypt
            AES_ECB_decrypt(ctx, (uint8_t*)textChar);
            // Remake String
            return String(textChar);
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