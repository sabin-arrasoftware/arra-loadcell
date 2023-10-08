// scales_handler.tpp

namespace arra {

// Calibrate method implementation
template <class TScale>
Message ScalesHandler<TScale>::Calibrate(const Message& msg) 
{
    CalibrateRequest request; 
    request.FromMessage(msg);
    
    if (!scaleExists(request.scaleIndex_)) {
        return createErrorResponse(ERR_INVALID_SCALE_INDEX);
    }
    scales_[request.scaleIndex_]->Calibrate(request.calibrationMass_);

    CalibrateResponse response;
    response.success_ = true;
    return response.ToMessage();
}

// AddScale method implementation
template <class TScale>
void ScalesHandler<TScale>::AddScale(const TScale& scale) 
{
    scales_[nrScales_] = &scale; 
    nrScales_++;                
}   

template <class TScale>
Message ScalesHandler<TScale>::GetWeights(const Message& msg) {
    // populate weightMessage
    WeightResponse response;
    msg.numberOfScales_ = nrScales_;
    for (int scaleIdx = 0; scaleIdx < nrScales_; ++scaleIdx) {
        msg.floatWeight_[scaleIdx] = getScaleValue(scaleIdx);
    }
    return msg.ToBuffer();
}

// Private

template <class TScale>
bool ScalesHandler<TScale>::scaleExists(const int scaleIdx) 
{
    return scaleIdx >= 0 && scaleIdx < nrScales_;
}

template <class TScale>
float ScalesHandler<TScale>::getScaleValue(const int scaleIdx) 
{
     if (!scaleExists(scaleIdx)) {
        return 0.0f;
    }
    return scales_[scaleIdx]->GetValue();
}

} // namespace arra
