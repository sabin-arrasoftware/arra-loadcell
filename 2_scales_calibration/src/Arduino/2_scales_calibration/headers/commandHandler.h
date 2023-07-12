#pragma once
#include "../hash_map/HashMap.h"
#include "proto.h"
#include "scalesHandler.h"
#include "serial.h"


namespace arra {

    class CommandHandler {
        public:
            typedef void (CommandHandler::*CommandFun)(const byte *);
            typedef void (*CallbackFun)();        

            CommandHandler(byte* buffer) : buffer_(buffer)
            {
                cmds_[CALIBRATE] = &CommandHandler::handle_calibrate;
                cmds_[CONFIG] = &CommandHandler::handle_config;
            }



            // Add callback for given event
            void add_callback(CommandType ct, fn_event cb) {
                shm_.insertNode(ct, cb);
            }

            fn_event get_callback(CommandType cmd) {
                return shm_.getValue(cmd); 
            } 

            void execute_callback(fn_event cb) {
                if (cb) 
                {
                    cb();
                }
            }

            void print_result(fn_event cb) {
                if (!cb) 
                {
                    //write("Key not found");
                    return;
                }

                //write("Calling callback");
            }  

            // void add_callback(CommandType t, CallbackFun cb)
            // {
            //     cbs_[t] = cb;
            // }

        private:
            // Command execution methods
            void handle_calibrate(const byte *buffer)
            {
                cbs_[CALIBRATE];
            }

            void handle_config(const byte *buffer)
            {
                cbs_[CONFIG];
            }

            CommandFun cmds_[LAST];

            CallbackFun cbs_[LAST];

            Scales scales_;

            SHashMap<CommandType, fn_event> shm_;

            byte* buffer_;
        };


}