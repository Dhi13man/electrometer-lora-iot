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

#include "controllers/base_controller.hpp"
#include "controllers/gateway_controller.hpp"
#include "controllers/node_controller.hpp"
#include "models/enums.hpp"

// Define Frequency Band
const LoraBand loraBand = LoraBand::ASIA;

// Define Control Mode
const ControlModes controlMode = ControlModes::GATEWAY;

// Wi-Fi Details
// const char* wifiSSID = "Elements 2.4Ghz";
// const char* wifiPassword = "enigma1302";
const char* wifiSSID = "Omega_jio2";
const char* wifiPassword = "55465858";

// REST Details
const String deviceID = "QB5ckYt0CS7Yc7swMKPu";
const char* host = "electrometer.netlify.app";

// Define Controller
BaseController *controller;

void setup() {
  // Set up Logging.
  Serial.begin(115200);

  switch (controlMode) {
    case ControlModes::NODE:
      controller = new NodeController(deviceID, 36, 37, loraBand, false, false, false, false);
      break;
    case ControlModes::GATEWAY:
      controller = new GatewayController(wifiSSID, wifiPassword, host, loraBand, false, false, false, false);
      break;
  }
}

void loop() {
  controller->operate();
}
