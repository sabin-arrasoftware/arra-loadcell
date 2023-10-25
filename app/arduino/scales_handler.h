/**
 * @file scales_handler.h
 * @brief Defines a ScalesHandler class that manages multiple scales.
 */

#pragma once
#include "proto.h"

namespace arra {

/**
 * @class ScalesHandler
 * @brief Manages multiple scales and provides methods to calibrate and get readings.
 * 
 * This class can handle multiple scales, calibrate them based on messages, and retrieve weights.
 * 
 * @tparam TScale The type of scale. Must have `GetWeight` and `GetValue` methods.
 */
template <class TScale, class TScaleFactory>
class ScalesHandler 
{
public:
    /**
     * @brief Construct a new ScalesHandler object.
     */
    ScalesHandler(TScaleFactory& factory);

    /**
     * @brief Calibrate a scale based on a given message.
     * 
     * @param msg The Message containing calibration data.
     *
     * @return Message The message containing the weight message.
     */
    Message Calibrate(const Message& msg);

    /**
     * @brief Add a scale to the handler.
     * 
     * @param scale The scale to be added.
     */
    Message AddScale(const Message& msg);

    /**
     * @brief Construct a weight message based on the values from all scales.
     *
     * @param msg The Message containing calibration data.
     *
     * @return Message The message containing the weight message.
     */
    Message GetWeights(const Message& msg);

private:
    bool scaleExists(const int scale_idx);
    float getScaleValue(const int scale_idx);  // Corrected the return type from 'flat' to 'float'

private:
    TScale* scales_[MAX_NR_SCALES];
    TScaleFactory& factory_;
    int nrScales_;
};

} // namespace arra

#include "scales_handler.tpp"
