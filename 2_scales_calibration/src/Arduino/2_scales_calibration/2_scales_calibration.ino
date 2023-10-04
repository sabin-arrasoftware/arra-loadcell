#pragma once
//#include <HX711.h>
//#include <Arduino.h>
#include "headers/scalesHandler.h"
#include "headers/commandHandler.h"
#include "headers/serial.h"

HardwareSerial& serial = Serial;
arra::CommandHandler ch;
arra::Serial<arra::CommandHandler> rw(serial, ch);
arra::Scales sh;
//Serial_& serial = Serial;

void run() 
{
  displayScaleValues();
  rw.processSerialData();  
}

void displayScaleValues()
{
  arra::Buffer buffer = sh.getWeightMessage();
  rw.writeBuffer(buffer);
}

void setup()
{
  sh.AddDoubleScale(15, 14, 17, 16, 0);
  ch.add_callback(arra::CALIBRATE, [](const arra::Buffer& buffer) { sh.Calibrate(buffer); });
  ch.add_callback(arra::CONFIG, [](const arra::Buffer& buffer) { sh.Config(buffer); });
  ch.add_callback(arra::WEIGHT, [](const arra::Buffer& buffer) { sh.Weight(buffer); });
  rw.start();
}

void loop()
{
  run();
}





// byte* getByteBuffer()
// {
//   arra::WeightMessage weightMessage;
//   weightMessage.numberOfScales = 2;
//   for (byte i = 0; i < weightMessage.numberOfScales; ++i)
//   {
//     weightMessage.floatWeight[i] = sh.GetValueFromIndex(i);
//   }
//   //arra::Message message = static_cast<arra::Message>(weightMessage);
//   arra::Message message = weightMessage;
//   byte* buffer;
//   buffer = arra::encode_message(arra::WEIGHT, message, buffer);
//   //buffer = arra::encode_message(arra::WEIGHT, weightMessage, buffer);
//   return buffer;
// }
