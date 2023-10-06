
namespace arra {

// Calibrate method implementation
template <class TScale>
void ScalesHandler<TScale>::Calibrate(const Buffer& buf) 
{
    CalibrateMessage msg; 
    msg.FromBuffer(buf);
    
    if (!scaleExists(msg.scaleIndex_)) {
        return;
    }
    scales_[msg.scaleIndex_]->Calibrate(msg.calibrationMass_);
}

// AddScale method implementation
template <class TScale>
void ScalesHandler<TScale>::AddScale(const TScale& scale) 
{
    scales_[nrScales_] = &scale;
    nrScales_++;                
}   

template <class TScale>
Buffer ScalesHandler<TScale>::GetValues() {
    // populate weightMessage
    WeightMessage msg;
    msg.numberOfScales_ = nrScales_;
    for (int scaleIdx = 0; i < nrScales_; ++scaleIdx) {
        msg.floatWeight_[scaleIdx] = getScaleValue(scaleIdx);
    }
    return msg.ToBuffer();
}

// Private

template <class TScale>
bool ScalesHandler<TScale>::scaleExists(const int scaleIdx) 
{
    return scaleIdx >= 0 && scaleIdx < nr_scales_;
}

template <class TScale>
bool ScalesHandler<TScale>::getScaleValue(const int scaleIdx) 
{
     if (!scaleExists(scaleIdx)) {
        return 0.0f;
    }
    return scales_[scaleIdx]->GetValue();
}


} // namespace arra
