/**
 * @file wifi_handler.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains a simple Wi-Fi functionality handler.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include "services/logger.hpp"

/**
 * @brief Modularly helps perform all functionality associated with WiFi network.
 * 
 */
class WiFiHandler {
    private:
        /// The SSID of the Wi-Fi to connect to.
        const char *ssid;

        /// The password of the Wi-Fi to connect to.
        const char *password;

        /// The logger to use for logging.
        Logger *logger;

        /// Secure WiFi Client to make HTTP Requests with.
        WiFiClientSecure *client;

    public:
        /**
         * @brief Construct a new Wi Fi Handler object.
         * 
         * @param ssid The SSID of the Wi-Fi to connect to.
         * @param password The password of the Wi-Fi to connect to.
         * @param verbose Whether or not to log the interface activities.
         */
        WiFiHandler(const char *ssid, const char *password, bool verbose = false) {
            this->ssid = ssid;
            this->password = password; 
            this->logger = new Logger(verbose, "WiFiHandler");
        }

        /**
         * @brief Connect to the Wi-Fi network specified by the SSID and password.
         * 
         */
        void connectWiFi() {
            WiFi.begin(ssid, password);
            logger->logSerial("Connecting to " + String(ssid), true);
            while (WiFi.status() != WL_CONNECTED) {
                delay(1000);
                logger->logSerial(".");
            }
            logger->logSerial("", true);
        }

        /**
         * @brief Send a GET request to the REST backend.
         * 
         * @param host The host to send the request to.
         * @return String The response from the REST backend.
         */
        bool connectClient(String host) {
            client = new WiFiClientSecure();
            logger->logSerial("Connecting to " + host, true);

            // Use WiFiClient class to create TCP connections
            const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
            client->setInsecure(); // this is the magical line that makes everything work
            return client->connect(host.c_str(), httpPort);
        }

        /**
         * @brief Make a request using the HTTP client.
         * 
         * @param request The full request to send.
         * @return size_t The size of the response.
         */
        size_t sendRequest(String request) {
            logger->logSerial("Sending request: " + request, true);
            size_t out = client->print(request);
            delete client;
            return out;
        }

        /**
         * @brief Get the IP address of the device on the Wi-Fi.
         * 
         * @return String the IP Address of the device or blank if not connected.
         */
        String getIP() {
            return (getStatus() == WL_CONNECTED) ? WiFi.localIP().toString() : "";
        }

        /**
         * @brief Get the status of the Wi-Fi connection
         * 
         * @return String 
         */
        wl_status_t getStatus() {
            return WiFi.status();
        }

        /**
         * @brief Destroy the WiFiHandler object
         * 
         */
        ~WiFiHandler() {
            delete logger;
            logger = nullptr;
        }
};
