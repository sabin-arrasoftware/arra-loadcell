/**
 * @file command_handler.h
 * @brief Defines the CommandHandler class for handling command callbacks.
 */

#pragma once
#include "../protocol/proto.h"

namespace arra {

/**
 * @class CommandHandler
 * @brief Handles the execution of callbacks based on command types.
 * 
 * This class provides methods to register callbacks for specific command types and to execute 
 * the appropriate callback based on the command type found in a buffer.
 */
class CommandHandler {
public:
    // Definitions:

    /**
     * @typedef CommandFun
     * @brief A member function pointer type for command functions.
     * @note This type is currently unused.
     */
    typedef void (CommandHandler::*CommandFun)(const Buffer&);

    /**
     * @typedef CallbackFun
     * @brief A function pointer type for command callbacks.
     */
    typedef void (*CallbackFun)(const Buffer&);     

    /**
     * @brief Default constructor.
     */
    CommandHandler();

    /**
     * @brief Registers a callback for a specific command type.
     * 
     * @param cmd The command type.
     * @param cb The callback function.
     */
    void AddCallback(CommandType cmd, CallbackFun cb);

    /**
     * @brief Executes the callback associated with the command type found in the buffer.
     * 
     * @param buf The buffer containing the command.
     */
    void Execute(const Buffer& buf);

private:
    /**
     * @brief Checks if the command type is valid.
     * 
     * @param cmd The command type to check.
     * @return bool True if the command type is valid, false otherwise.
     */
    bool isValid(const int cmd);

private:
    CallbackFun cbs_[LAST]; /**< An array of callbacks, indexed by command type. */        
};

} // end namespace arra
