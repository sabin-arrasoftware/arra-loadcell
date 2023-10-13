// Include necessary header files
#include "hx711_adapter.h"
#include "scales_handler.h"
#include "command_handler.h"
#include "serial.h"
#include "scale.h"
#include "proto.h"

// Define the serial object for communication
HardwareSerial& serial = Serial;


// Create instances of the necessary classes
arra::CommandHandler ch;
arra::Serial<arra::CommandHandler> rw(serial, ch);
arra::ScalesHandler<arra::Scale<arra::HX711Adapter>> sh;
arra::HX711Adapter firstAdapter(14, 15), secondAdapter(18, 19);
arra::Scale<arra::HX711Adapter> scale(firstAdapter, secondAdapter);

void setup()
{
  // Add the scale to the scales handler
  sh.AddScale(scale);

  // Register command callbacks for calibration and weight retrieval
  ch.AddCallback(arra::CALIBRATE, [](const arra::Message& msg)  -> arra::Message { return sh.Calibrate(msg); });
  ch.AddCallback(arra::WEIGHT, [](const arra::Message& msg) -> arra::Message { return sh.GetWeights(msg); });

  // Start serial communication
  rw.Start();
}

void loop()
{
  // Continuously process incoming serial data
  rw.ProcessSerialData();
}
