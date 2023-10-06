#pragma once
#include "proto.h" 

namespace arra {

class CommandHandler {
public:
    // Definitions:

    typedef void (CommandHandler::*CommandFun)(const Buffer&);
    typedef void (*CallbackFun)(const Buffer&);     
    
    // Constructor
    CommandHandler() = default;
    
    void add_callback(CommandType cmd, CallbackFun cb) {
        if(!cmd_is_valid(cmd)){
            return; // Log the error
        }
        cbs_[cmd] = cb;
    }

    void execute(const Buffer& buf) {
        CommandType cmd = getCommandType(buf);
        if(!cmd_is_valid(cmd)){
            return; // Log the error
        }
        (cbs_[cmd])(buf);
    }

private:
    bool cmd_is_valid(const int cmd){
        return cmd >= 0 && cmd < LAST;
    }

private:
    CallbackFun cbs_[LAST];         
};

} // end namepsace arra
