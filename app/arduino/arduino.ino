// Include necessary header files
#include "hx711_adapter.h"
#include "scales_handler.h"
#include "command_handler.h"
#include "serial.h"
#include "scale.h"
#include "proto.h"
#include "threshold_provider.h"
#include "scale_factory.h"

// Rename some things for clarity
using DualScale = arra::Scale<arra::HX711Adapter, arra::ThresholdProvider>;
using Factory = arra::ScaleFactory<arra::HX711Adapter, arra::ThresholdProvider>;


// Define the serial object for communication
HardwareSerial& serial = Serial;

// Define the points for interpolation
arra::Point points[arra::NUMBER_OF_POINTS] = {
    {1, 0.2},
    {1000, 1},
    {10000, 5}
};


// Create instances of the necessary classes
arra::CommandHandler ch;
arra::Serial<arra::CommandHandler> rw(serial, ch);
arra::ThresholdProvider tp(points);
Factory factory(tp);
arra::ScalesHandler<DualScale, Factory> sh(factory);

void setup()
{
  pinMode(10, OUTPUT); // Initialize the digital pin as an output
  digitalWrite(10, LOW);

  // Register command callbacks for calibration and weight retrieval
  ch.AddCallback(arra::CALIBRATE, [](const arra::Message& msg)  -> arra::Message { return sh.Calibrate(msg); });
  ch.AddCallback(arra::WEIGHT, [](const arra::Message& msg) -> arra::Message { return sh.GetWeights(msg); });
  ch.AddCallback(arra::ADD_SCALE, [](const arra::Message& msg)  -> arra::Message { return sh.AddScale(msg); });

  // Start serial communication
  rw.Start();
}

void loop()
{
  // Continuously process incoming serial data
  rw.ProcessSerialData();
}
