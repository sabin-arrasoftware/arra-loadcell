#pragma once

#include "hx711_adapter.h"
#include "scales_handler.h"
#include "commnad_handler.h"
#include "serial.h"
#include "scale.h"


HardwareSerial& serial = Serial;
arra::CommandHandler ch;
arra::Serial<arra::CommandHandler> rw(serial, ch);
arra::ScalesHandler<arra::Scale<arra::HX711Adapter>> sh;
arra::HX711Adapter firstAdapter(15, 14), secondAdapter(17, 16);
arra::Scale<arra::HX711Adapter> scale(firstAdapter, secondAdapter);

void run() 
{
  displayScaleValues();
  rw.ProcessSerialData();  
}

void displayScaleValues()
{
  arra::Buffer buffer = sh.GetValues();
  rw.Write(buffer.ToString());
}

void setup()
{
  sh.AddScale(scale);
  ch.AddCallback(arra::CALIBRATE, [](const arra::Buffer& buffer) { sh.Calibrate(buffer); });
  rw.Start();
}

void loop()
{
  run();
}