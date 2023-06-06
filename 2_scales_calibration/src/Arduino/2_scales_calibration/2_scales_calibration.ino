#include <Wire.h>
#include <HX711.h>

// Define the HX711 scale objects
HX711 scales[2];

// Pins:
const int HX711_dout[2] = {15, 17}; // MCU > HX711 dout pins for load cells
const int HX711_sck[2] = {14, 16}; // MCU > HX711 sck pins for load cells

float calibrationMasses[2] = {0.0, 0.0};
float calibrationFactors[2] = {1.0, 1.0};

void setup()
{
  Serial.begin(9600);

  // Initialize the HX711 scales
  for (int i = 0; i < 2; i++)
  {
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

    else for (int i = 0; i < 2; i++)
    {
      float scale_weight = scales[i].get_units();
      String msg = "Scale " + String(i + 1) + ": " + String(scale_weight) + "  ";
      Serial.print(msg);
    }
    Serial.println();
  
  delay(500);
}

float performCalibrationScale(float mass, int scaleNum)
{
  // Calibration logic for the given scale
  float calibrationFactor = mass / scales[scaleNum].get_units();
  // Apply the calibration factor to the scale
  scales[scaleNum].set_scale(calibrationFactor);
  // Tare the scale
  scales[scaleNum].tare();
  return calibrationFactor;
}

void parseCalibrate(const String& command)
{
  command.remove(0, strlen("CALIBRATE: "));
  int separatorIndex = command.indexOf(' ');
  if (separatorIndex != -1)
  {
    String mass1String = command.substring(0, separatorIndex);
    String mass2String = command.substring(separatorIndex + 1);
    // Update the calibration factors
    calibrationFactors[0] = performCalibrationScale(mass1String.toFloat(), 0);
    calibrationFactors[1] = performCalibrationScale(mass2String.toFloat(), 1);
    // Send the calibration factors back to the Python code
    String msg = "CAL_FACTOR: " + String(calibrationFactors[0]) + " " + String(calibrationFactors[1]);
    Serial.println(msg);
  }
  else
  {
    Serial.println("Error calibration");
  }
}


