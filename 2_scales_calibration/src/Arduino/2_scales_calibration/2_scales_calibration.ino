//#pragma once
//#include <HX711.h>
//#include <Arduino.h>
#include "headers/scalesHandler.h"
#include "headers/commandHandler.h"
#include "headers/serial.h"

HardwareSerial& serial = Serial;
//Serial_& serial = Serial;

arra::Serial rw(serial);
arra::Scales sh;
arra::CommandHandler ch;

// void CalibrateCallback(const byte* buffer)
// {
//     sh.Calibrate(buffer);
// }

// void ConfigCallback(const byte* buffer)
// {
//     sh.Config(buffer);
// }

void processSerialData()
{
  if (rw.available())
  {
    ch.set_buffer(rw.get_buffer());     
    ch.get_command_from_buffer();
    ch.activate_command();
  }
}

void run() 
{
  displayScaleValues();
  processSerialData();
}

void displayScaleValues()
{
  rw.write("Scale_1: " + String(sh.GetValueFromIndex(0)) + "  Scale_2: " + String(sh.GetValueFromIndex(1)));
}

void setup()
{
  sh.AddScale(15, 14, 0);
  sh.AddScale(17, 16, 1);
  ch.add_callback(arra::CALIBRATE, [](const byte* buffer) { sh.Calibrate(buffer); });
  ch.add_callback(arra::CONFIG, [](const byte* buffer) { sh.Config(buffer); });

  // ch.add_callback(arra::CALIBRATE, CalibrateCallback);
  // ch.add_callback(arra::CONFIG, ConfigCallback);
  rw.start();
}

void loop()
{
  run();
}
