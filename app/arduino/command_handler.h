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
class CommandHandler 
{
public:
    /**
     * @typedef CallbackFun
     * @brief A function pointer type for command callbacks.
     */
    typedef Message (*FnRequest)(const Message&);     

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
    void AddCallback(OperationType op, FnRequest cb);

    /**
     * @brief Executes the callback associated with the command type found in the buffer.
     * 
     * @param buf The buffer containing the command.
     */
    Message Execute(const Message& buf);

private:
    FnRequest cbs_[ERROR]; /**< An array of callbacks, indexed by command type. */        
};

} // end namespace arra
