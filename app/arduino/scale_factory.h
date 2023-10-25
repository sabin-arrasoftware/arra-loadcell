#pragma once

#include "scale.h"

namespace arra {

template<class TAdapter, class TThresholdProvider>
class ScaleFactory 
{

public:
    /**
     * @brief Constructor that initializes the ScaleFactory with a threshold provider.
     * 
     * @param tp The threshold provider object.
     */
    ScaleFactory(TThresholdProvider& tp);

    /**
     * @brief Constructs a Scale object based on a given setup request.
     * 
     * @param request The AddScaleRequest providing the necessary details to set up the scale.
     * @return Scale<TAdapter, TThresholdProvider> The created Scale object.
     */
    Scale<TAdapter, TThresholdProvider> CreateScale(const AddScaleRequest& request);
    
    // ... Any other factory-related methods ...

private:
    // Reference to the threshold provider
    TThresholdProvider& tp_;
    
    // ... Any other private members or helper methods ...
};

} // namespace arra

#include "scale_factory.tpp"
