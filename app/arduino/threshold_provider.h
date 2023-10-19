/**
 * @file threshold_provider.h
 * @brief Defines a ThresholdProvider class for managing threshold calculations.
 */

#pragma once
#include "lagrange_interpolation.h"

namespace arra {
/**
 * @class ThresholdProvider
 * @brief Provides threshold calculations.
 * 
 * This class provides methods for threshold calculations.
 * It can be used with the Scale class to povide calculated values.
 */
 

class ThresholdProvider
{
public:
    /**
     * @brief Construct a new ThresholdProvider object.
     * 
     * @param points The points to be used for interpolation.
     */
    ThresholdProvider(const Point points[NUMBER_OF_POINTS]);

    /**
     * @brief Calculate the threshold percentage for a given mass in grams.
     * 
     * @param mass_in_grams The mass measured in grams for threshold calculation.
     * @return float The threshold percentage.
     */
    float get_threshold_percentage(float mass) const;

private:
    LagrangeInterpolation Li_;
};

} // namespace arra