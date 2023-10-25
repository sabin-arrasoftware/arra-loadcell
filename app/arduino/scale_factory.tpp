
namespace arra {

// Constructor for the ScaleFactory
template<class TAdapter, class TThresholdProvider>
ScaleFactory<TAdapter, TThresholdProvider>::ScaleFactory(TThresholdProvider& tp)
: tp_(tp)
{
}

template<class TAdapter, class TThresholdProvider>
DualScale ScaleFactory<TAdapter, TThresholdProvider>::CreateScale(const AddScaleRequest& request)
{
    // Construct your adapters based on the request data
    TAdapter firstAdapter(request.dt1_, request.sck1_);
    TAdapter secondAdapter(request.dt2_, request.sck2_);
    
    // Construct and return the Scale object
    return DualScale(firstAdapter, secondAdapter, tp_);
}

} // namespace arra
