// scale.tpp

namespace arra {

// Free Functions.

bool isWithinThreshold(const float in, const float ref, const float threshold) 
{
    float change = ((in - ref) / ref) * 100.0;
    return abs(change) < threshold;
}

// Constructor
template<class TAdapter>
Scale<TAdapter>::Scale(TAdapter& first, TAdapter& second)
: first_(first)
, second_(second) 
{
}

// Public
template<class TAdapter>
void Scale<TAdapter>::Calibrate(const float refMass) 
{
    first_.Calibrate(refMass);
    second_.Calibrate(refMass);
    measured_ = refMass;
    driftReference_ = refMass;
}

template<class TAdapter>
float Scale<TAdapter>::GetValue() const 
{
    // get average measurement from both adapters:
    float average = (first_.GetValue() + second_.GetValue()) / 2.0;

    // if is drifting, we don't change value, just update driftReference
    const bool isDrifting = isWithinThreshold(average, driftReference_, driftThreshold_);
    if (isDrifting) 
    {
        driftReference_ = average;  // Fixed from 'val' to 'average'
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
    }

    return measured_;
}

} // namespace arra
