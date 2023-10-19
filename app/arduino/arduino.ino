// Include necessary header files
#include "hx711_adapter.h"
#include "scales_handler.h"
#include "command_handler.h"
#include "serial.h"
#include "scale.h"
#include "proto.h"
#include "threshold_provider.h"

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
arra::ScalesHandler<arra::Scale<arra::HX711Adapter, arra::ThresholdProvider>> sh;
arra::ThresholdProvider tp(points);
arra::HX711Adapter firstAdapter(14, 15), secondAdapter(18, 19);
arra::Scale<arra::HX711Adapter, arra::ThresholdProvider> scale(firstAdapter, secondAdapter, tp);

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
