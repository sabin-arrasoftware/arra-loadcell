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

// Pins:
const int HX711_doutPin[2] = {15, 17}; // MCU > HX711 dout pins for load cells
const int HX711_sckPin[2] = {14, 16}; // MCU > HX711 sck pins for load cells

HX711 scales[2];

int loopCounter = 0;
int displayPeriod = 5;

void setup()
{
  Serial.begin(9600);

  initializeHX711Scales();
}

void loop()
{
  readAndProcessSerialCommand();
  printScaleWeights();

  delay(500);

  // if (++loopCounter % displayPeriod == 0)
  // {
  //   printScaleWeights();
  // }
}

void initializeHX711Scales()
{
  for (int i = 0; i < 2; i++)
  {
    scales[i].begin(HX711_doutPin[i], HX711_sckPin[i]);
    scales[i].tare();
  }
}

void readAndProcessSerialCommand()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command.startsWith("CALIBRATE:"))
    {
      handleCalibrationCommand(command);
    }
  }
}

void handleCalibrationCommand(const String& command)
{
  String commandCopy = command;
  commandCopy.remove(0, strlen("CALIBRATE: "));
  int separatorIndex = commandCopy.indexOf(' ');
  if (separatorIndex != -1)
  {
    String scaleNumString = extractScaleNumber(commandCopy, separatorIndex);
    String massString = extractMassValue(commandCopy, separatorIndex);
    int scaleNum = parseScaleNumber(scaleNumString);
    if (isValidScaleNumber(scaleNum))
    {
      float knownMass = parseMassValue(massString);
      updateCalibrationFactor(knownMass, scaleNum);
      printCalibrationFactors();
    }
    else
    {
      printInvalidScaleNumberError();
    }
  }
  else
  {
    printCalibrationError();
  }
}

String extractScaleNumber(const String& command, int separatorIndex)
{
  return command.substring(0, separatorIndex);
}

String extractMassValue(const String& command, int separatorIndex)
{
  return command.substring(separatorIndex + 1);
}

int parseScaleNumber(const String& scaleNumString)
{
  return scaleNumString.toInt();
}

bool isValidScaleNumber(int scaleNum)
{
  return scaleNum == 1 || scaleNum == 2;
}

float parseMassValue(const String& massString)
{
  return massString.toFloat();
}

void printCalibrationFactors()
{
  String msg = "CAL_FACTOR: " + String(scales[0].get_scale()) + " " + String(scales[1].get_scale());
  Serial.println(msg);
}

void printInvalidScaleNumberError()
{
  Serial.println("Error calibration: scale number is not 1 or 2");
}

void printCalibrationError()
{
  Serial.println("Error calibration");
}

void updateCalibrationFactor(float knownMass, int scaleNum)
{
  int previous = scaleNum - 1;

  String msg = "Place the known mass on scale " + String(scaleNum) + " ...";
  Serial.println(msg);
  delay(10000);

  float scale_weight = readAverageScaleWeight(previous);
  printScaleRawValuesMean(scaleNum, scale_weight);

  if (isValidScaleWeight(scale_weight))
  {
    float calibrationFactor = calculateCalibrationFactor(scale_weight, knownMass);
    updateScaleCalibrationFactor(previous, calibrationFactor);
    printUpdatedCalibrationFactor(scaleNum, calibrationFactor);
  }
  else
  {
    printErrorUpdatingCalibrationFactor();
  }
}

float readAverageScaleWeight(int scaleIndex)
{
  float scale_weight = 0.0;
  int numReadings = 10;
  for (int i = 0; i < numReadings; i++)
  {
    scale_weight += scales[scaleIndex].get_value();
    delay(100);
  }
  scale_weight /= numReadings;
  return scale_weight;
}

void printScaleRawValuesMean(int scaleNum, float scale_weight)
{
  String msg = "Scale " + String(scaleNum) + " raw values mean: " + String(scale_weight);
  Serial.println(msg);
}

bool isValidScaleWeight(float scale_weight)
{
  return scale_weight > -100000 && scale_weight < 100000;
}

float calculateCalibrationFactor(float scale_weight, float knownMass)
{
  return scale_weight / knownMass;
}

void updateScaleCalibrationFactor(int scaleIndex, float calibrationFactor)
{
  scales[scaleIndex].set_scale(calibrationFactor);
}

void printUpdatedCalibrationFactor(int scaleNum, float calibrationFactor)
{
  String msg = "Calibration factor for scale " + String(scaleNum) + " updated: ";
  Serial.print(msg);
  Serial.println(calibrationFactor, 6);
}

void printErrorUpdatingCalibrationFactor()
{
  Serial.println("Error updating calibration factor");
}

void printScaleWeights()
{
  for (int i = 0; i < 2; i++)
  {
    float scale_weight = scales[i].get_units();
    String msg = "Scale " + String(i + 1) + ": " + String(scale_weight) + "  ";
    Serial.print(msg);
  }
  Serial.println();
}
