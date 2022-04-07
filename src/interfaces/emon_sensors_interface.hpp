/**
 * @file emon_sensors_interface.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains interface to control the Energy Monitoring sensor interfaces using EmonLib
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<Arduino.h>
#include "EmonLib.h"

#include "services/logger.hpp"

/**
 * @brief Base Interface to control sensor interfaces.
 * 
 */
class EmonSensorsInterface {
    private:
        /// Initialize Open Energy Monitor Object.
        EnergyMonitor emon;
    
    protected:
        /// The logger to use for logging.
        Logger *logger;

        /**
         * @brief Calibrate and form power related values in Energy Monitor Library.
         */
        void formValues() {
            // Calculate all. No.of half wavelengths (crossings), time-out
            emon.calcVI(20,2000);
        }

    public:
        /**
         * @brief Construct a new Sensor Interface object.
         * 
         * @param currentSensorPin The pin that the current sensor is connected to
         * @param voltageSensorPin The pin that the voltage sensor is connected to
         * @param verbose Whether or not to log the interface activities.
         */
        EmonSensorsInterface(uint8_t currentSensorPin, uint8_t voltageSensorPin, bool verbose = false) {
            emon.current(currentSensorPin, 111.1);
            emon.voltage(voltageSensorPin, 234.26, 1.7);
            this->logger = new Logger(verbose, "WiFiHandler");
        }

        /**
         * @brief Get the Real Power from the Energy Monitor Library.
         * 
         * @return double 
         */
        double getRealPower() {
            formValues();
            return emon.realPower;
        }

        /**
         * @brief Get the Apparent Power from the Energy Monitor Library.
         * 
         */
        double getApparentPower() {
            formValues();
            return emon.apparentPower;
        }

        /**
         * @brief Get the Power Factor from the Energy Monitor Library.
         * 
         */
        double getPowerFactor() {
            formValues();
            return emon.powerFactor;
        }

        /**
         * @brief Get the RMS Voltage from the Energy Monitor Library.
         * 
         */
        double getRMSVoltage() {
            formValues();
            return emon.Vrms;
        }

        /**
         * @brief Get the RMS Current from the Energy Monitor Library.
         * 
         */
        double getRMSCurrent() {
            formValues();
            return emon.Irms;
        }

        /**
         * @brief Calculate and get only the RMS Current from the Energy Monitor Library.
         * 
         */
        double getOnlyRMSCurrent() {
            return emon.calcIrms(1480);
        }

        /**
         * @brief Destroy the Sensor Interface object
         * 
         */
        ~EmonSensorsInterface() {
            delete this->logger;
            logger = nullptr;
        }
};
