/**
 * @file HX711Adapter.h
 * @brief Adapter for the HX711 load cell amplifier.
 * 
 * This file provides an adapter for the HX711 load cell amplifier, allowing for easy calibration and weight reading.
 */

#pragma once
#include <HX711.h>

namespace arra {

/**
 * @class HX711Adapter
 * @brief Adapter class for the HX711 load cell amplifier.
 * 
 * This class provides an interface to calibrate and read values from the HX711 load cell amplifier.
 */
class HX711Adapter 
{
    
public:
    /**
     * @brief Construct a new HX711 Adapter object and initialize the HX711.
     * 
     * @param dout Data output pin number.
     * @param sck Clock pin number.
     */
    HX711Adapter(const int dout, const int sck);

    /**
     * @brief Calibrate the HX711 based on a known calibration mass.
     * 
     * This method takes multiple readings from the HX711, averages them, and then calculates a calibration factor.
     * 
     * @param calibrationMass Known mass for calibration.
     */
    void Calibrate(const float calibrationMass);

    /**
     * @brief Get the current value from the HX711 in units based on the calibration.
     * 
     * @return float Weight value based on calibration.
     */
    float GetValue() const;

private:
    mutable HX711 hx711_; ///< HX711 object. Mutable because internal state might change during const operations.
};

} // namespace arra
