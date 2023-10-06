#include "proto.h"

namespace arra {

String Buffer::ToString() const 
{
    String result = "";
    for (int i = 0; i < size_; ++i) {
        result += String(int(payload[i])) + " ";
    }
    result += "\n";
    return result;
}

CalibrateMessage::CalibrateMessage() 
{
}

void CalibrateMessage::FromBuffer(const Buffer& buf) 
{
    scaleIndex_ = buf.payload[1];
    int mass = buf.payload[2] << 8 | buf.payload[3];
    calibrationMass_ = mass / 100.0;
}

Buffer CalibrateMessage::ToBuffer() 
{
    Buffer buf;
    buf.payload[0] = CALIBRATE;
    buf.payload[1] = scaleIndex_;
    buf.size_ = 2;
    return buf;
}

WeightMessage::WeightMessage()
{
}

void WeightMessage::FromBuffer(const Buffer& buf)
{
    numberOfScales_ = buf.payload[1];
    for (byte i = 0; i < numberOfScales_; ++i) {
        int intWeight = buf.payload[2 * i + 2] << 8 | buf.payload[2 * i + 3];
        floatWeight_[i] = intWeight / 100.0;
    }
}

Buffer WeightMessage::ToBuffer()
{
    Buffer buf;
    buf.payload[0] = WEIGHT;
    buf.payload[1] = numberOfScales_;
    for (byte i = 0; i < numberOfScales_; ++i) {
        int intWeight = static_cast<int>(floatWeight_[i] * 100);
        buf.payload[2 * i + 2] = intWeight >> 8;
        buf.payload[2 * i + 3] = intWeight;
    }
    buf.size_ = 2 + numberOfScales_ * 2;
    return buf;
}

bool IsCalibrateMsg(const Buffer& buffer) 
{
    return (buffer.payload[0] == CALIBRATE);
}

bool IsWeightMsg(const Buffer& buffer) {
    return (buffer.payload[0] == WEIGHT);
}

CommandType GetCommandType(const Buffer& buffer) {
    return CommandType(buffer.payload[0]);
}

} // namespace arra
