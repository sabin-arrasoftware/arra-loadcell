#pragma once
#include "proto.h"

namespace  arra {

template <class TScale>
class ScalesHandler {

public:
    ScalesHandler() : nr_scales_(0) {};

    void Calibrate(const Buffer& buf) {
        CalibrateMessage msg; 
        msg.fromBuffer(buf);
        
        if (isNotValidIndex(msg.scaleIndex_)) {
            return;
        }
        scales_[msg.scaleIndex_]->Calibrate(msg.calibrationMass_);
    }

    void AddScale(const TScale& scale) {
        scales_[nr_scales_] = &scale;
        nr_scales_++;                
    }   

    float GetValueFromIndex(const int index) {
        if (isNotValidIndex(index)) {
            return 0.0f;
        }
        return scales_[index]->GetValue();
    }   

    Buffer getWeightMessage() {
        WeightMessage weightMessage;
        weightMessage.numberOfScales_ = nr_scales_;
        for (int i = 0; i < nr_scales_; ++i) {
            weightMessage.floatWeight_[i] = GetValueFromIndex(i);
        }
        return weightMessage.toBuffer();
    }

private:
    bool isNotValidIndex(const int index) {
        return index < 0 || index >= nr_scales_;
    }   

private:
    TScale *scales_[MAX_NR_SCALES];
    int nr_scales_;
};

}