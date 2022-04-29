/**
 * @file main.cpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief A project to create a novel LoRA and Cloud communicating Electric Meter.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<Arduino.h>
#include<heltec.h>

#include "controllers/base_controller.hpp"
#include "controllers/gateway_controller.hpp"
#include "controllers/node_controller.hpp"
#include "models/enums.hpp"

// Define Frequency Band
const LoraBand loraBand = LoraBand::ASIA;

// Define Control Mode
const ControlModes controlMode = ControlModes::NODE;

// Wi-Fi Details
const char* wifiSSID = "Omega_jio2";
const char* wifiPassword = "55465858";

// Encryption Details
const String encryptionKey = "1234567890ABCDEF1234567890ABCDE";

// REST Details
const String deviceID = "QB5ckYt0CS7Yc7swMKPu";
const char* host = "electrometer.netlify.app";

// Define Controller
BaseController *controller;

void setup() {
  // Set up Logging.
  Serial.begin(115200);
  /// OLED and LoRA Init
  Heltec.begin(true, true, false , true);

  switch (controlMode) {
    case ControlModes::NODE:
      controller = new NodeController(
        deviceID,
        A0,
        2,
        encryptionKey,
        loraBand,
        false,
        false,
        false
      );
      break;
    case ControlModes::GATEWAY:
      controller = new GatewayController(
        wifiSSID,
        wifiPassword,
        host,
        encryptionKey,
        loraBand,
        false,
        false,
        true,
        false
      );
      break;
  }
}

void loop() {
  controller->operate();
}
