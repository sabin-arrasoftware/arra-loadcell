// hx711_adapter.cpp

#include "hx711_adapter.h"

namespace arra {

// Constructor implementation
HX711Adapter::HX711Adapter(const int dout, const int sck) 
{
    hx711_.begin(dout, sck);
    hx711_.tare();  // Reset the scale to 0
}

// Calibrate method implementation
void HX711Adapter::Calibrate(const float refMass) 
{
    static const int numReads = 10;
    float totalVal  = 0.0;
    for (int i = 0; i < numReads; ++i) {
        totalVal += hx711_.get_value();
    }
    float calFactor  = totalVal / numReads / refMass;
    hx711_.set_scale(calFactor);  // Set the calibration factor
}

// GetValue method implementation
float HX711Adapter::GetValue()  
{
    return hx711_.get_units();  // Get the value in units based on calibration
}

} // namespace arra
