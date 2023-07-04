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

#pragma once
#include <HX711.h>
#include "../headers/scale.h"
#include "../headers/scales.h"
#include "../headers/commandHandler.h"
#include "../headers/proto.h"

HardwareSerial& serial = Serial;
arra::Serial rw(serial);
arra::Scale scale_1(15, 14);
arra::Scale scale_2(17, 16);
//arra::CommandHandler ch;
arra::Message message;
arra::Scale scales[] = {scale_1, scale_2};



void configureScale(arra::Scale& scale)
{
  arra::scale_config newConfig;
  // Set the new configuration values for scale based on your requirements
  // Read the user input for calibrationMass
  float calibrationMass;
  serial.println("Enter calibrationMass:");
  while (!serial.available()) {
    // Wait for user input
  }
  calibrationMass = serial.parseFloat();

  // Read the user input for numReadings
  int numReadings;
  serial.println("Enter numReadings:");
  while (!serial.available()) {
    // Wait for user input
  }
  numReadings = serial.parseInt();
  newConfig.calibrationMass = calibrationMass;
  newConfig.numReadings = numReadings;
  scale.Config(newConfig);
}



void setup()
{
  // rw.add_callback("1", [&scale_1]() { scale_1.Calibrate(); });
  // rw.add_callback("2", [&scale_2]() { scale_2.Calibrate(); }); 
  // rw.add_callback("3", [&scale_1]() { configureScale(scale_1); });
  // rw.add_callback("4", [&scale_2]() { configureScale(scale_2); });
  rw.start();

}

void loop()
{
  message = rw.handle_serial_data();
  //ch.handleCommand(message);
  rw.write("Scale_1: " + String(scale_1.GetValue()) + "  Scale_2: " + String(scale_2.GetValue()));
}
