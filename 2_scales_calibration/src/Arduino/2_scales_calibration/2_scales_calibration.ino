//#pragma once
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
  rw.write("Scale_1: " + String(sh.GetValueFromIndex(0)) + "  Scale_2: " + String(sh.GetValueFromIndex(1)));
}

void setup()
{
  sh.AddScale(15, 14, 0);
  sh.AddScale(17, 16, 1);
  ch.add_callback(arra::CALIBRATE, [](const byte* buffer) { sh.Calibrate(buffer); });
  ch.add_callback(arra::CONFIG, [](const byte* buffer) { sh.Config(buffer); });
  // ch.add_callback(arra::WEIGHT, ...)
  rw.start();
}

void loop()
{
  run();
}
