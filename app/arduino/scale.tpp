// scale.tpp

#include <math.h>

namespace arra {

// Free Functions.
static bool isWithinThreshold(const float in, const float ref, const float threshold) 
{
    float refNonNull = (ref < 0.01) ? 0.01 : ref;
    float change = ((in - ref) / refNonNull) * 100.0;
    return abs(change) < threshold;
}

// Constructor
template<class TAdapter, class TThresholdProvider>
Scale<TAdapter, TThresholdProvider>::Scale(const TAdapter& first, const TAdapter& second, TThresholdProvider& tp)
: first_(first)
, second_(second)
, tp_(tp) 
{
}

// Public
template<class TAdapter, class TThresholdProvider>
void Scale<TAdapter, TThresholdProvider>::Calibrate(const float refMass) 
{
    first_.Calibrate(refMass);
    second_.Calibrate(refMass);
    measured_ = refMass;
    driftReference_ = refMass;
}

template<class TAdapter, class TThresholdProvider>
float Scale<TAdapter, TThresholdProvider>::GetValue()
{
    // get average measurement from both adapters:
    float average = (first_.GetValue() + second_.GetValue()) / 2.0;

    // calculate the drift threshold based on drift reference;
    driftThreshold_ = tp_.get_threshold_percentage(driftReference_);

    // if is drifting, we don't change value, just update driftReference
    const bool isDrifting = isWithinThreshold(average, driftReference_, driftThreshold_);
    if (isDrifting) 
    {
        driftReference_ = average; 
        // drift might accumulate, if the drift is too big, we just recalibrate
        const bool needsCalibration = !isWithinThreshold(average, measured_, calibThreshold_);
        if(needsCalibration) 
        {
            Calibrate(measured_);
        }
    }
    else // value isn't considered as drifting
    {
        measured_ = average;
        driftReference_ = measured_;
    }

    return measured_;
}


} // namespace arra
