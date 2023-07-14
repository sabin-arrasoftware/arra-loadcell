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