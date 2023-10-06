#include "command_handler.h"

namespace arra {

CommandHandler::CommandHandler()
{
}

void CommandHandler::AddCallback(CommandType cmd, CallbackFun cb) 
{
    if(!cmd_is_valid(cmd))
    {
        return;
    }
    cbs_[cmd] = cb;
}

void CommandHandler::Execute(const Buffer& buf) 
{
    CommandType cmd = getCommandType(buf);
    if(!isValid(cmd)){
        return;
    }
    (cbs_[cmd])(buf);
}

bool CommandHandler::isValid(const int cmd) {
    return cmd >= 0 && cmd < LAST;
}

} // end namespace arra
