
#pragma once
#include <HX711.h>

namespace  arra {
    //config
    struct scale_config {
        float calibrationMass = 6.1;
        int numReadings = 10;
    };

    class Scale {   
        public:
            Scale() = default; // Default constructor

            Scale(const int& dout, const int& sck) {
                hx711_.begin(dout, sck);
                hx711_.tare();
            }

            void Config(const scale_config& config) {
                config_ = config;
            }

            void Calibrate() {
              // Calculate average for numReadings
                float scale_weight = 0.0;
                for (int i = 0; i < config_.numReadings; ++i) {
                    scale_weight += hx711_.get_value();
                }
                scale_weight /= config_.numReadings;

                float calibrationFactor = scale_weight / config_.calibrationMass;
                hx711_.set_scale(calibrationFactor);
            }

            float GetValue() const {
                return hx711_.get_units();
            }

        private:
            scale_config config_;
            mutable HX711 hx711_; 
    };
}