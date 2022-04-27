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
#include "interfaces/power_sensors_interface.hpp"
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

        /// The interface to use the Electrometer based sensors.
        PowerSensorsInterface *powerSensorInterface;

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
         * @param powerSensorsVerbose Whether or not to log the PowerSensorsInterface activities.
         * @param loraInterfaceVerbose Whether or not to log the LoraInterface activities.
         */
        NodeController(
            const String nodeID,
            const uint8_t currentSensorPin,
            const uint8_t voltageSensorPin,
            const String encryptionKey,
            const LoraBand loraBand = LoraBand::ASIA,
            const bool verbose = false,
            const bool powerSensorsVerbose=false,
            const bool loraInterfaceVerbose=false
        ) : BaseController(new Logger(verbose, "NodeController")) {
            // Set up device ID

            // Set up sensor interfaces
            this->powerSensorInterface = new PowerSensorsInterface(
                currentSensorPin, 
                voltageSensorPin, 
                powerSensorsVerbose
            );

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
            // Sense needed values
            const double iRMS = powerSensorInterface->getRMSCurrent();
            //  const double vRMS = emonSensorInterface->getRMSVoltage();
            // For Serializable Data
            SerializableData dataList[] = {
                SerializableData("deviceID", nodeID),
                SerializableData("current", String(iRMS)),
                SerializableData("voltage", String(244)),
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
            delete this->powerSensorInterface;
            this->powerSensorInterface = nullptr;
            delete this->loraInterface;
            this->loraInterface = nullptr;
            delete this->cryptoService;
            this->cryptoService = nullptr;
        }
};