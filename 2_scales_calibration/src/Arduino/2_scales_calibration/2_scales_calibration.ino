// For the calibration of each scale,
// type in the Serial Monitor prompt a message with the following format:
// Calibrate_<scale number (1 or 2)>
// then press Ctrl + Enter 
// e.g. to calibrate scale 1, type the message
// Calibrate_1
// then place a 50 bani coin on scale 1
// To calibrate scale 2, type the message
// Calibrate_2
// then place a 50 bani coin on scale 2

//#pragma once
//#include <HX711.h>
//#include <Arduino.h>
#include "headers/scale.h"
#include "headers/scalesHandler.h"
#include "headers/commandHandler.h"
#include "headers/serial.h"
#include "headers/proto.h"

HardwareSerial& serial = Serial;
arra::Serial rw(serial);
arra::Scales sh;
arra::CommandHandler ch;

// arra::Scale scale_1(15, 14);
// arra::Scale scale_2(17, 16);
// arra::Message message;
// arra::Scale scales[] = {scale_1, scale_2};

void CalibrateCallback(const byte* buffer)
{
    sh.Calibrate(buffer);
}

void ConfigCallback(const byte* buffer)
{
    sh.Config(buffer);
}

void processSerialData()
{
  if (rw.available())
  {
    ch.set_buffer(rw.get_buffer());
    ch.get_command_from_buffer();
    ch.activate_command();
  }
}

void displayScaleValues()
{
  rw.write("Scale_1: " + String(sh.GetValueFromIndex(0)) + "  Scale_2: " + String(sh.GetValueFromIndex(1)));
}

void setup()
{
  sh.AddScale(15, 14, 0);
  sh.AddScale(17, 16, 1);
  ch.add_callback(arra::CALIBRATE, CalibrateCallback);
  ch.add_callback(arra::CONFIG, ConfigCallback);
  rw.start();
}

void loop()
{
  displayScaleValues();
  
  processSerialData();
}
