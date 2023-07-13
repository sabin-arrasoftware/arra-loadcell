#pragma once

namespace arra {

    class CommandHandler {
        public:
            typedef void (CommandHandler::*CommandFun)(const byte *);
            typedef void (*CallbackFun)(const byte *);        

            CommandHandler()
            {
                cmds_[CALIBRATE] = &CommandHandler::handle_calibrate;
                cmds_[CONFIG] = &CommandHandler::handle_config;
            }

            void get_command_from_buffer() {
                cmd_ = static_cast<CommandType>(buffer_[0]);
            }   

            // Add callback for given event
            // void add_callback(CommandType ct, fn_event cb) {
            //     shm_.insertNode(ct, cb);
            // }

            // fn_event get_callback(CommandType cmd) {
            //     return shm_.getValue(cmd); 
            // } 

            // void execute_callback(fn_event cb) {
            //     if (cb) 
            //     {
            //         cb();
            //     }
            // }

            // void print_result(fn_event cb) {
            //     if (!cb) 
            //     {
            //         //write("Key not found");
            //         return;
            //     }

            //     //write("Calling callback");
            //}  

            void add_callback(CommandType t, CallbackFun cb)
            {
                cbs_[t] = cb;
            }

            void set_buffer(byte* buffer) {
                buffer_ = buffer;
            }

            // This executes the command contained in cmd_ (fhe first byte from buffer_) using the rest of buffer_ 
            void activate_command() {
                (this->*cmds_[cmd_])(buffer_);
            }

        private:
            // Command execution methods
            void handle_calibrate(const byte *buffer)
            {
                cbs_[CALIBRATE](buffer);
            }
            

            void handle_config(const byte *buffer)
            {
                cbs_[CONFIG](buffer);
            }

            CommandType cmd_;

            CommandFun cmds_[LAST];

            CallbackFun cbs_[LAST];

            byte* buffer_;

            //SHashMap<CommandType, fn_event> shm_;            
        };


}