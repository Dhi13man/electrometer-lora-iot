/**
 * @file power_sensors_interface.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains interfaces to control the Energy Monitoring sensor interfaces.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<Arduino.h>
#include <Filters.h>
#include <EmonLib.h>

#include "services/logger.hpp"

/**
 * @brief Interface to control sensor interfaces responsible for calculating power.
 * 
 */
class PowerSensorsInterface {
    private:
        /// Current Sensor Pin
        uint8_t currentSensorPin;

        /// Voltage Sensor Pin
        uint8_t voltageSensorPin;

        /// Create statistics to look at the raw test signal
        RunningStatistics inputStats;

        /// Energy monitor approach for measuring
        EnergyMonitor emon;

        /// Calibration Slope to be derived from testing
        float slope;

        /// Calibration Intercept to be derived from testing
        float intercept;

        /// The logger to use for logging.
        Logger *logger;

    public:
        /**
         * @brief Construct a new Sensor Interface object.
         * 
         * @param currentSensorPin The pin that the current sensor is connected to.
         * @param voltageSensorPin The pin that the voltage sensor is connected to.
         * @param slope The slope to be derived from testing.
         * @param intercept The intercept to be derived from testing.
         * @param testFrequency The test frequency to use for the current sensor.
         * @param verbose Whether or not to log the interface activities.
         */
        PowerSensorsInterface(
            uint8_t currentSensorPin, 
            uint8_t voltageSensorPin, 
            float slope = 0.0752,
            float intercept = 0,
            float testFrequency = 50,
            bool verbose = false
        ) {
            // Set Input Pins
            pinMode(currentSensorPin, INPUT);
            this->currentSensorPin = currentSensorPin;
            pinMode(voltageSensorPin, INPUT);
            this->voltageSensorPin = voltageSensorPin;
            emon.current(currentSensorPin, 1);
            emon.voltage(voltageSensorPin, 234.26, 1.7);

            // Set Calibration Slope and Intercept
            this->slope = slope;
            this->intercept = intercept;

            // Set the window length for the statistics
            const float windowLength = 40.0 / testFrequency;     // How long to average the signal
            inputStats.setWindowSecs(windowLength);

            // Create the logger
            this->logger = new Logger(verbose, "PowerSensors");
        }

        /**
         * @brief Get the RMS Current from the Current sensor.
         * 
         */
        double getRMSCurrentEmon() {
            // emon.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
            // emon.serialprint();
            double result = max(0.0, emon.calcIrms(1480));
            logger->logSerial("Current: " + String(result) + "A", true);
            return result;
        }

        /**
         * @brief Get the RMS Current from the Current sensor.
         * 
         */
        double getRMSCurrent() {
            const uint16_t analogCurrentSensorValue = analogRead(currentSensorPin);
            inputStats.input(analogCurrentSensorValue);  // Log to Stats function
                
            const float out = intercept + slope * inputStats.sigma();
            logger->logSerial("Current: " + String(out) + "A", true);
            return out;
        }

        /**
         * @brief Get the RMS Voltage from the Voltage sensor.
         * 
         */
        double getRMSVoltage() {
            return 240.0;
        }

        /**
         * @brief Get the Power from the current and voltage sensors.
         * 
         * @return double 
         */
        double getPower() {
            const float out = getRMSCurrent() * getRMSVoltage();
            logger->logSerial("Power: " + String(out) + "W", true);
            return out;
        }

        /**
         * @brief Destroy the Sensor Interface object
         * 
         */
        ~PowerSensorsInterface() {
            delete this->logger;
            logger = nullptr;
        }
};
