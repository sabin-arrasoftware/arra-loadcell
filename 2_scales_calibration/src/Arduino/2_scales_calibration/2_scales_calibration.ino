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
#include "headers/proto.h"
#include "headers/serial.h"

HardwareSerial& serial = Serial;
arra::Serial rw(serial);
arra::Scale scale_1(15, 14);
arra::Scale scale_2(17, 16);
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
  rw.start();
}

void loop()
{
  if (rw.available()) {
    byte* buffer = rw.get_buffer();

    arra::CommandHandler ch = arra::CommandHandler(buffer);

    ch.get_command_from_buffer();

    arra::Message decodedMessage = arra::decode_message(buffer);   
  }

}
