/**
 * @file proto.h
 * @brief Defines the communication protocol structures and functions for handling messages.
 */

#pragma once

namespace arra {

/**
 * @brief The maximum buffer size for communication.
 */
const byte BUFFER_SIZE = 16;

/**
 * @brief The maximum number of scales supported.
 */
const byte MAX_NR_SCALES = 4;

/**
 * @enum CommandType
 * @brief Enumerates the types of commands supported by the protocol.
 */
enum CommandType {
    CALIBRATE = 0, /**< Calibration command. */
    WEIGHT,        /**< Weight measurement command. */
    LAST           /**< Sentinel value. Used to determine the number of commands. */
};

/**
 * @struct Buffer
 * @brief Represents a buffer for communication.
 * 
 * This structure holds the payload data and its size for communication.
 */
struct Buffer {
    byte payload_[BUFFER_SIZE]; /**< The payload data. */
    size_t size_;              /**< The size of the payload. */

    /**
     * @brief Converts the buffer to a string representation.
     * @return String The string representation of the buffer.
     */
    String ToString() const;
};

/**
 * @struct CalibrateMessage
 * @brief Represents a calibration message.
 * 
 * This structure is used to handle calibration messages, converting them to and from buffers.
 */
struct CalibrateMessage {
    /**
     * @brief Default constructor.
     */
    CalibrateMessage();

    /**
     * @brief Converts a buffer to a CalibrateMessage.
     * @param buf The buffer to convert.
     */
    void FromBuffer(const Buffer& buf);

    /**
     * @brief Converts the CalibrateMessage to a buffer.
     * @return Buffer The resulting buffer.
     */
    Buffer ToBuffer();

    byte scaleIndex_;          /**< The index of the scale to calibrate. */
    float calibrationMass_;    /**< The calibration mass value. */
};

/**
 * @struct WeightMessage
 * @brief Represents a weight measurement message.
 * 
 * This structure is used to handle weight measurement messages, converting them to and from buffers.
 */
struct WeightMessage {
    /**
     * @brief Default constructor.
     */
    WeightMessage();

    /**
     * @brief Converts a buffer to a WeightMessage.
     * @param buf The buffer to convert.
     */
    void FromBuffer(const Buffer& buf);

    /**
     * @brief Converts the WeightMessage to a buffer.
     * @return Buffer The resulting buffer.
     */
    Buffer ToBuffer();

    byte numberOfScales_;                      /**< The number of scales for which weights are provided. */
    float floatWeight_[MAX_NR_SCALES];         /**< The weight values for each scale. */
};

/**
 * @brief Checks if a buffer represents a calibration message.
 * @param buffer The buffer to check.
 * @return bool True if the buffer represents a calibration message, false otherwise.
 */
bool IsCalibrateMsg(const Buffer& buffer);

/**
 * @brief Checks if a buffer represents a weight measurement message.
 * @param buffer The buffer to check.
 * @return bool True if the buffer represents a weight measurement message, false otherwise.
 */
bool IsWeightMsg(const Buffer& buffer);

/**
 * @brief Retrieves the command type from a buffer.
 * @param buffer The buffer to check.
 * @return CommandType The type of command represented by the buffer.
 */
CommandType GetCommandType(const Buffer& buffer);

} // namespace arra
