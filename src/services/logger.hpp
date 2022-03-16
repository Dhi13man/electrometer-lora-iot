/**
 * @file logger.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief A simple dynamicallly allocated Logger to log messages across microcontroller unit.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include<Arduino.h>
#include<heltec.h>

/**
 * @brief Used for logging across the microcontroller unit.
 * 
 */
class Logger {
    private:
        /// Whether to log to the serial port.
        bool verbose;

        /// The interface that is making the log.
        String interface;

    public:
        /**
         * @brief Construct a new Logger object
         * 
         * @param verbose Whether or not to print out the log messages.
         */
        Logger(bool verbose = false, String interface = "") {
            this->verbose = verbose;
            this->interface = interface;
        }

        /**
         * @brief Log a message if verbose is true.
         * 
         * @param msg The message to log.
         * @param newline Whether or not to add a newline to the end of the message.
        */
        void logSerial(String msg, bool newline=false, int serial_port=0) {
            // Add the interface (if specified) to the log.
            if (interface.length() > 0 && newline) {
                msg = interface + "::" + msg;
            }
            if (verbose) {
                switch (serial_port) {
                    case 0:
                        Serial.print(msg);
                        if (newline) {
                            Serial.println();
                        }
                        break;

                    case 1:
                        Serial1.print(msg);
                        if (newline) {
                            Serial1.println();
                        }
                        break;

                    case 2:
                        Serial2.print(msg);
                        if (newline) {
                            Serial2.println();
                        }
                        break;
                }
            }
        }

        /**
         * @brief Log into the display OLED if enabled and available
         * 
         */
        void logOLED(String msg) {
            Heltec.display->clear();
            Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
            Heltec.display->setFont(ArialMT_Plain_10);
            Heltec.display->drawStringMaxWidth(0 , 26 , 128, msg);
            Heltec.display->display();
        }
};