// threshold_provider.cpp

#include "threshold_provider.h"
//#include <math.h>

namespace arra {

// Constructor
ThresholdProvider::ThresholdProvider(const Point points[NUMBER_OF_POINTS]) {
    Li_ = LagrangeInterpolation(points);
}

// Public
float ThresholdProvider::get_threshold_percentage(float mass) const {
    return (Li_.get_y_coordinate(mass) / mass) * 100;
}

}  // namespace arra