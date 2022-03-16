/**
 * @file lora_dto.hpp
 * @author dhi13man (https://www.github.com/dhi13man/)
 * @brief Contains a Serializable LoRa DTO.
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <Arduino.h>

#include "models/serializable_data.hpp"

/**
 * @brief Data model holding the Data to be sent or received from Lora.
 * 
 */
class LoraDTO {
    private:
        /// The list of Serializable data items in the Data Transfer.
        SerializableData *dataList;

        /// The number of Serializable data items in the Data Transfer.
        int dataListSize;

    public:
        /**
         * @brief Construct a new Lora Response object
         * 
         * @param dataList The list of Serializable data items in the Data Transfer.
         * @param dataListSize The number of Serializable data in the Data Transfer.
         */
        LoraDTO(SerializableData *dataList, int dataListSize) {
            this->dataList = dataList;
            this->dataListSize = dataListSize;
        }
        
        /**
         * @brief Deserialize a String into a LoraDTO object.
         * 
         * @param data The String data to deserialize.
         * @return LoraDTO The deserialized LoraDTO object.
         */
        static LoraDTO fromString(String data) {
            int amps = 0;
            for (int i = 0; i < data.length(); i++) {
                amps += data.charAt(i) == '&';
            }
            SerializableData *dataList = new SerializableData[amps + 1];
            int dataListSize = 0;
            int index = 0;
            while ((index < data.length()) && (dataListSize < amps + 1)) {
                int keyEndIndex = data.indexOf("=", index);
                String key = data.substring(index, keyEndIndex);
                index = keyEndIndex + 1;
                int valEndIndex = data.indexOf("&", index);
                String val = data.substring(index, valEndIndex);
                index = valEndIndex + 1;
                dataList[dataListSize] = SerializableData(key, val);
                dataListSize++;
            }
            return LoraDTO(dataList, dataListSize);
        }
        
        /**
         * @brief Get the Data List of the Lora Response.
         * 
         * @return SerializableData* The list of Serializable data items received.
         */
        SerializableData *getDataList() {
            return this->dataList;
        }

        /**
         * @brief Get the Data List Size.
         * 
         * @return int the number of Serializable data items received.
         */
        int getDataListSize() {
            return this->dataListSize;
        }
        
        /**
         * @brief Serialize the Lora Response to a String.
         * 
         * @return String The serialized Lora Response.
         */
        String toString() {
            String serializedData = "";
            for (int i = 0; i < this->dataListSize; i++) {
                serializedData += this->dataList[i].toString();
                if (i != this->dataListSize - 1) {
                    serializedData += "&";
                }
            }
            return serializedData;
        }
};
