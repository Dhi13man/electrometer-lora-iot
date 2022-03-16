/**
 * @file serializable_data.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains a Serializable data model.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

/**
 * @brief Holds particular data to be sent in RESTful request or LoRA Communiation as key value pairs.
 * 
 */
class SerializableData {
    private:
        /// The key of the data.
        String key;

        /// The value of the data.
        String val;

    public:
        /**
         * @brief Construct a new Rest Data object
         * 
         * @param key The key of the data.
         * @param val The value of the data.
         */
        SerializableData(String key="", String val="") {
            this->key = key;
            this->val = val;
        }

        /**
         * @brief Get the Key of the data.
         * 
         * @return String The key of the data.
         */
        String getKey() {
            return this->key;
        }

        /**
         * @brief Get the Value of the data.
         * 
         * @return String The value of the data.
         */
        String getVal() {
            return this->val;
        }

        /**
         * @brief Convert the RestData object to a String.
         * 
         */
        String toString() {
            return String(this->key) + "=" + this->val;
        }

        /**
         * @brief Convert serialized String to RestData object.
         * 
         */
        static SerializableData fromString(String serialized) {
            String key = serialized.substring(0, serialized.indexOf("="));
            String val = serialized.substring(serialized.indexOf("=") + 1);
            return SerializableData(key, val);
        }

        /**
         * @brief Destroy the Rest Data object
         * 
         */
        ~SerializableData() { }
};