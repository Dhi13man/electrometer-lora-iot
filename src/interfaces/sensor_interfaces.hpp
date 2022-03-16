/**
 * @file sensor_interface.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains interfaces to modularly control sensor interfaces.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<Arduino.h>

/**
 * @brief Base Interface to control sensor interfaces.
 * 
 */
class SensorInterface {
    private:
        /// The pin number of the sensor
        uint8_t sensorPin;
    
    protected:
        /// The logger to use for logging.
        Logger *logger;

        /**
         * @brief Get the uncalibrated Sensor Value.
         * 
         * @return uint16_t the uncalibrated sensor value. 
         */
        uint16_t getSensorValue() {
            const uint16_t out = analogRead(this->sensorPin);
            logger->logSerial("Uncalibrated Value: " + String(out), true);
            return out;
        }

    public:
        /**
         * @brief Construct a new Sensor Interface object.
         * 
         * @param sensorPin The pin the sensor is connected to
         * @param logger The logger to use for logging.
         */
        SensorInterface(uint8_t sensorPin, Logger *logger) {
            this->sensorPin = sensorPin;
            this->logger = logger;
        }

        /**
         * @brief Get the Calibrated Sensor Value.
         * 
         * @return double The calibrated sensor value.
         */
        virtual double getCalibratedValue() = 0;

        /**
         * @brief Destroy the Sensor Interface object
         * 
         */
        virtual ~SensorInterface() {
            delete this->logger;
            logger = nullptr;
        }
};

/**
 * @brief An interface for hadling current sensors.
 * 
 */
class CurrentSensorInterface : protected SensorInterface {
    public:
        /**
         * @brief Construct a new Current Sensor Interface object
         * 
         * @param verbose Whether or not to log the interface activities.
         */
        CurrentSensorInterface(uint8_t sensorPin, bool verbose) 
            : SensorInterface(sensorPin, new Logger(verbose, "CurrentSensorInterface")) { }

        /**
         * @brief Get the Calibrated Sensor Value.
         * 
         * @return double The calibrated sensor value.
         */
        double getCalibratedValue() override {
            const double rawValue = getSensorValue();
            const double value = abs(rawValue - 9)/(770 - 9) * 0.03043478260869565217391304347826;
            logger->logSerial("Calibrated Value: " + String(value), true);
            return value;
        }
};

/**
 * @brief An interface for hadling voltage sensors.
 * 
 */
class VoltageSensorInterface : protected SensorInterface {
    public:
        /**
         * @brief Construct a new Voltage Sensor Interface object
         * 
         * @param verbose Whether or not to log the interface activities.
         */
        VoltageSensorInterface(uint8_t sensorPin, bool verbose) 
            : SensorInterface(sensorPin, new Logger(verbose, "VoltageSensorInterface")) { }

        /**
         * @brief Get the Calibrated Sensor Value.
         * 
         * @return double The calibrated sensor value.
         */
        double getCalibratedValue() override {
            double value = 230.0;
            logger->logSerial("Calibrated Value: " + String(value), true);
            return value;
        }
};
