# Load Cell GUI

## Description
This Python code provides a graphical user interface (GUI) for displaying the output values from load cells. 

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
Configuration:
    Load cell -> HX711:
        red   -> E+
        black -> E-
        white -> A-
        green -> A+
    
    1st HX711 -> Arduino:
        GND -> GND
        DT  -> A1
        SCK -> A0
        VCC -> 5V
    
    2nd HX711 -> Arduino:
        GND -> GND
        DT  -> A3
        SCK -> A2
        VCC -> 5V
    
    Arduino -> USB cable -> your machine (Virtual Machine on laptop, e.g.)

IMAGES:
![Pic1](res/IMG_20230407_155340.jpg)
![Pic2](res/IMG_20230407_155405.jpg)


## Usage
Upload code to Arduino:
- in Visual Studio Code go to 2_scales_calibration.ino file
- on top right side click icon Arduino:Upload

Start GUI:
- go to folder in Terminal: ...arra-loadcell/2_scales_calibration/src/Python 
- type: python3 main.py
- the GUI is displayed: ![GUI](<res/Screenshot from 2023-10-06 13-05-17.png>)

Buttons:
- Start Measurements -> start displaying measurements from scales
- Stop Measurements  -> stop displaying measurements from scales
- Clear              -> clear the textbox
- Calibrate          -> calibrate the scales with the set weight:
    - set weight: 6.1 g
    - object: 50 bani coin
    
    - after click, a message box appears: ![Calibrate messagebox](<res/Screenshot from 2023-10-06 13-16-27.png>)
    - put objects on scales
    - click OK

Close GUI:
- X button on top right of GUI window
