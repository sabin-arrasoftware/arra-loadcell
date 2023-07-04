#pragma once

namespace arra
{
enum CommandType
{
  CALIBRATE = 0,
  CONFIG,
  LAST
};

//const byte MESSAGE_START_BYTE = 0xFF;

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

void encodeCalibrateCommand(const Message& message, byte* buffer)
{
  buffer[1] = message.calibrate.scaleIndex;
}

void encodeConfigCommand(const Message& message, byte* buffer)
{
  buffer[1] = message.config.scaleIndex;
  int calibrationMassInt = static_cast<int>(message.config.calibrationMass * 100);
  buffer[2] = static_cast<byte>(calibrationMassInt >> 8);     
  buffer[3] = static_cast<byte>(calibrationMassInt);        
  buffer[4] = message.config.numReadings;
}

byte* encodeMessage(CommandType command, const Message& message, byte* buffer)
{
  switch (command)
  {
    case CALIBRATE:
      buffer[0] = command;
      encodeCalibrateCommand(message, buffer);
      break;

    case CONFIG:
      buffer[0] = command;
      encodeConfigCommand(message, buffer);
      break;

    default:
      // Unknown command, do not encode any message
      break;
  }

  return buffer;
}

// Reset a Message object to its default state
void resetMessage(Message& message)
{
  message.calibrate.scaleIndex = -1;
  message.config.scaleIndex = -1;
  message.config.calibrationMass = 0.0f;
  message.config.numReadings = 0;
}

bool decodeCalibrateCommand(const byte* buffer, Message& message)
{  
  message.calibrate.scaleIndex = buffer[1];
  return true;
}

bool decodeConfigCommand(const byte* buffer, Message& message)
{  
  message.config.scaleIndex = buffer[1];
  int calibrationMassValue = static_cast<int>(buffer[2]) << 8 | static_cast<int>(buffer[3]);
  message.config.calibrationMass = static_cast<float>(calibrationMassValue) / 100.0;
  message.config.numReadings = buffer[4];
  return true;
}

Message decodeMessage(const byte* buffer)
{
  Message message;

  CommandType command = static_cast<CommandType>(buffer[0]);

  bool success = false;

  switch (command)
  {
    case CALIBRATE:
      success = decodeCalibrateCommand(buffer, message);
      
      break;

    case CONFIG:
      success = decodeConfigCommand(buffer, message);
      break;

    default:
      // Unknown command, do not decode any message
      break;
  }

  if (!success)
      {
        // Invalid message
        resetMessage(message);
      }

  return message;
}
}