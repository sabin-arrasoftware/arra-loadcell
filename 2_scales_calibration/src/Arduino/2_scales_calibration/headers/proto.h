#pragma once

namespace arra
{
enum CommandType
{
  CALIBRATE = 0,
  CONFIG,
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

union Message
{
  CalibrateMessage calibrate;
  ConfigMessage config;
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

const CalibrateMessage& get_calibrate_message(const Message& message)
{
  return message.calibrate;
}

const ConfigMessage& get_config_message(const Message& message)
{
  return message.config;
}

}