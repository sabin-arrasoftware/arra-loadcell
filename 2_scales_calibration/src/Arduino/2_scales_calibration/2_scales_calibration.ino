// For the calibration of each scale,
// type in the Serial Monitor prompt a message with the following format:
// CALIBRATE: <scale number (0 or 1)> <known weight of object on scale measured in grams (e.g 6.1 for a 50 bani coin)>
// then press Ctrl + Enter and follow the instruction to place an object on the scale
// There should be a space between CALIBRATE: and the scale number
// and a space between the scale number and the weight
// e.g. to calibrate scale 0, type the message
// CALIBRATE: 0 6.1
// then place a 50 bani coin on scale 0
// To calibrate scale 1, type the message
// CALIBRATE: 1 6.1
// then place a 50 bani coin on scale 1

#include <HX711.h>

// Pins:
const int HX711_dout[2] = {15, 17}; // MCU > HX711 dout pins for load cells
const int HX711_sck[2] = {14, 16}; // MCU > HX711 sck pins for load cells

HX711 scales[2];

void setup()
{
  Serial.begin(9600);

  // Initialize the HX711 scales
  for (int i = 0; i < 2; i++)
  {
    // Initialize the HX711 sensor
    scales[i].begin(HX711_dout[i], HX711_sck[i]);
    scales[i].tare();
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command.startsWith("CALIBRATE:"))
    {
      parseCalibrate(command);
    }
  }
  else 
  { 
    for (int i = 0; i < 2; i++)
    {
      float scale_weight = scales[i].get_units();
      String msg = "Scale " + String(i + 1) + ": " + String(scale_weight) + "  ";
      Serial.print(msg);
    }
  Serial.println();
  }
  delay(500);
}

void parseCalibrate(const String& command)
{
  command.remove(0, strlen("CALIBRATE: "));
  int separatorIndex = command.indexOf(' ');
  if (separatorIndex != -1)
  {
    String scaleNumString = command.substring(0, separatorIndex);
    Serial.print("scaleNumString: ");
    Serial.print(scaleNumString);
    String massString = command.substring(separatorIndex + 1);
    Serial.print(" massString: ");
    Serial.println(massString);
    int scaleNum = scaleNumString.toInt();
    if (scaleNum == 0 || scaleNum == 1)
    {
      // Update the calibration factor
      updateCalibrationFactor(massString.toFloat(), scaleNum);
      // Send the calibration factors back to the Python code
      String msg = "CAL_FACTOR: " + String(scales[0].get_scale()) + " " + String(scales[1].get_scale());
      Serial.println(msg); 
    }
    else
    {
      Serial.println("Error calibration: scale number is not 0 or 1");
    }   
  }
  else
  {
    Serial.println("Error calibration");
  }
}

void updateCalibrationFactor(float knownMass, int scaleNum) {
  // Place the known mass on the scale
  String msg = "Place the known mass on scale " + String(scaleNum) + " ...";
  Serial.println(msg);
  delay(10000);  // Wait for 5 seconds to stabilize the reading

  // Read the average weight value
  float scale_weight = 0.0;
  int numReadings = 10;
  for (int i = 0; i < numReadings; i++) {
    scale_weight += scales[scaleNum].get_value();
    delay(100);
  }
  scale_weight /= numReadings;
  msg = "Scale " + String(scaleNum) + " raw values mean: " + String(scale_weight);
  Serial.println(msg);

  // Update the calibration factor if the weight is within a reasonable range
  if (scale_weight > -100000 && scale_weight < 100000) {
    float calibrationFactor = scale_weight / knownMass;
    scales[scaleNum].set_scale(calibrationFactor);

    // Print the updated calibration factor
    msg = "Calibration factor for scale " + String(scaleNum) + " updated: ";
    Serial.print(msg);
    Serial.println(calibrationFactor, 6);  // Display up to 6 decimal places
  } else {
    // Invalid weight value or out of range, indicate error
    Serial.println("Error updating calibration factor");
  }
}


