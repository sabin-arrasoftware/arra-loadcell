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

#include <HX711.h>
#include "scale.h"
#include "serial.h"

HardwareSerial& serial = Serial;
arra::Serial rw(serial);
arra::Scale scale_1(15, 14);
arra::Scale scale_2(17, 16);

void setup()
{
  rw.add_callback("Calibrate_1", [&scale_1]() { scale_1.Calibrate(); });
  rw.add_callback("Calibrate_2", [&scale_2]() { scale_2.Calibrate(); });
  rw.start();
}

void loop()
{
  rw.read();

  rw.write("Scale_1: " + String(scale_1.GetValue()) + " Scale_2: " + String(scale_2.GetValue()));
}
