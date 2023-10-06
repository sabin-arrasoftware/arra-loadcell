#pragma once

#include "headers/hx711_adapter.h"
#include "headers/scalesHandler.h"
#include "headers/commandHandler.h"
#include "headers/serial.h"
#include "headers/scale.h"


// Elemetns
HardwareSerial& serial = Serial;
arra::CommandHandler ch;
arra::Serial<arra::CommandHandler> rw(serial, ch);
arra::ScalesHandler<arra::Scale<arra::HX711Adapter>> sh;
arra::HX711Adapter firstAdapter(15, 14), secondAdapter(17, 16);
arra::Scale<arra::HX711Adapter> scale(firstAdapter, secondAdapter);


//Serial_& serial = Serial;

void run() 
{
  displayScaleValues();
  rw.processSerialData();  
}

void displayScaleValues()
{
  arra::Buffer buffer = sh.getWeightMessage();
  rw.write(buffer.toString());
}

void setup()
{
  sh.AddScale(scale);
  ch.add_callback(arra::CALIBRATE, [](const arra::Buffer& buffer) { sh.Calibrate(buffer); });
  rw.start();
}

void loop()
{
  run();
}
