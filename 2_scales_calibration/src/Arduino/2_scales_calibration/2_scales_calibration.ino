/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   -------------------------------------------------------------------------------------
*/

/*
   This example file shows how to calibrate the load cell and optionally store the calibration
   value in EEPROM, and also how to change the value manually.
   The result value can then later be included in your project sketch or fetched from EEPROM.

   To implement calibration in your project sketch the simplified procedure is as follow:
       LoadCell.tare();
       //place known mass
       LoadCell.refreshDataSet();
       float newCalibrationValue = LoadCell.getNewCalibration(known_mass);
*/
/*
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout1 = 15; //mcu > HX711 dout pin for first load cell
const int HX711_sck1 = 14; //mcu > HX711 sck pin for first load cell
const int HX711_dout2 = 17; //mcu > HX711 dout pin for second load cell
const int HX711_sck2 = 16; //mcu > HX711 sck pin for second load cell

//HX711 constructor:
HX711_ADC LoadCell1(HX711_dout1, HX711_sck1);
HX711_ADC LoadCell2(HX711_dout2, HX711_sck2);

const int calVal1_eepromAdress = 0;
const int calVal2_eepromAdress = 0;
unsigned long t = 0;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell1.begin();
  LoadCell2.begin();
  //LoadCell1.setReverseOutput(); //uncomment to turn a negative output value to positive for first load cell
  //LoadCell2.setReverseOutput(); //uncomment to turn a negative output value to positive for second load cell
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell1.start(stabilizingtime, _tare);
  LoadCell2.start(stabilizingtime, _tare);
  if (LoadCell1.getTareTimeoutFlag() || LoadCell1.getSignalTimeoutFlag() || LoadCell2.getTareTimeoutFlag() || LoadCell2.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell1.setCalFactor(1.0); // user set calibration value (float), initial value 1.0 may be used for this sketch
    LoadCell2.setCalFactor(1.0); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete");
  }
  while (!LoadCell1.update() && !LoadCell2.update());
  calibrate(); //start calibration procedure
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell1.update() || LoadCell2.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i1 = LoadCell1.getData();
      float i2 = LoadCell2.getData();
      Serial.print("Load_cell 1 output val: ");
      Serial.println(i1);
      Serial.print("Load_cell 2 output val: ");
      Serial.println(i2);
      newDataReady = 0;
      t = millis();
    }
  }

  // receive command from serial terminal
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    //if (inByte == 't') LoadCell.tareNoDelay(); //tare
    if (inByte == '1') LoadCell1.tareNoDelay(); //tare load cell 1
    else if (inByte == '2') LoadCell2.tareNoDelay(); //tare load cell 2
    else if (inByte == 'r') calibrate(); //calibrate
    else if (inByte == 'c') changeSavedCalFactor(LoadCell1, calVal1_eepromAdress); //edit calibration value manually for cell 1
    else if (inByte == 'd') changeSavedCalFactor(LoadCell2, calVal2_eepromAdress); //edit calibration value manually for cell 2
  }

  // check if last tare operation is complete for load cell 1
  if (LoadCell1.getTareStatus() == true) {
    Serial.println("Tare complete for load cell 1");
  }
  
  // check if last tare operation is complete for load cell 2
  if (LoadCell2.getTareStatus() == true) {
    Serial.println("Tare complete for load cell 2");
  }

}

void calibrate() {
  Serial.println("***");
  Serial.println("Start calibration:");
  Serial.println("Place load cell 1 and load cell 2 on level stable surfaces.");
  Serial.println("Remove any load applied to the load cell.");
  Serial.println("Send '1' from serial monitor to set the tare offset for load cell 1 or '2' for load cell 2.");

  boolean _resume1 = false;
  boolean _resume2 = false;

  while (_resume1 == false || _resume2 == false) {
    LoadCell1.update();
    LoadCell2.update();

    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == '1') LoadCell1.tareNoDelay();
      else if (inByte == '2') LoadCell2.tareNoDelay();
    }

    if (LoadCell1.getTareStatus() == true && !_resume1) {
      Serial.println("Tare complete for load cell 1");
      _resume1 = true;
    }

    if (LoadCell2.getTareStatus() == true && !_resume2) {
      Serial.println("Tare complete for load cell 2");
      _resume2 = true;
    }
  }

  Serial.println("Now, place your known mass on load cell 1.");
  Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");

  float known_mass1 = 0;
  _resume1 = false;
  while (_resume1 == false) {
    LoadCell1.update();
    if (Serial.available() > 0) {
      known_mass1 = Serial.parseFloat();
      if (known_mass1 != 0) {
        Serial.print("Known mass on load cell 1 is: ");
        Serial.println(known_mass1);
        _resume1 = true;
      }
    }
  }

  Serial.println("Now, place your known mass on load cell 2.");
  Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");

  float known_mass2 = 0;
  _resume2 = false;
  while (_resume2 == false) {
    LoadCell2.update();
    if (Serial.available() > 0) {
      known_mass2 = Serial.parseFloat();
      if (known_mass2 != 0) {
        Serial.print("Known mass on load cell 2 is: ");
        Serial.println(known_mass2);
        _resume2 = true;
      }
    }
  }
  
  LoadCell1.refreshDataSet();
  LoadCell2.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
  float newCalibrationValue1 = LoadCell1.getNewCalibration(known_mass1);
  float newCalibrationValue2 = LoadCell2.getNewCalibration(known_mass2); //get the new calibration value

  Serial.print("New calibration value for load cell 1 has been set to: ");
  Serial.print(newCalibrationValue1);
  Serial.println(", use this as calibration value (calFactor) in your project sketch.");
  Serial.print("Save this value to EEPROM adress ");
  Serial.print(calVal1_eepromAdress);
  Serial.println("? y/n");

  _resume1 = false;
  while (_resume1 == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal1_eepromAdress, newCalibrationValue1);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calVal1_eepromAdress, newCalibrationValue1);
        Serial.print("Value ");
        Serial.print(newCalibrationValue1);
        Serial.print(" saved to EEPROM address: ");
        Serial.println(calVal1_eepromAdress);
        _resume1 = true;

      }
      else if (inByte == 'n') {
        Serial.println("Value not saved to EEPROM");
        _resume1 = true;
      }
    }
  }

  Serial.print("New calibration value for load cell 2 has been set to: ");
  Serial.print(newCalibrationValue2);
  Serial.println(", use this as calibration value (calFactor) in your project sketch.");
  Serial.print("Save this value to EEPROM adress ");
  Serial.print(calVal2_eepromAdress);
  Serial.println("? y/n");

  _resume2 = false;
  while (_resume2 == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal2_eepromAdress, newCalibrationValue2);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calVal2_eepromAdress, newCalibrationValue2);
        Serial.print("Value ");
        Serial.print(newCalibrationValue2);
        Serial.print(" saved to EEPROM address: ");
        Serial.println(calVal2_eepromAdress);
        _resume2 = true;

      }
      else if (inByte == 'n') {
        Serial.println("Value not saved to EEPROM");
        _resume2 = true;
      }
    }
  }


  Serial.println("End calibration");
  Serial.println("***");
  Serial.println("To re-calibrate, send 'r' from serial monitor.");
  Serial.println("For manual edit of the calibration value for load cell 1, send 'c' from serial monitor.");
  Serial.println("For manual edit of the calibration value for load cell 2, send 'd' from serial monitor.");
  Serial.println("***");  
}

void changeSavedCalFactor(HX711_ADC& LoadCell, int calVal_eepromAdress) {
  float oldCalibrationValue = LoadCell.getCalFactor();
  boolean _resume = false;
  Serial.println("***");
  Serial.print("Current value is: ");
  Serial.println(oldCalibrationValue);
  Serial.println("Now, send the new value from serial monitor, i.e. 696.0");
  float newCalibrationValue;
  while (_resume == false) {
    if (Serial.available() > 0) {
      newCalibrationValue = Serial.parseFloat();
      if (newCalibrationValue != 0) {
        Serial.print("New calibration value is: ");
        Serial.println(newCalibrationValue);
        LoadCell.setCalFactor(newCalibrationValue);
        _resume = true;
      }
    }
  }
  _resume = false;
  Serial.print("Save this value to EEPROM adress ");
  Serial.print(calVal_eepromAdress);
  Serial.println("? y/n");
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calVal_eepromAdress, newCalibrationValue);
        Serial.print("Value ");
        Serial.print(newCalibrationValue);
        Serial.print(" saved to EEPROM address: ");
        Serial.println(calVal_eepromAdress);
        _resume = true;
      }
      else if (inByte == 'n') {
        Serial.println("Value not saved to EEPROM");
        _resume = true;
      }
    }
  }
  Serial.println("End change calibration value");
  Serial.println("***");
}
*/

#include <Wire.h>
#include <HX711.h>

// Define the HX711 scale objects
HX711 scale1;
HX711 scale2;

// Pins:
const int HX711_dout1 = 15; // MCU > HX711 dout pin for first load cell
const int HX711_sck1 = 14; // MCU > HX711 sck pin for first load cell
const int HX711_dout2 = 17; // MCU > HX711 dout pin for second load cell
const int HX711_sck2 = 16; // MCU > HX711 sck pin for second load cell

bool calibrationInProgress = false;
float calibrationMasses[2] = {0.0, 0.0};
float calibrationFactors[2] = {1.0, 1.0};

void setup()
{
  Serial.begin(9600);

  // Initialize the HX711 scales
  scale1.begin(HX711_dout1, HX711_sck1);
  scale2.begin(HX711_dout2, HX711_sck2);

  scale1.tare();
  scale2.tare();
}

void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("CALIBRATE:"))
    {
      calibrationInProgress = true;
      
      // Parse the calibration masses from the command
      command.remove(0, strlen("CALIBRATE:"));
      int separatorIndex = command.indexOf(' ');
      if (separatorIndex != -1)
      {
        String mass1String = command.substring(0, separatorIndex);
        String mass2String = command.substring(separatorIndex + 1);
        
        // Convert the masses to floating-point values
        float mass1 = mass1String.toFloat();
        float mass2 = mass2String.toFloat();
        
        // Perform calibration process for each scale
        float calibrationFactor1 = performCalibrationScale1(mass1);
        float calibrationFactor2 = performCalibrationScale2(mass2);

        // Update the calibration factors
        calibrationFactors[0] = calibrationFactor1;
        calibrationFactors[1] = calibrationFactor2;

        // Send the calibration factors back to the Python code
        Serial.print("CAL_FACTOR:");
        Serial.print(calibrationFactor1);
        Serial.print(" ");
        Serial.print(calibrationFactor2);
        Serial.println();
      }
      
      calibrationInProgress = false;
    }
    else if (command.equals("CALIBRATION_FACTORS"))
    {
      // Send the calibration factors back to the Python code
      Serial.print("CAL_FACTOR:");
      Serial.print(calibrationFactors[0]);
      Serial.print(" ");
      Serial.print(calibrationFactors[1]);
      Serial.println();
    }
  }

  if (!calibrationInProgress)
  {
    float scale1_weight = scale1.get_units();
    float scale2_weight = scale2.get_units();

    Serial.print("Scale 1: ");
    Serial.print(scale1_weight);
    Serial.print("  Scale 2: ");
    Serial.println(scale2_weight);

    delay(500);
  }
}

float performCalibrationScale1(float mass1)
{
  // Calibration logic for scale 1
  
  float calibrationFactor1 = mass1 / scale1.get_units();
  
  // Apply the calibration factor to the scale
  scale1.set_scale(calibrationFactor1);

  // Tare the scale
  scale1.tare();

  return calibrationFactor1;
}

float performCalibrationScale2(float mass2)
{
  // Calibration logic for scale 2
  
  float calibrationFactor2 = mass2 / scale2.get_units();
  
  // Apply the calibration factor to the scale
  scale2.set_scale(calibrationFactor2);

  // Tare the scale
  scale2.tare();

  return calibrationFactor2;
}


