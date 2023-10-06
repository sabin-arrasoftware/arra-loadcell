#pragma once
#include <HX711.h>

namespace  arra {

class HX711Adapter {   
public:
    HX711Adapter(const int dout, const int sck) {
        hx711_.begin(dout, sck);
        hx711_.tare();    
    } 

    void Calibrate(const float calibrationMass) {
        static const int numReadings = 10; // keep it here.
        float scale_weight = 0.0;
        for (int i = 0; i < numReadings; ++i) {
            scale_weight += hx711_.get_value();
        }
        float calibrationFactor = scale_weight / numReadings/ calibrationMass;
        hx711_.set_scale(calibrationFactor);
    }

    float GetValue() const {
        return hx711_.get_units();
    }

private:
    mutable HX711 hx711_;           
};

}