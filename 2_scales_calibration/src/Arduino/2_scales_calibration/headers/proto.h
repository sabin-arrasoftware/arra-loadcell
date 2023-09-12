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
    byte command;                   // Command type
    byte scaleIndex;
    float calibrationMass;
    byte numReadings;
    byte numberOfScales;
    float floatWeight[MAX_NR_SCALES];
};

void encode_calibrate_command(const Message& message, byte buffer[BUFFER_SIZE])
{
    buffer[0] = message.command;
    buffer[1] = message.scaleIndex;
}

void encode_config_command(const Message& message, byte buffer[BUFFER_SIZE])
{
    buffer[0] = message.command;
    buffer[1] = message.scaleIndex;
    int calibrationMassInt = static_cast<int>(message.calibrationMass * 100);
    buffer[2] = static_cast<byte>(calibrationMassInt >> 8);
    buffer[3] = static_cast<byte>(calibrationMassInt);
    buffer[4] = message.numReadings;
}

void encode_weight_command(const Message& message, byte buffer[BUFFER_SIZE])
{
    buffer[0] = message.command;
    buffer[1] = message.numberOfScales;
    for (byte i = 0; i < message.numberOfScales; i++)
    {
        int intWeight = static_cast<int>(message.floatWeight[i] * 100);
        buffer[2 * i + 2] = static_cast<byte>(intWeight >> 8);
        buffer[2 * i + 3] = static_cast<byte>(intWeight);
    }
}

void encode_message(const Message& message, byte buffer[BUFFER_SIZE])
{
  switch (message.command)
  {
    case CALIBRATE:
      encode_calibrate_command(message, buffer);
      break;

    case CONFIG:
      encode_config_command(message, buffer);
      break;
    
    case WEIGHT:
      encode_weight_command(message, buffer);

    default:
      // Unknown command, do not encode any message
      break;
  }
}

void reset_message(Message& message)
{
    message.command = 0;
}

void decode_calibrate_command(const byte buffer[BUFFER_SIZE], Message& message)
{
    message.command = buffer[0];
    message.scaleIndex = buffer[1];
}

void decode_config_command(const byte buffer[BUFFER_SIZE], Message& message)
{
    message.command = buffer[0];
    message.numberOfScales = buffer[1];
    int calibrationMassValue = static_cast<int>(buffer[2]) << 8 | static_cast<int>(buffer[3]);
    message.calibrationMass = static_cast<float>(calibrationMassValue) / 100.0;
    message.numReadings = buffer[4];
}

void decode_weight_command(const byte buffer[BUFFER_SIZE], Message& message)
{
    message.command = buffer[0];
    message.numberOfScales = buffer[1];
    for (byte i = 0; i < message.numberOfScales; i++)
    {
        int intWeight = static_cast<int>(buffer[2 * i + 2]) << 8 | static_cast<int>(buffer[2 * i + 3]);
        message.floatWeight[i] = static_cast<float>(intWeight) / 100.0;
    }
}

void decode_message(const byte buffer[BUFFER_SIZE], Message& message)
{
  CommandType command = static_cast<CommandType>(buffer[0]);

  switch (command)
  {
    case CALIBRATE:
      decode_calibrate_command(buffer, message);      
      break;

    case CONFIG:
      decode_config_command(buffer, message);
      break;

    case WEIGHT:
      decode_weight_command(buffer, message);

    default:
      // Unknown command, do not decode any message
      break;
  }
}


bool is_calibrate_message(const Message& message)
{
    return (message.command == CALIBRATE);
}

bool is_config_message(const Message& message)
{
    return (message.command == CONFIG);
}

bool is_weight_message(const Message& message)
{
    return (message.command == WEIGHT);
}

CommandType getCommandType(const byte buffer[BUFFER_SIZE])
{
  return CommandType(buffer[0]);
}

}