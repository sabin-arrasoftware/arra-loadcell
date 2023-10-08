/**
 * @file serial.h
 * @brief Defines a Serial class for handling serial communication.
 */

#pragma once
#include "../protocol/proto.h"

namespace arra {

/**
 * @typedef fn_event
 * @brief A function pointer type pointing to functions taking no arguments and returning void.
 */
typedef void (*fn_event)();

/**
 * @class Serial
 * @brief Handles serial communication and processes incoming data using a command handler.
 * 
 * This class provides methods to start serial communication, send messages, read data into a buffer, 
 * and process incoming data using a command handler.
 * 
 * @tparam TCmdHandler The type of command handler. Must have an `execute` method.
 */
template <class TCmdHandler>
class Serial 
{

public:
    /**
     * @brief Construct a new Serial object.
     * 
     * @param serial The HardwareSerial object for communication.
     * @param h The command handler.
     */
    Serial(HardwareSerial& serial, TCmdHandler& h);

    /**
     * @brief Start serial communication.
     */
    void Start();

    /**
     * @brief Process incoming data from serial using the command handler.
     */
    void ProcessSerialData();

private:
    Message read();
    void write(const Message& msg);

private:
    HardwareSerial& serial_; 
    TCmdHandler& ch_;

    static const int baud = 9600;
    static const int BUFFER_SIZE = 16;
};

} // namespace arra

#include "serial.tpp"
