/**
 * @file node_controller.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains the LoRA Node controller logic.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "controllers/base_controller.hpp"
#include "interfaces/lora_interface.hpp"
#include "interfaces/sensor_interfaces.hpp"
#include "models/enums.hpp"
#include "models/lora_dto.hpp"
#include "services/crypto.hpp"
#include "services/logger.hpp"

/**
 * @brief The control logic for the microcontroller's operation as a Node.
 * 
 */
class NodeController : public BaseController {
    private:
        /// The Device ID of the node.
        String nodeID;

        /// The interface to use the Current sensor.
        CurrentSensorInterface *currentSensor;

        /// The interface to use the Voltage sensor.
        VoltageSensorInterface *voltageSensor;

        /// The interface to use for LoRa Communication (sending)
        LoraInterface *loraInterface;

        /// The encryption service
        Crypto *cryptoService;
    
    public:
        /**
         * @brief Construct a new Gateway Controller object
         * 
         * @param nodeID The Device ID of the node.
         * @param currentSensorPin The pin that the current sensor is connected to.
         * @param voltageSensorPin The pin that the voltage sensor is connected to.
         * @param loraBand The frequency band to be used for LoRA Communication.
         * @param encryptionKey The key to use for encryption of data in communication.
         * @param verbose Whether or not to log the Gatway Controller activities.
         * @param currentSensorVerbose Whether or not to log the CurrentSensorInterface activities.
         * @param voltageSensorVerbose Whether or not to log the VoltageSensorInterface activities.
         * @param loraInterfaceVerbose Whether or not to log the LoraInterface activities.
         */
        NodeController(
            const String nodeID,
            const uint8_t currentSensorPin,
            const uint8_t voltageSensorPin,
            const String encryptionKey,
            const LoraBand loraBand = LoraBand::ASIA,
            const bool verbose = false,
            const bool currentSensorVerbose = false,
            const bool voltageSensorVerbose = false,
            const bool loraInterfaceVerbose = false
        ) : BaseController(new Logger(verbose, "NodeController")) {
            // Set up device ID
            this->nodeID = nodeID;

            // Set up sensor interfaces
            this->currentSensor = new CurrentSensorInterface(currentSensorPin, currentSensorVerbose);
            this->voltageSensor = new VoltageSensorInterface(voltageSensorPin, voltageSensorVerbose);

            // Set up LoRa interface
            this->loraInterface = new LoraInterface(loraBand, loraInterfaceVerbose);

            // Set up Encryption Service
            this->cryptoService = new Crypto(encryptionKey);
        }

        /**
         * @brief The main operation logic for the microcontroller running as Node.
         * 
         */
        void operate() {
            // String enc = cryptoService->encrypt("hehe sexy sex");
            // String dec = cryptoService->decrypt(enc);
            // Serial.println("enc: " + enc + "; dec: " + dec);
            // Sense needed values
            unsigned long last = millis();
            double cSum = 0, vSum = 0, count = 0;
            while (int(millis() - last) < 5000) {
                cSum += currentSensor->getCalibratedValue();
                vSum += voltageSensor->getCalibratedValue();
                count += 1;
                delay(200);
            }
            // For Serializable Data
            SerializableData dataList[] = {
                SerializableData("deviceID", nodeID),
                SerializableData("current", String(cSum / count)),
                SerializableData("voltage", String(vSum / count)),
            };
            // Send LoRA Message
            LoraDTO dto = LoraDTO(dataList, 3);
            loraInterface->sendLoraMessage(dto, cryptoService);
        }

        /**
         * @brief Destroy the Node Controller object
         * 
         */
        ~NodeController() {
            delete this->currentSensor;
            this->currentSensor = nullptr;
            delete this->voltageSensor;
            this->voltageSensor = nullptr;
            delete this->loraInterface;
            this->loraInterface = nullptr;
            delete this->cryptoService;
            this->cryptoService = nullptr;
        }
};