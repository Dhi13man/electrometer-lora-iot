/**
 * @file gateway_controller.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains the LoRA Gateway controller logic.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "controllers/base_controller.hpp"
#include "interfaces/rest_client.hpp"
#include "interfaces/lora_interface.hpp"
#include "interfaces/wifi_handler.hpp"
#include "models/enums.hpp"
#include "models/lora_dto.hpp"
#include "models/serializable_data.hpp"
#include "services/crypto.hpp"
#include "services/logger.hpp"

/**
 * @brief The control logic for the microcontroller's operation as a Gateway.
 * 
 */
class GatewayController : public BaseController {
    private:
        /// The REST Client to make requests to the endpoint with.
        RESTClient *restClient;

        /// The Client used to make HTTP requests.
        WiFiHandler *wifi;

        /// The interface to use for LoRa Communication (receiving)
        LoraInterface *loraInterface;

        /// The encryption service
        Crypto *cryptoService;
    
    public:
        /**
         * @brief Construct a new Gateway Controller object
         * 
         * @param wifiSSID The SSID of the Wi-Fi that the Gateway will connect to.
         * @param wifiPassword The password of the Wi-Fi that the Gateway will connect to.
         * @param restHost The base URL of the REST backend to send requests to.
         * @param encryptionKey The key to use for encryption of data in communication.
         * @param loraBand The frequency band to be used for LoRA Communication.
         * @param verbose Whether or not to log the Gatway Controller activities.
         * @param wifiVerbose Whether or not to log the WiFiHandler activities.
         * @param restVerbose Whether or not to log the RESTClient activities.
         * @param loraInterfaceVerbose Whether or not to log the LoraInterface activities.
         */
        GatewayController(
            const char *wifiSSID,
            const char *wifiPassword,
            const char *restHost,
            const String encryptionKey,
            const LoraBand loraBand = LoraBand::ASIA,
            bool verbose = false,
            bool wifiVerbose = false,
            bool restVerbose = false,
            bool loraInterfaceVerbose = false
        ) : BaseController(new Logger(verbose, "GatewayController")) {
            // Set up Wi-Fi Connection.
            this->wifi = new WiFiHandler(wifiSSID, wifiPassword, wifiVerbose);
            this->logger->logSerial(wifi->getIP(), true);

            // Set up the REST Client.
            this->restClient = new RESTClient(restHost, wifi, restVerbose);

            // Set up LoRa interface
            this->loraInterface = new LoraInterface(loraBand, loraInterfaceVerbose);

            // Set up Encryption Service
            this->cryptoService = new Crypto(encryptionKey);
            
            // Connect to Wi-Fi
            this->wifi->connectWiFi();
        }

        /**
         * @brief The main operation logic for the microcontroller running as Gateway.
         * 
         */
        void operate() override {
            LoraDTO dto = loraInterface->receiveLoraMessage(nullptr);
            if (dto.getDataListSize() == 0) {
                logger->logSerial("Nothing to send!", true);
            } else {
                const char* dataSendPath = "/.netlify/functions/server";
                restClient->makeGETRequest(dataSendPath, dto.getDataList(), dto.getDataListSize());
            }
        }

        /**
         * @brief Destroy the Gateway Controller object
         * 
         */
        ~GatewayController() {
            delete this->restClient;
            this->restClient = nullptr;
            delete this->wifi;
            this->wifi = nullptr;
            delete this->loraInterface;
            this->loraInterface = nullptr;
            delete this->cryptoService;
            this->cryptoService = nullptr;
        }
};