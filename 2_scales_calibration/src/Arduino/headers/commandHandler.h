#pragma once
#include "../hash_map/HashMap.h"
#include "proto.h"


namespace arra {
    typedef void (*fn_event)();

    class Serial {

    static const int baud = 9600;
    public:
        Serial(HardwareSerial& serial) : serial_(serial){}

        // Add callback for given event.
        void add_callback(CommandType ct, fn_event cb) {
          shm_.insertNode(ct, cb);
        }

        void start() {
            serial_.begin(baud);
        }

        arra::Message handle_serial_data() { 
            arra::Message message;
            if (!serial_.available())
            {
                resetMessage(message);
            }
            byte* buffer = getBuffer();
            message = decodeMessage(buffer); 
            return message;
        } 

        void write(const String& msg) {
            serial_.println(msg);
        }

        byte* getBuffer() {
            static byte buffer[32]; // Assuming a maximum command length of 32 bytes
            memset(buffer, 0, sizeof(buffer)); // Clear the command buffer

            size_t len = serial_.readBytesUntil('\n', buffer, sizeof(buffer) - 1); // Read command bytes
            buffer[len] = '\0'; // Null-terminate the command buffer

            return buffer;
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
                write("Key not found");
                return;
            }

            write("Calling callback");
        }

    private:
        HardwareSerial& serial_;

        SHashMap<CommandType, fn_event> shm_;
    };
}

// #include <Arduino.h>
// #include "../hash_map/HashMap.h"
// #include "scale.h"

// namespace arra
// {
//   // Define the maximum message content length
//   const size_t MAX_CONTENT_LENGTH = 16;

//   // Structure for holding a message
//   struct Message
//   {
//     byte id;
//     char content[MAX_CONTENT_LENGTH];
//   };

//   // Enum for command types
//   enum CommandType
//   {
//     CALIBRATE = 0,
//     CONFIG,
//     LAST
//   };

//   class CommandHandler
//   {
//   public:
//     typedef void (CommandHandler::*CommandFun)(const char *);
//     typedef void (*CallbackFun)();

    

//     CommandHandler(arra::Scale *scales) : scales_(scales)
//     {
//       cmds_[CALIBRATE] = &CommandHandler::handleCalibrate;
//       cmds_[CONFIG] = &CommandHandler::handleConfig;
//     }

//     void AddCallback(CommandType t, CallbackFun cb)
//     {
//       cbs_[t] = cb;
//     }

//     // Method to handle the received command
//     void handleCommand(const Message &message)
//     {
//       if (message.id < 0 || message.id >= LAST)
//       {
//         return;
//       }

//       // Get the function pointer from the map
//       (this->*cmds_[message.id])(message.content);
//     }

//   private:
//     // Command execution methods
//     void handleCalibrate(const char *content)
//     {

//       int scaleIndex = atoi(content) - 1;

//       if (scaleIndex < 0 || scaleIndex >= sizeof(scales_) / sizeof(scales_[0]))
//       {
//         return;
//       }

//       cbs_[CALIBRATE];
//     }

//     void handleConfig(const char *content)
//     {
//       // Parse the index
//       int scaleIndex = atoi(content) - 1;

//       mass 4.0 numReads 5

//           if (scaleIndex < 0 || scaleIndex >= sizeof(scales_) / sizeof(scales_[0]))
//       {
//         return;
//       }

//       // mass, numReads = parseContent(content);

//       // Find the position of the first space character
//       const char *spacePos = strchr(content, ' ');
//       if (spacePos == nullptr)
//       {
//         // Invalid content format, handle the error
//         return;
//       }

//       // Parse the float value
//       float mass = atof(spacePos + 1);

//       // Find the position of the second space character
//       const char *secondSpacePos = strchr(spacePos + 1, ' ');
//       if (secondSpacePos == nullptr)
//       {
//         // Invalid content format, handle the error
//         return;
//       }

//       // Parse the int value
//       int numReads = atoi(secondSpacePos + 1);
//     }

//     CommandFun cmds_[LAST];

//     CallbackFun cbs_[LAST];
//   };

//   void setMessageContent(Message &message, const char *content)
//   {
//     strncpy(message.content, content, MAX_CONTENT_LENGTH);
//   }

// }