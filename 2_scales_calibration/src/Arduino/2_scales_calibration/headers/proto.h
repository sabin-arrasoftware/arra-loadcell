#pragma once

namespace arra
{
enum CommandType
{
  CALIBRATE = 0,
  CONFIG,
  WEIGHT,
  LAST
};

struct CalibrateMessage
{
  byte scaleIndex;
};

struct ConfigMessage
{
  byte scaleIndex;
  float calibrationMass;
  int numReadings;
};

struct WeightMessage
{
  byte numberOfScales;
  float floatWeight[7];
};

union Message
{
  CalibrateMessage calibrate;
  ConfigMessage config;
  WeightMessage weight;
};

void encode_calibrate_command(const Message& message, byte* buffer)
{
  buffer[1] = message.calibrate.scaleIndex;
}

void encode_config_command(const Message& message, byte* buffer)
{
  buffer[1] = message.config.scaleIndex;
  int calibrationMassInt = static_cast<int>(message.config.calibrationMass * 100);
  buffer[2] = static_cast<byte>(calibrationMassInt >> 8);     
  buffer[3] = static_cast<byte>(calibrationMassInt);        
  buffer[4] = message.config.numReadings;
}

void encode_weight_command(const Message& message, byte* buffer)
{
  buffer[1] = message.weight.numberOfScales;
  for (byte i = 0; i < message.weight.numberOfScales; i++)
  {
    int intWeight = static_cast<int>(message.weight.floatWeight[i] * 100);
    buffer[2 * i + 2] = static_cast<byte>(intWeight >> 8);  
    buffer[2 * i + 3] = static_cast<byte>(intWeight);
  }
}

byte* encode_message(CommandType command, const Message& message, byte* buffer)
{
  switch (command)
  {
    case CALIBRATE:
      buffer[0] = command;
      encode_calibrate_command(message, buffer);
      break;

    case CONFIG:
      buffer[0] = command;
      encode_config_command(message, buffer);
      break;
    
    case WEIGHT:
      buffer[0] = command;
      encode_weight_command(message, buffer);

    default:
      // Unknown command, do not encode any message
      break;
  }

  return buffer;
}

// Reset a Message object to its default state
void reset_message(Message& message)
{
  message.config.scaleIndex = 255;
  message.config.calibrationMass = 0.0f;
  message.config.numReadings = 0;
  message.weight.numberOfScales = 0;
}

void decode_calibrate_command(const byte* buffer, Message& message)
{  
  message.calibrate.scaleIndex = static_cast<int>(buffer[1]);
}

void decode_config_command(const byte* buffer, Message& message)
{  
  message.config.scaleIndex = static_cast<byte>(buffer[1]);
  int calibrationMassValue = static_cast<int>(buffer[2]) << 8 | static_cast<int>(buffer[3]);
  message.config.calibrationMass = static_cast<float>(calibrationMassValue) / 100.0;
  message.config.numReadings = static_cast<int>(buffer[4]);
}

void decode_weight_command(const byte* buffer, Message& message)
{
  message.weight.numberOfScales = static_cast<byte>(buffer[1]);
  for (byte i = 0; i < message.weight.numberOfScales; i++)
  {
    int intWeight = static_cast<int>(buffer[2 * i + 2]) << 8 | static_cast<int>(buffer[2 * i + 3]);
    message.weight.floatWeight[i] = static_cast<float>(intWeight) / 100.0;
  }
}

Message decode_message(const byte* buffer)
{
  Message message;
  reset_message(message);

  CommandType command = static_cast<CommandType>(buffer[0]);

  bool success = false;

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

  if (!success)
      {
        // Invalid message
      }

  return message;
}

bool is_calibrate_message(const Message& message)
{  
  return (message.calibrate.scaleIndex != 255 && message.config.numReadings == 0);
}

bool is_config_message(const Message& message)
{
  return (message.config.scaleIndex != 255 && message.config.numReadings != 0);
}

bool is_weight_message(const Message& message)
{
  return (message.weight.numberOfScales != 0);
}

const CalibrateMessage& get_calibrate_message(const Message& message)
{
  return message.calibrate;
}

const ConfigMessage& get_config_message(const Message& message)
{
  return message.config;
}

const WeightMessage get_weight_message(const Message& message)
{
  return message.weight;
}

}