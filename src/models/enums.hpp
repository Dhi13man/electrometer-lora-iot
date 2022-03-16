/**
 * @file enums.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains enums required by the project.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

/**
 * @brief The Country whose Frequency Band is being used for LoRa Communication
 * 
 */
enum LoraBand {
    ASIA,
    EUROPE,
    NORTHAMERICA
};


/// The Control Mode types available to be used by the Robot.
enum ControlModes {
    NODE,
    GATEWAY,
    TEST
};