
#pragma once
#include <HX711.h>
// #include <iostream>
// #include <vector>


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

    class DoubleScale {
        public:
            DoubleScale() = default; // Default constructor

            DoubleScale(const int& dout0, const int& sck0, const int& dout1, const int& sck1)
                : scale0_(dout0, sck0), scale1_(dout1, sck1) {
                initialValue_ = GetValue();
                currentValue_ = initialValue_;
            }

            void Config(const scale_config& config) {
                scale0_.Config(config);
                scale1_.Config(config);
                initialValue_ = GetValue();
                currentValue_ = initialValue_;
            }

            void Calibrate() {
                scale0_.Calibrate();
                scale1_.Calibrate();
                initialValue_ = GetValue();
                currentValue_ = initialValue_;
            }

            float GetValue() const {
                return (scale0_.GetValue() + scale1_.GetValue()) / 2.0;
            }            

            bool isDrifting() {
                return checkPercentageChange(initialValue_, driftingThresholdPercentage_);
            }

            bool conditionToCalibrate() {
                return checkPercentageChange(previousValue_, calibrationThresholdPercentage_);
            }

            float ShowValue() {
                previousValue_ = currentValue_;
                currentValue_ = GetValue();

                if (isDrifting()) {
                    return initialValue_;
                }

                if (conditionToCalibrate()) {
                    Calibrate();
                }
                
                return currentValue_;
            }

            

            
        
        private:
            Scale scale0_, scale1_;
            float initialValue_, previousValue_, currentValue_;
            float driftingThresholdPercentage_ = 1.0, calibrationThresholdPercentage_ = 0.5;
        
            bool checkPercentageChange(const float& referenceValue, const float& thresholdPercentage) {
                float percentageChange = ((currentValue_ - referenceValue) / referenceValue) * 100.0;
                return abs(percentageChange) < thresholdPercentage;
            }
    };
}