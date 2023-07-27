#pragma once
#include "proto.h"

namespace arra {
    typedef void (*fn_event)();

    template <class TCmdHandler>
    class Serial {

    static const int baud = 9600;
    static const int BUFFER_SIZE = 16;
    
    public:
        Serial(HardwareSerial& serial, TCmdHandler& h) 
        : serial_(serial)
        , ch_(h){} 
        //Serial(Serial_& serial) : serial_(serial){}          

        void start() {
            serial_.begin(baud);
        }

        bool available() {
            return serial_.available();
        }        

        void write(const String& msg) {
            serial_.println(msg);
        }

        byte* get_buffer() {
            static byte buffer[BUFFER_SIZE]; // Assuming a maximum command length of 16 bytes
            memset(buffer, 0, sizeof(buffer)); // Clear the command buffer

            size_t len = serial_.readBytesUntil('\n', buffer, sizeof(buffer) - 1); // Read command bytes
            buffer[len] = '\0'; // Null-terminate the command buffer

            return buffer;
        }
        
        void processSerialData()
        {
            if (available())
            {
                // o singura comanda
                // ch.receive
                ch_.set_buffer(get_buffer());     
                ch_.get_command_from_buffer();
                ch_.activate_command();
            }
            // else ... ch.send
        }            

        
       
         

    private:
        HardwareSerial& serial_; 

        TCmdHandler& ch_;
        //Serial_& serial_;       
    };

}



















// void printWeightMessage(const Message& message)    {
//             if (is_weight_message(message))
//             {
//                     const WeightMessage& weight = get_weight_message(message);
//                     write("Weight Message");
//                     serial_.print("Number of scales: ");
//                     serial_.println(static_cast<byte>(weight.numberOfScales));
//                     for (byte i = 0; i < weight.numberOfScales; i++)
//                     {
//                         serial_print("Scale ");
//                         serial_print(i);
//                         serial_print(": ");
//                         serial_println(weight.floatWeight[i]);
//                     }
//             }
//         }

// void printMessage(const Message& message)    {
//             serial_.print("message.calibrate.scaleIndex: ");
//             serial_.println(message.calibrate.scaleIndex);
//             serial_.print("isCalibrateMessage: ");
//             serial_.println(isCalibrateMessage(message));
//             if (isCalibrateMessage(message))
//             {
//                 const CalibrateMessage& calibrate = getCalibrateMessage(message);
//                 write("Calibrate Message");
//                 serial_.print("Scale Index: ");
//                 serial_.println(static_cast<byte>(calibrate.scaleIndex));
//             }
//             else if (isConfigMessage(message))
//             {
//                 const ConfigMessage& config = getConfigMessage(message);
//                 write("Config Message");
//                 serial_.print("Scale Index: ");
//                 serial_.println(static_cast<byte>(config.scaleIndex));
//                 serial_.print("Calibration Mass: ");
//                 serial_.println(config.calibrationMass);
//                 serial_.print("Num Readings: ");
//                 serial_.println(config.numReadings);
//             }
//             else
//             {
//                 write("Unknown Message");
//             }
//         }

// Message handle_serial_data() { 
        //     Message message;
        //     if (!serial_.available())
        //     {
        //         reset_message(message);
        //     }
        //     byte* buffer = get_buffer();
        //     message = decode_message(buffer); 
        //     return message;
        // } 