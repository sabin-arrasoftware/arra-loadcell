/**
 * @file scales_handler.h
 * @brief Defines a ScalesHandler class that manages multiple scales.
 */

#pragma once
#include "../protocol/proto.h"

namespace arra {

/**
 * @class ScalesHandler
 * @brief Manages multiple scales and provides methods to calibrate and get readings.
 * 
 * This class can handle multiple scales, calibrate them based on messages, and retrieve values.
 * 
 * @tparam TScale The type of scale. Must have `Calibrate` and `GetValue` methods.
 */
template <class TScale>
class ScalesHandler 
{

public:
    /**
     * @brief Construct a new ScalesHandler object.
     */
    ScalesHandler();

    /**
     * @brief Calibrate a scale based on a given message.
     * 
     * @param buf The buffer containing calibration data.
     */
    void Calibrate(const Buffer& buf);

    /**
     * @brief Add a scale to the handler.
     * 
     * @param scale The scale to be added.
     */
    void AddScale(const TScale& scale);

    /**
     * @brief Construct a weight message based on the values from all scales.
     * 
     * @return Buffer The buffer containing the weight message.
     */
    Buffer GetValues();

private:
    bool scaleExists(const int scale_idx);
    flat getScaleValue(const int scale_idx)

private:
    TScale *scales_[MAX_NR_SCALES];
    int nr_scales_;
};

} // namespace arra

#include "scales_handler."
