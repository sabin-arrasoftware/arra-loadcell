#pragma once
#include "scale.h"
#include "proto.h"

namespace  arra {

    class Scales {   
        public:
            Scales() : nr_scales_(0) {}

            void Calibrate(const byte buffer[BUFFER_SIZE]) {
                // Message message;
                // CalibrateMessage calibrateMessage;
                // buffer_to_message(buffer, message);
                // decode_calibrate_command(message, calibrateMessage);
                // const int id = static_cast<int>(calibrateMessage.scaleIndex);
                const int id = static_cast<int>(buffer[1]);
                if (isNotValidIndex(id)) {
                    return;
                }
                scales_[id].Calibrate();
            }

            void Config(const byte buffer[BUFFER_SIZE]) {           
                Message message;
                ConfigMessage configMessage;
                buffer_to_message(buffer, message);
                decode_config_command(message, configMessage);
                const int id = configMessage.scaleIndex;
                if (isNotValidIndex(id)) {
                    return;
                }
                scale_config config;
                config.calibrationMass = configMessage.calibrationMass;
                config.numReadings = configMessage.numReadings;
                scales_[id].Config(config);
            }

            void AddScale(const int pinOut, const int pinSck, const int id) {
                scales_[id] = Scale(pinOut, pinSck);
                nr_scales_++;                
            }   

            float GetValueFromIndex(const int index) {
                if (isNotValidIndex(index)) {
                    return 0.0f;
                }
                return scales_[index].GetValue();
            }   

            Message getWeightMessage() {
                Message message;
                WeightMessage weightMessage;
                weightMessage.numberOfScales = nr_scales_;
                for (int i = 0; i < nr_scales_; i++) {
                    weightMessage.floatWeight[i] = GetValueFromIndex(i);
                }
                encode_weight_command(weightMessage, message);
                return message;
            }

            


                          

        private:
            Scale scales_[MAX_NR_SCALES];
            int nr_scales_;

            bool isNotValidIndex(const int index) {
                return index < 0 || index >= nr_scales_;
            }   
    };
}