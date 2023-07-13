#pragma once
#include "proto.h"

namespace arra {
    typedef void (*fn_event)();

    class Serial {

    static const int baud = 9600;
    public:
        Serial(HardwareSerial& serial) : serial_(serial){}        

        void start() {
            serial_.begin(baud);
        }

        bool available() {
            return serial_.available();
        }

        Message handle_serial_data() { 
            Message message;
            if (!serial_.available())
            {
                reset_message(message);
            }
            byte* buffer = get_buffer();
            message = decode_message(buffer); 
            return message;
        } 

        void write(const String& msg) {
            serial_.println(msg);
        }

        byte* get_buffer() {
            static byte buffer[16]; // Assuming a maximum command length of 16 bytes
            memset(buffer, 0, sizeof(buffer)); // Clear the command buffer

            size_t len = serial_.readBytesUntil('\n', buffer, sizeof(buffer) - 1); // Read command bytes
            buffer[len] = '\0'; // Null-terminate the command buffer

            return buffer;
        }

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

             

    private:
        HardwareSerial& serial_;        
    };

}

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