// scales_handler.tpp

namespace arra {


// Ctor
template <class TScale, class TThresholdProvider>
ScalesHandler(TThresholdProvider& tp) 
: tp_(tp)
{
}

// Calibrate method implementation
template <class TScale, class TThresholdProvider>
Message ScalesHandler<TScale, TThresholdProvider>::Calibrate(const Message& msg) 
{
    CalibrateRequest request; 
    request.FromMessage(msg);

    if (!scaleExists(request.scaleIndex_)) {
        return createErrorResponse(ERR_INVALID_SCALE_INDEX);
    }
    TScale& scaleRef = scales_[scaleIdx].scale;
    scaleRef.Calibrate(request.calibrationMass_);

    CalibrateResponse response;
    response.success_ = true;
    return response.ToMessage();
}

// AddScale method implementation
template <class TScale, class TScaleFactory>
Message ScalesHandler<TScale, TScaleFactory>::AddScale(const Message& msg) 
{
    AddScaleRequest request; 
    request.FromMessage(msg);

    // This part is wrong and must be rethinked.
    // At current stage we are working with only one scale, and we get the index value from outside world
    // this should be done internally. Please revisit this decision in the future.
    if (scaleExists(request.scaleIndex_)) 
    {
        scales_[request.scaleIndex_].scale.~TScale();  // Explicitly call the destructor
    } 
    else
    {
        // just increment the number of scales
        nrScales_++;
    }

    // R.Amarandei: this looks crazy and unnecessary difficult, but given the restrictions we have... we can say only shit happens.
    new (&scales_[request.scaleIndex_].scale) TScale(factory_.CreateScale(request));

    AddScaleResponse response;
    response.success_ = true;
    return response.ToMessage();     
}   

template <class TScale, class TScaleFactory>
Message ScalesHandler<TScale, TScaleFactory>::GetWeights(const Message& ) {
    // populate weightMessage
    WeightResponse response;
    response.numberOfScales_ = nrScales_;
    for (int scaleIdx = 0; scaleIdx < nrScales_; ++scaleIdx) {
        response.floatWeight_[scaleIdx] = getScaleValue(scaleIdx);
    }
    return response.ToMessage();
}

// Private

template <class TScale, class TScaleFactory>
bool ScalesHandler<TScale, TScaleFactory>::scaleExists(const int scaleIdx) 
{
    return scaleIdx >= 0 && scaleIdx < nrScales_;
}

template <class TScale, class TScaleFactory>
float ScalesHandler<TScale, TScaleFactory>::getScaleValue(const int scaleIdx) 
{
     if (!scaleExists(scaleIdx)) {
        return 0.0f;
    }
    TScale& scaleRef = scales_[scaleIdx].scale;
    return scaleRef.GetValue();
}

} // namespace arra
