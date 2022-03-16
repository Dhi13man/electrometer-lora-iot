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

#include <heltec.h>

#include "models/serializable_data.hpp"
#include "models/enums.hpp"
#include "models/lora_dto.hpp"
#include "services/logger.hpp"

#define RST 14

/**
 * @brief Interface to handle duplex LoRa Communication.
 * 
 */
class LoraInterface {
    private:
        /// The logger to use for logging.
        Logger *logger;

        /// The frequency band to be used for LoRA Communication.
        int band;

        //Initialize LoRa module
        void startLoRA() {
            //SPI LoRa pins
            SPI.begin(SCK, MISO, MOSI, SS);
            //setup LoRa transceiver module
            LoRa.setPins(SS, RST, DIO0);

            int counter = 0;
            while (!LoRa.begin(band, false) && counter < 10) {
                Serial.print(".");
                counter++;
                delay(500);
            }
            if (counter == 10) {
                logger->logSerial("Starting LoRa failed!", true); 
            } else {
                logger->logSerial("LoRa Initialization OK!", true);
                LoRa.setSyncWord(0xF3);
            }
        }

    public:
        /**
         * @brief Construct a new LoRa Interface object.
         * 
         * @param loraBand The frequency band to be used for LoRA Communication.
         * @param verbose Whether or not to print verbose logs.
         */
        LoraInterface(LoraBand loraBand = LoraBand::ASIA, bool verbose = false) {
            this->logger = new Logger(verbose, "LoraInterface");

            // Set frequency band
            switch (loraBand) {
                case LoraBand::ASIA:
                    this->band = 433E6;
                    break;
                case LoraBand::EUROPE:
                    this->band = 866E6;
                    break;
                case LoraBand::NORTHAMERICA:
                    this->band = 915E6;
                    break;
            }

            // Initialize LoRa
            Heltec.begin(true, true, verbose , true, this->band);
	        LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
            // this->startLoRA();
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
            //Send LoRa packet to receiver
            LoRa.beginPacket();
            LoRa.print(serializedData);
            LoRa.endPacket();
        }

        /**
         * @brief Receive the LoRa Message.
         * 
         * @return LoraDTO The received LoRa data.
         */
        LoraDTO receiveLoraMessage() {
            int parsed = LoRa.parsePacket();
            this->logger->logSerial(String(parsed), true);
            String message = LoRa.available() ? LoRa.readString() : "";
            if (message.length() > 0) {
                this->logger->logSerial("Received LoRa Message: " + message, true);
            } else {
                this->logger->logSerial("Nothing received!", true);
            }
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