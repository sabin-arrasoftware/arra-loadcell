/**
 * @file proto.h
 * @brief Defines the communication protocol structures and functions for handling messages.
 */

#pragma once
#include <stdint.h>
#include <string.h>
#include <stdio.h>
typedef uint8_t byte;

namespace arra {

/**
 * @brief The header size.
 */
const byte HEADER_SIZE = 3;

/**
 * @brief The maximum buffer size for communication.
 */
const byte PAYLOAD_SIZE = 16;

/**
 * @brief The maximum number of scales supported.
 */
const byte MAX_NR_SCALES = 4;

/**
 * @enum OperationType
 * @brief Enumerates the types of operations supported by the protocol.
 */
enum OperationType 
{
    CALIBRATE = 0, /**< Calibration operation. */
    WEIGHT,        /**< Weight measurement operation. */
    ERROR          /**< Also Sentinel value. Used to determine the number of operations. */
};

/**
 * @enum MessageType
 * @brief Enumerates the types of messages supported by the protocol.
 */
enum MessageType
{
    REQUEST = 0,
    RESPONSE
};

/**
 * @enum ErrorCode
 * @brief Enumerates the types of error codes supported by the protocol.
 */
enum ErrorCode 
{
    ERR_UNKNOWN_REQUEST = 0,  /**< The request type is not recognized. */
    ERR_HEADER_TRUNCATED,     /**< The header was not fully received. */
    ERR_PAYLOAD_TRUNCATED,    /**< The payload was not fully received based on the expected size. */
    ERR_INVALID_PAYLOAD_SIZE, /**< The size of the payload specified in the header exceeds the maximum allowed. */
    ERR_INVALID_SCALE_INDEX   /**< The provided scale index is unknown for us. */
};

/**
 * @struct Message
 * @brief Represents a message for communication.
 * 
 * This structure holds the message type, operation type, payload size and the payload data for communication.
 */
#pragma pack(push, 1)
struct Message 
{
    byte operationType_;
    byte messageType_;
    byte payloadSize_;
    byte payload_[PAYLOAD_SIZE]; /**< Fixed-size payload. */

    char* ToString() 
    {
        static char buffer[256]; 

        snprintf(buffer, sizeof(buffer), "OperationType: %d, MessageType: %d, PayloadSize: %d, Payload: %s",
                 operationType_, messageType_, payloadSize_, payload_);

        return buffer;
    }
};
#pragma pack(pop)

/**
 * @struct CalibrateRequest
 * @brief Represents a calibration request message.
 */
struct CalibrateRequest 
{
    byte scaleIndex_;          /**< The index of the scale to calibrate. */
    float calibrationMass_;    /**< The calibration mass value. */

    void FromMessage(const Message& msg);
    Message ToMessage();
};

/**
 * @struct CalibrateResponse
 * @brief Represents a calibration response message.
 */
struct CalibrateResponse 
{
    bool success_;              /**< Indicates if the calibration was successful. */

    void FromMessage(const Message& msg);
    Message ToMessage();
};

/**
 * @struct WeightRequest
 * @brief Represents a weight measurement request message.
 */
struct WeightRequest 
{
    void FromMessage(const Message& msg);
    Message ToMessage();
};

/**
 * @struct WeightResponse
 * @brief Represents a weight measurement response message.
 */
struct WeightResponse 
{
    byte numberOfScales_;                      /**< The number of scales for which weights are provided. */
    float floatWeight_[MAX_NR_SCALES];         /**< The weight values for each scale. */

    void FromMessage(const Message& msg);
    Message ToMessage();
};

/**
 * @struct ErrorResponse
 * @brief Represents a response for an unknown or unsupported request.
 */
struct ErrorResponse 
{
    byte errorCode_; /**< An error code indicating the type of error or reason for the unknown request. */

    void FromMessage(const Message& msg);
    Message ToMessage();
};

Message createErrorResponse(const ErrorCode ec);

} // namespace arra
