#include "proto.h"

namespace arra {

void CalibrateRequest::FromMessage(const Message& msg) 
{
    scaleIndex_ = msg.payload_[0];
    int mass = msg.payload_[1] << 8 | msg.payload_[2];
    calibrationMass_ = mass / 100.0;
}

Message CalibrateRequest::ToMessage() 
{
    Message msg;
    msg.messageType_ = REQUEST;
    msg.operationType_ = CALIBRATE;
    msg.payloadSize_ = 3;  // Size of the payload for CalibrateRequest

    msg.payload_[0] = scaleIndex_;
    int mass = static_cast<int>(calibrationMass_ * 100);
    msg.payload_[1] = mass >> 8;
    msg.payload_[2] = mass & 0xFF;
    return msg;
}

void CalibrateResponse::FromMessage(const Message& msg) 
{
    success_ = (msg.payload_[0] != 0);  // Explicit conversion to bool
}

Message CalibrateResponse::ToMessage() 
{
    Message msg;
    msg.operationType_ = CALIBRATE;
    msg.messageType_ = RESPONSE;
    msg.payloadSize_ = 1;  // Size of the payload for CalibrateResponse

    msg.payload_[0] = success_;
    return msg;
}

void WeightRequest::FromMessage(const Message& msg)
{
    // Currently, there's no specific data to extract for a weight request.
}

Message WeightRequest::ToMessage()
{
    Message msg;
    msg.operationType_ = WEIGHT;
    msg.messageType_ = REQUEST;
    msg.payloadSize_ = 0;  // No payload for WeightRequest
    return msg;
}

void WeightResponse::FromMessage(const Message& msg)
{
    numberOfScales_ = msg.payload_[0];
    for (byte i = 0; i < numberOfScales_; ++i) 
    {
        int intWeight = msg.payload_[1 + 2 * i] << 8 | msg.payload_[2 + 2 * i];
        floatWeight_[i] = intWeight / 100.0;
    }
}

Message WeightResponse::ToMessage()
{
    Message msg;
    msg.operationType_ = WEIGHT;
    msg.messageType_ = RESPONSE;
    msg.payloadSize_ = 1 + 2 * numberOfScales_;  // Size of the payload for WeightResponse

    msg.payload_[0] = numberOfScales_;
    for (byte i = 0; i < numberOfScales_; ++i) 
    {
        int intWeight = static_cast<int>(floatWeight_[i] * 100);
        msg.payload_[1 + 2 * i] = intWeight >> 8;
        msg.payload_[2 + 2 * i] = intWeight & 0xFF;
    }
    return msg;
}

void AddScaleRequest::FromMessage(const Message& msg) 
{
    scaleIndex_ = msg.payload_[0];
    dt1_ = msg.payload_[1] ;
    sck1_ = msg.payload_[2] ;
    dt2_ = msg.payload_[3];
    sck2_ = msg.payload_[4];
}

Message AddScaleRequest::ToMessage() 
{
    Message msg;
    msg.messageType_ = REQUEST;
    msg.operationType_ = ADD_SCALE;
    msg.payloadSize_ = 5;  // Size of the payload for SetupRequest

    msg.payload_[0] = scaleIndex_;

    // Put the HX711 pins in the payload
    msg.payload_[1] = dt1_;
    msg.payload_[2] = sck1_;
    msg.payload_[3] = dt2_;
    msg.payload_[4] = sck2_;

    return msg;
}

void AddScaleResponse::FromMessage(const Message& msg) 
{
    success_ = (msg.payload_[0] != 0);  // Explicit conversion to bool
}

Message AddScaleResponse::ToMessage() 
{
    Message msg;
    msg.operationType_ = ADD_SCALE;
    msg.messageType_ = RESPONSE;
    msg.payloadSize_ = 1;  // Size of the payload for SetupResponse

    msg.payload_[0] = success_;
    return msg;
}

void ErrorResponse::FromMessage(const Message& msg) 
{
    errorCode_ = msg.payload_[0];  // Fixed the typo here
}

Message ErrorResponse::ToMessage() 
{
    Message msg;
    msg.operationType_ = ERROR;
    msg.messageType_ = RESPONSE;
    msg.payloadSize_ = 1;  // Size of the payload for ErrorResponse

    msg.payload_[0] = errorCode_;
    return msg;
}

// Utility functions

Message createErrorResponse(const ErrorCode ec)
{
    ErrorResponse response;
    response.errorCode_ = ec;
    return response.ToMessage();
}

} // namespace arra
