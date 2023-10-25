// scales_handler.tpp
#include <new>

namespace arra {


// Ctor
template <class TScale, class TFactory>
ScalesHandler<TScale, TFactory>::ScalesHandler(TFactory& factory) 
: factory_(factory)
, nrScales_(0)
{
    for(int i = 0; i < MAX_NR_SCALES; ++i) 
    {
        scales_[i] = NULL;
    }
 
}

// Calibrate method implementation
template <class TScale, class TFactory>
Message ScalesHandler<TScale, TFactory>::Calibrate(const Message& msg) 
{
    CalibrateRequest request; 
    request.FromMessage(msg);

    if (!scaleExists(request.scaleIndex_)) 
    {
        return createErrorResponse(ERR_INVALID_SCALE_INDEX);
    }

    scales_[request.scaleIndex_]->Calibrate(request.calibrationMass_);

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
        delete scales_[request.scaleIndex_];
        scales_[request.scaleIndex_] = NULL;
    } 
    else
    {
        // just increment the number of scales
        nrScales_++;
    }

    scales_[request.scaleIndex_] = new TScale(factory_.CreateScale(request));

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

    return scales_[scaleIdx]->GetValue();
}

} // namespace arra
