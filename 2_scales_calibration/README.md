# Load Cell GUI

## Description
This Python code provides a graphical user interface (GUI) for displaying the output values from load cells. It uses the Tkinter library to create the GUI and will include functionality to periodically update the load cell values and display them in the GUI.

## Prerequisites
- Python 3.x
- Tkinter library (included in Python standard library)
- Arduino
- 2 load cells
- 2 HX711 modules
- breadboard
- USB cable for Arduino
- connecting wires

## Arduino, load cells, breadboard and wires
- Put the two HX711 modules in the breadboard so that they do not share any row.
- Connect the wires from each loadcell to the breadboard in the following way:
    - the red wire must link on the same row of the breadboard to the E+ section of the HX711 module;
    - the black wire to E-;
    - the white wire to A-;
    - the green wire to E+;
Connect the first load cell to the first HX711 module and the second load cell to the second HX711 module in this way.
- Connect with wires from the breadboard to the Arduino in the following way:
    - for the first HX711 module:
        - the GND section of HX711 to a GND pin on Arduino;
        - the DT section of HX711 to the A1 pin on Arduino;
        - the SCK section of HX711 to the A0 pin on Arduino;
        - the VCC section of HX711 to a 5V pin on Arduino;
    - for the second HX711 module:
        - the GND section of HX711 to a GND pin on Arduino;
        - the DT section of HX711 to the A3 pin on Arduino;
        - the SCK section of HX711 to the A2 pin on Arduino;
        - the VCC section of HX711 to a 5V pin on Arduino;

## Python
- Ensure that Python 3.x is installed on your system.
- Install the Tkinter library if it is not already available. You can check if it is installed by running `python -m tkinter` in the terminal. If it is not installed, you can install it using your package manager or by following the instructions specific to your operating system.
- Download or clone the repository to your local machine.

## Usage
- Connect the Arduino to your laptop by plugging in the cable which connects them.
- If the downloaded content of the repository is on a virtual machine, after plugging the cable, you might see a small window which says "New USB Cable Detected" where you must select the respective virtual machine.
- Navigate to the file containing the Arduino code: 2_scales_calibration/src/Arduino/2_scales_calibration/2_scales_calibration.ino
- In the top right of the window, click the button which says "Arduino: Verify" when hovering over it with your cursor. 
- In the lower part of the window, select the OUTPUT window. There the last line should be: "[Done] Verifying sketch 'src/Arduino/2_scales_calibration/2_scales_calibration.ino'". This means that the Arduno sketch is ready to be uploaded to the Arduino.
- In the top right of the window, click the button which says "Arduino: Upload". 
- In the OUTPUT window, the last line should be "[Done] Uploading sketch 'src/Arduino/2_scales_calibration/2_scales_calibration.ino'". The Arduino code is now uploaded to your Arduino.
- In the lower part of the window, select the SERIAL MONITOR window. There, at Monitor Mode select Serial, at Port select the port for the USB connecting the Arduino (e.g. /dev/ttyUSB0), at Baud Rate select 9600.
- Navigate to the directory containing the Python code: arra-loadcell/2_scales_calibration/src/Python/
- Run the following command to start the Load Cell GUI: python3 main.py
- The GUI window should appear.
- If there is any error, take the USB cable out and plug it in again, connecting it to your virtual machine if that is the case.
- Click the "Start Measurements" button to display the scale values. The button will then show the text "Stop Measurements" and if clicked then the displaying of the scale values will be off and the button will show the text "Start Measurements".
- Click the "Clear" button to remove all the prevoiusly displayed values from the textbox.
- When you want to calibrate the scales, click the "Calibrate button". A messagebox will appear showing the message "Place known mass on each scale...". Place on each scale an object having the set weight for calibration (in our case the set weight is 6.1 grams, and the object is a 50 bani coin). After placing both objects on scales, click the OK button from the messagebox. The next values to be displayed should be close to the set weight.
- In the lower part of the GUI window, in the Settings section there is the setting for the time intetrval in milliseconds between the measurements. The default value is 100 ms. If you want to change that, type the wanted time interval value in the entry label (where it initially shows 100) and then click the "Update" button.