/**
 * @file Scale.h
 * @brief Defines a Scale class that manages two adapter objects.
 */

#pragma once

namespace arra {

/**
 * @class Scale
 * @brief Manages two adapter objects to provide calibrated scale readings.
 * 
 * This class takes two adapter objects and provides methods to calibrate and get readings.
 * The readings are averaged from both adapters, and the class provides drift and calibration checks.
 * 
 * @tparam TAdapter The type of adapter. Must have `Calibrate` and `GetValue` methods.
 * @tparam TThresholdProvider The type of threshold provider. Must have `get_threshold` method.
 */
template<class TAdapter, class TThresholdProvider>
class Scale 
{

public:
    /**
     * @brief Construct a new Scale object.
     * 
     * @param first The first adapter object.
     * @param second The second adapter object.
     * @param tp The threshold provider object.
     */
    Scale(const TAdapter& first, const TAdapter& second, TThresholdProvider& tp);

    /**
     * @brief Calibrate both adapter objects.
     * 
     * @param refMass The known mass for calibration.
     */
    void Calibrate(const float refMass);

     /**
     * @brief Get the threshold percentage from the mass in grams.
     * 
     * @return float The threshold percentage.
     */
    float GetThreshold(float drift_ref_mass);

    /**
     * @brief Get the calibrated value from the scale.
     * 
     * @return float The calibrated value.
     */
    float GetValue();

   

private:
    TAdapter first_;
    TAdapter second_;
    TThresholdProvider& tp_;
    
    float measured_;
    float driftReference_;

    float driftThreshold_  = 0.7;
    static constexpr float calibThreshold_  = 0.5;
};

} // namespace arra

#include "scale.tpp"