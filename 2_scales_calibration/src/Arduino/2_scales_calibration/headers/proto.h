#pragma once

namespace arra
{

const byte BUFFER_SIZE = 16;  
const byte MAX_NR_SCALES = 4;

enum CommandType
{
    CALIBRATE = 0,
    WEIGHT,
    LAST
};

struct Buffer
{
    byte payload[BUFFER_SIZE];

    String toString() const {
        String result = "";
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            result += String(int(payload[i])) + " ";
        }
        return result;
    }
};

struct CalibrateMessage {
    CalibrateMessage() {}
    
    void fromBuffer(const Buffer& buf) {
        // scaleIndex
        scaleIndex_ = buf.payload[1];
        // calibrationMass
        int mass = buf.payload[2] << 8 | buf.payload[3];
        calibrationMass_ = mass / 100.0;
    }

    Buffer toBuffer() {
        Buffer buf;
        buf.payload[0] = CALIBRATE;
        buf.payload[1] = scaleIndex_;
        return buf;
    }

    byte scaleIndex_;
    float calibrationMass_;
};

struct WeightMessage{   
    WeightMessage() {}
    
    
    void fromBuffer(const Buffer& buf) {
        numberOfScales_ = buf.payload[1];
        for (byte i = 0; i < numberOfScales_; ++i)
        {
            int intWeight = buf.payload[2 * i + 2] << 8 | buf.payload[2 * i + 3];
            floatWeight_[i] = static_cast<float>(intWeight) / 100.0;
        }
    }

    Buffer toBuffer() {
        Buffer buf;
        buf.payload[0] = WEIGHT;
        buf.payload[1] = numberOfScales_;
        for (byte i = 0; i < numberOfScales_; ++i)
        {
            int intWeight = static_cast<int>(floatWeight_[i] * 100);
            buf.payload[2 * i + 2] = intWeight >> 8;
            buf.payload[2 * i + 3] = intWeight;
        }
        return buf;
    }

    byte numberOfScales_;
    float floatWeight_[MAX_NR_SCALES];
};


bool is_calibrate_message(const Buffer& buffer)
{
    return (buffer.payload[0] == CALIBRATE);
}

bool is_weight_message(const Buffer& buffer)
{
    return (buffer.payload[0] == WEIGHT);
}

CommandType getCommandType(const Buffer& buffer)
{
  return CommandType(buffer.payload[0]);
}

}
