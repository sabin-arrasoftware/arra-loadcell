
#pragma once

namespace  arra {

template<class TAdapter>
class Scale {
public:
    Scale(TAdapter& first, TAdapter& second) : first_(first), second_(second){}

    void Calibrate(const float calibrationMass) {
        first_.Calibrate(calibrationMass);
        second_.Calibrate(calibrationMass);
        real_ = calibrationMass;
        drifting_ = calibrationMass;
    }

    float GetValue() const {
        float val = get_value();

        check_drifting(val);
        check_calibrate(val);

        return real_;
    }

private:

    float get_value() const {
        return (first_.GetValue() + second_.GetValue()) / 2.0;
    }
            
    bool checkPercentageChange(const float read, const float ref, const float threshold) {
        float change = ((read - ref) / ref) * 100.0;
        return abs(change) < threshold;
    }

    void check_drifting(const float read) {
        const bool isDrifting = checkPercentageChange(read, drifting_, driftingThreshold_);
        if (isDrifting) {
            drifting_ = read;
            return;
        }
        real_ = read;
    }

    void check_calibrate(const float read) {
        const bool needsCalibration = !checkPercentageChange(read, real_, calibrationThreshold_);
        if(needsCalibration) {
            Calibrate(real_);
        }
    }

private:
    TAdapter& first_;
    TAdapter& second_;
    
    float real_;
    float drifting_;

    float driftingThreshold_ = 0.5, calibrationThreshold_ = 0.3;
};

}