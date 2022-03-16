/**
 * @file base_controller.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains the base controller that all controllers inherit from.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "services/logger.hpp"

/**
 * @brief Base Controller class that other controllers will inherit from and implement.
 * 
 */
class BaseController {
    protected:
        /// The logger to use for logging.
        Logger *logger;

    public:
        /**
         * @brief Construct a new Base Controller object
         * 
         * @param logger 
         */
        BaseController(Logger *logger) {
            this->logger = logger;
        }

        /**
         * @brief The main operation logic for the controller, ran on each iteration
         * 
         */
        virtual void operate() = 0;

        /**
         * @brief Destroy the Base Controller object
         * 
         */
        virtual ~BaseController() {
            delete this->logger;
            this->logger = nullptr;
        }
};