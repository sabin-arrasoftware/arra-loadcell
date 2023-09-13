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

struct Message
{
    byte ID;
    byte payload[BUFFER_SIZE - 1];
};

struct CalibrateMessage
{
    byte scaleIndex;
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

void encode_calibrate_command(const CalibrateMessage& calibrateMessage, Message& message)
{
    message.ID = CALIBRATE;
    message.payload[0] = calibrateMessage.scaleIndex;
}

void encode_config_command(const ConfigMessage& configMessage, Message& message)
{
    message.ID = CONFIG;
    message.payload[0] = configMessage.scaleIndex;
    int calibrationMassInt = static_cast<int>(configMessage.calibrationMass * 100);
    message.payload[1] = static_cast<byte>(calibrationMassInt >> 8);
    message.payload[2] = static_cast<byte>(calibrationMassInt);
    message.payload[3] = configMessage.numReadings;
}

void encode_weight_command(const WeightMessage& weightMessage, Message& message)
{
    message.ID = WEIGHT;
    message.payload[0] = weightMessage.numberOfScales;
    for (byte i = 0; i < weightMessage.numberOfScales; i++)
    {
        int intWeight = static_cast<int>(weightMessage.floatWeight[i] * 100);
        message.payload[2 * i + 1] = static_cast<byte>(intWeight >> 8);
        message.payload[2 * i + 2] = static_cast<byte>(intWeight);
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

void decode_calibrate_command(const Message& message, CalibrateMessage& calibrateMessage)
{
    calibrateMessage.scaleIndex = message.payload[0];
}

void decode_config_command(const Message& message, ConfigMessage& configMessage)
{
    configMessage.scaleIndex = message.payload[0];
    int calibrationMassIntValue = static_cast<int>(message.payload[1]) << 8 | static_cast<int>(message.payload[2]);
    configMessage.calibrationMass = static_cast<float>(calibrationMassIntValue) / 100.0;
    configMessage.numReadings = message.payload[3];
}

void decode_weight_command(const Message& message, WeightMessage& weightMessage)
{
    weightMessage.numberOfScales = message.payload[0];
    for (byte i = 0; i < weightMessage.numberOfScales; i++)
    {
        int intWeight = static_cast<int>(message.payload[2 * i + 1]) << 8 | static_cast<int>(message.payload[2 * i + 2]);
        weightMessage.floatWeight[i] = static_cast<float>(intWeight) / 100.0;
    }
}

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


bool is_calibrate_message(const Message& message)
{
    return (message.ID == CALIBRATE);
}

bool is_config_message(const Message& message)
{
    return (message.ID == CONFIG);
}

bool is_weight_message(const Message& message)
{
    return (message.ID == WEIGHT);
}

void message_to_buffer(const Message& message, byte buffer[BUFFER_SIZE])
{
    buffer[0] = message.ID;
    for (byte i = 1; i < BUFFER_SIZE; ++i)
    {
        buffer[i] = message.payload[i - 1];
    }
}

void buffer_to_message(const byte buffer[BUFFER_SIZE], Message& message)
{
    message.ID = buffer[0];
    for (byte i = 1; i < BUFFER_SIZE; ++i)
    {
        message.payload[i - 1] = buffer[i];
    }
}

CommandType getCommandType(const byte buffer[BUFFER_SIZE])
{
  return CommandType(buffer[0]);
}

}