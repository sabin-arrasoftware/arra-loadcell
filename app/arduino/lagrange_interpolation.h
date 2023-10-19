/**
 * @file lagrange_interpolation.h
 * @brief Defines a LagrangeInterpolation class for managing interpolation calculations.
 */

#pragma once

namespace arra {

// Number of points in Lagrange interpolation graph
static const int NUMBER_OF_POINTS = 3;

// Define a structure for a Point
struct Point {
    float x;
    float y;
};

/**
 * @class LagrangeInterpolation
 * @brief Provides interpolation calculations.
 * 
 * This class provides methods for interpolation calculations.
 * It can be used with the ThresholdProvider class to calculate interpolated values and percentages.
 */

class LagrangeInterpolation
{
public:
    /**
     * @brief Construct a new LagrangeInterpolatiom object.
     */
    LagrangeInterpolation() = default;
    
    /**
     * @brief Construct a new LagrangeInterpolation object.
     * 
     * @param points The points for interpolation.
     */
    LagrangeInterpolation(const Point points[NUMBER_OF_POINTS]);

    /**
     * @brief Perform Lagrange interpolation for a given x value.
     * 
     * @param x The x value for interpolation.
     * @return float The interpolated value.
     */
    float get_y_coordinate(float x) const;

private:
    Point points_[NUMBER_OF_POINTS];

};

} // namespace arra
