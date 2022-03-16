/**
 * @file rest_client.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains an interface to modularly control REST clients.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<Arduino.h>

#include "interfaces/wifi_handler.hpp"
#include "models/serializable_data.hpp"
#include "services/logger.hpp"

/**
 * @brief A simple REST Clients to send requests over the network to a cloud backend.
 * 
 */
class RESTClient {
    private:
        /// The base URL of the REST backend to send requests to.
        String host;
    
        /// The logger to use for logging.
        Logger *logger;

        /// The Client used to make HTTP requests
        WiFiHandler *wifi;

        /**
         * @brief Form the Get Request URL using the endpoint where data is to be sent,
         * 
         * @param urlEndpoint The endpoint where data is to be sent.
         * @param data The data to be sent in the get request as a list of {SerializableData}.
         * @param dataLength The length of the data list.
         * @return String The formed URL with the data encoded in it.
         */
        String formGetRequestURL(
            const String urlEndpoint,
            SerializableData *data,
            const int dataLength
        ) {
            String encodedURL = urlEndpoint + "?";
            for (int i = 0; i < dataLength; i++) {
                encodedURL += data[i].toString();
                if (i != dataLength - 1) {
                    encodedURL += "&";
                }
            }
            logger->logSerial("Requesting URL: ");
            logger->logSerial(encodedURL, true);
            return encodedURL;
        }

    public:
        /**
         * @brief Construct a new REST Client object.
         * 
         * @param host The base URL of the REST backend to send requests to.
         * @param wifi The WiFiHandler to use for sending requests.
         * @param verbose Whether or not to log the interface activities.
         */
        RESTClient(String host, WiFiHandler *wifiHandler, bool verbose = false) {
            this->host = host;
            this->logger = new Logger(verbose, "RESTClient");
            this->wifi = wifiHandler;
        }

        /**
         * @brief Send a GET request to the REST backend.
         * 
         * @param urlEndpoint The base endpoint where data is to be sent.
         * @param data The data to be sent in the get request as a list of {SerializableData}.
         * @param dataLength The length of the data list.
         * @return {size_t} The size of the response.
         */
        size_t makeGETRequest(
            const String urlEndpoint,
            SerializableData *data,
            const int dataLength
        ) {
            logger->logSerial("Sending GET request...");
            const String encodedURL = formGetRequestURL(urlEndpoint, data, dataLength);
            const bool con = wifi->connectClient(host);
            if (wifi->getStatus() == WL_CONNECTED && con) {
                // This will send the request to the server
                size_t res = wifi->sendRequest(
                    "GET " + encodedURL + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" + 
                    "Connection: close\r\n\r\n"
                );
                logger->logSerial("Sent!", true);
                return res;
            } else {
                logger->logSerial("Connection failed! Reconnecting...", true);
                wifi->connectWiFi();
                return 0;
            }
        }

        /**
         * @brief Destroy the RESTClient object
         * 
         */
        ~RESTClient() {
            delete logger;
            logger = nullptr;
        }
};
