// command_handler.cpp

#include "command_handler.h"

namespace arra {

// Free Functions

/**
 * @brief Checks if the given operation type is known.
 * 
 * @param op The operation type to check.
 * @return True if the operation type is known, false otherwise.
 */
bool isKnownOperation(const OperationType op) 
{
    return op >= 0 && op <= ERROR;
}

// Ctor 
CommandHandler::CommandHandler()
{
    // Initialize the callback array to nullptr
    for (int i = 0; i < ERROR; ++i) {
        cbs_[i] = [](const Message&){return Message();};
    }
}

// Public

void CommandHandler::AddCallback(OperationType op, FnRequest cb) 
{
    if(!isKnownOperation(op))
    {
        return;
    }
    cbs_[op] = cb;
}

Message CommandHandler::Execute(const Message& msg) 
{
    if(!isKnownOperation(OperationType(msg.operationType_)))
    { 
        return createErrorResponse(ERR_UNKNOWN_REQUEST);
    }
    return (cbs_[msg.operationType_])(msg);
}

} // end namespace arra
