#pragma once

namespace arra
{

const byte BUFFER_SIZE = 16;  
const byte MAX_NR_SCALES = 4;

enum CommandType
{
    CALIBRATE = 0,
    CONFIG,
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

struct CalibrateMessage
{
    byte scaleIndex;
    float calibrationMass;
};

struct ConfigMessage
{
    byte scaleIndex;
    float calibrationMass;
    byte numReadings;
};


struct WeightMessage
{   
    byte numberOfScales;
    float floatWeight[MAX_NR_SCALES];
};

void encode_calibrate_command(const CalibrateMessage& calibrateMessage, Buffer& buffer)
{
    buffer.payload[0] = CALIBRATE;
    buffer.payload[1] = calibrateMessage.scaleIndex;
}

void encode_config_command(const ConfigMessage& configMessage, Buffer& buffer)
{
    buffer.payload[0] = CONFIG;
    buffer.payload[1] = configMessage.scaleIndex;
    int calibrationMassInt = static_cast<int>(configMessage.calibrationMass * 100);
    buffer.payload[2] = static_cast<byte>(calibrationMassInt >> 8);
    buffer.payload[3] = static_cast<byte>(calibrationMassInt);
    buffer.payload[4] = configMessage.numReadings;
}

void encode_weight_command(const WeightMessage& weightMessage, Buffer& buffer)
{
    buffer.payload[0] = WEIGHT;
    buffer.payload[1] = weightMessage.numberOfScales;
    for (byte i = 0; i < weightMessage.numberOfScales; ++i)
    {
        int intWeight = static_cast<int>(weightMessage.floatWeight[i] * 100);
        buffer.payload[2 * i + 2] = static_cast<byte>(intWeight >> 8);
        buffer.payload[2 * i + 3] = static_cast<byte>(intWeight);
    }
}

void decode_calibrate_command(const Buffer& buffer, CalibrateMessage& calibrateMessage)
{
    calibrateMessage.scaleIndex = buffer.payload[1];
}

void decode_config_command(const Buffer& buffer, ConfigMessage& configMessage)
{
    configMessage.scaleIndex = buffer.payload[1];
    int calibrationMassIntValue = static_cast<int>(buffer.payload[2]) << 8 | static_cast<int>(buffer.payload[3]);
    configMessage.calibrationMass = static_cast<float>(calibrationMassIntValue) / 100.0;
    configMessage.numReadings = buffer.payload[4];
}

void decode_weight_command(const Buffer& buffer, WeightMessage& weightMessage)
{
    weightMessage.numberOfScales = buffer.payload[1];
    for (byte i = 0; i < weightMessage.numberOfScales; ++i)
    {
        int intWeight = static_cast<int>(buffer.payload[2 * i + 2]) << 8 | static_cast<int>(buffer.payload[2 * i + 3]);
        weightMessage.floatWeight[i] = static_cast<float>(intWeight) / 100.0;
    }
}

bool is_calibrate_message(const Buffer& buffer)
{
    return (buffer.payload[0] == CALIBRATE);
}

bool is_config_message(const Buffer& buffer)
{
    return (buffer.payload[0] == CONFIG);
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



// void encode_message(const Message& message, byte buffer[BUFFER_SIZE])
// {
//   switch (message.command)
//   {
//     case CALIBRATE:
//       encode_calibrate_command(message, buffer);
//       break;

//     case CONFIG:
//       encode_config_command(message, buffer);
//       break;
    
//     case WEIGHT:
//       encode_weight_command(message, buffer);

//     default:
//       // Unknown command, do not encode any message
//       break;
//   }
// }

// void decode_message(const byte buffer[BUFFER_SIZE], Message& message)
// {
//   CommandType command = static_cast<CommandType>(buffer[0]);

//   switch (command)
//   {
//     case CALIBRATE:
//       decode_calibrate_command(buffer, message);      
//       break;

//     case CONFIG:
//       decode_config_command(buffer, message);
//       break;

//     case WEIGHT:
//       decode_weight_command(buffer, message);

//     default:
//       // Unknown command, do not decode any message
//       break;
//   }
// }
