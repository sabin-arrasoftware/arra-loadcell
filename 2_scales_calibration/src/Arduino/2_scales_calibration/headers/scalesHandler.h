#pragma once
#include "scale.h"
#include "proto.h"

namespace  arra {

    class Scales {   
        public:
            Scales() {}

            void Calibrate(const byte* buffer) {
                const int id = static_cast<int>(buffer[1]);
                scales_[id].Calibrate();
            }

            void Config(const byte* buffer) {           
                Message message;
                decode_config_command(buffer, message);
                const int id = message.config.scaleIndex;
                scale_config config;
                config.calibrationMass = message.config.calibrationMass;
                config.numReadings = message.config.numReadings;
                scales_[id].Config(config);
            }

            void AddScale(const int pinOut, const int pinSck, const int id) {
                scales_[id] = Scale(pinOut, pinSck);
            }   

            float GetValueFromIndex(const int index) {
                return scales_[index].GetValue();
            }         

        private:
            Scale scales_[15];
    };
}

// void Calibrate(const char* content) {
            //   const int id = static_cast<int>(content[0]); 
            //   scales_[id].Calibrate(); 
            // }


            // PROTOCOL:
            // byte 0 : id
            // byte 1-3: mass, max value 254, we must divide to 10 to have the value
            // byte 4: numReads, max value 15.
            // void Config(const char* content) {

            //     const int id = content[0];
            //     const int decoded_mass = decodeConfigMass(content);
            //     const float mass = decoded_mass / 100.0;
            //     const int numReads = content[3];
            //     //scales_.getValue(id).Config();
            // }

            // void AddScale(const int pinOut, const int pinSck, const int id) {
            //     //scales_.//create;
            // }

// MessageType, ValueType, Value

// CommandType Calibrate
// ValueType CalibrateIndex
// valoare 0-15

// CommandType Config
// ValueType ConfigMass
// valoare 61

// enum CommandType {
//     Calibrate,
//     Confing,
//     //Etc
// }

// enum ValueType {
//     // Calibrate
//     CalibrateIndex

//     // Config
//     ConfigIndex
//     ConfigMass,
//     ConfigNumReads
// }



// void encodeCalibrateIndex(char* msg, int value){
//     msg <- insert value at corect position;
// }

// int decodeConfigMass(char* msg) {
//     return (content[1] << 8) + (content[2] << 4) + content[3];
// }

// First, create a C++ file with the function you want to use:
// cpp
// Copy code
// // my_cpp_lib.cpp

// extern "C" {
//     int my_cpp_function(int x) {
//         return x * 2;
//     }
// }
// Compile the C++ file into a shared library:
// bash
// Copy code
// g++ -shared -o my_cpp_lib.so my_cpp_lib.cpp
// Then, in your Python file, load the library and call the function:
// python
// Copy code
// # my_python_script.py

// import ctypes

// # Load the shared library
// my_cpp_lib = ctypes.CDLL('./my_cpp_lib.so')

// # Call the function
// result = my_cpp_lib.my_cpp_function(10)
// print(result)  # Output: 20
// This example is very simplified and might not work for your specific use case, especially if your header file includes complex classes or templates. For complex integration, consider using a tool like SWIG or pybind11 as mentioned above.

