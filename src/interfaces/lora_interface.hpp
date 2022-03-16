/**
 * @file lora_interface.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains interfaces to modularly control LoRa Communication.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "models/serializable_data.hpp"
#include "models/lora_dto.hpp"
#include "services/logger.hpp"

#pragma once

/**
 * @brief Interface to handle duplex LoRa Communication.
 * 
 */
class LoraInterface {
    private:
        /// The logger to use for logging.
        Logger *logger;

    public:
        /**
         * @brief Construct a new LoRa Interface object.
         * 
         * @param verbose Whether or not to print verbose logs.
         */
        LoraInterface(bool verbose = false) {
            this->logger = new Logger(verbose, "LoraInterface");
        }

        /**
         * @brief Send the LoRa Message.
         * 
         * @param loraDTO The LoRa DTO to send.
         */
        void sendLoraMessage(LoraDTO loraDTO) {
            this->logger->logSerial("Sending LoRa Message", true);
            // Serialize the data list
            String serializedData = loraDTO.toString();
            // TODO: Send message over LoRA
        }

        /**
         * @brief Receive the LoRa Message.
         * 
         * @param String The received LoRa Message.
         * @return LoraDTO The received LoRa data.
         */
        LoraDTO receiveLoraMessage(String message) {
            this->logger->logSerial("Received LoRa Message", true);
            // Deserialize received message
            return LoraDTO::fromString(message);
        }

        /**
         * @brief Destroy the LoRa Interface object
         * 
         */
        ~LoraInterface() {
            delete this->logger;
            this->logger = nullptr;
        }
};