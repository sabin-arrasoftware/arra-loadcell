// lagrange_interpolation.cpp

#include "lagrange_interpolation.h"

namespace arra {

// Constructor with points
LagrangeInterpolation::LagrangeInterpolation(const Point points[NUMBER_OF_POINTS]) {
    // Initialize the points_ array with the provided points
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        points_[i] = points[i];
    }
}


// Public 
// Calculate the corresponding y coordinate to a given x coordinate
float LagrangeInterpolation::get_y_coordinate(float x) const {
    float result = 0.0;
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        float term = points_[i].y;
        for (int j = 0; j < NUMBER_OF_POINTS; j++) {
            if (i != j) {
                term *= (x - points_[j].x) / (points_[i].x - points_[j].x);
            }
        }
        result += term;
    }
    return result;
}

} // namespace arra
