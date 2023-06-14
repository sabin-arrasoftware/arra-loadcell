// For the calibration of each scale,
// type in the Serial Monitor prompt a message with the following format:
// CALIBRATE: <scale number (1 or 2)> <known weight of object on scale measured in grams (e.g 6.1 for a 50 bani coin)>
// then press Ctrl + Enter and follow the instruction to place an object on the scale
// There should be a space between CALIBRATE: and the scale number
// and a space between the scale number and the weight
// e.g. to calibrate scale 1, type the message
// CALIBRATE: 1 6.1
// then place a 50 bani coin on scale 1
// To calibrate scale 2, type the message
// CALIBRATE: 2 6.1
// then place a 50 bani coin on scale 2

#include <HX711.h>
#include "scale.h"
#include "serial_reader.h"


arra::Serial rw;
arra::Scale scale_1(15,14);
arra::Scale scale_2(17,16);

void setup()
{
  rw.add_callback("Calibrate_1", [&scale_1]() { scale_1.Calibrate(); });
  rw.add_callback("Calibrate_2", [&scale_2]() { scale_2.Calibrate(); });
  rw.start();
}

void loop()
{
  rw.read();

  rw.write("Scale_1: " + String(scale_1.GetValue()));
  rw.write("Scale_2: " + String(scale_2.GetValue()));

  delay(500);
}
