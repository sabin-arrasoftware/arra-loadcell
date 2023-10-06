import serial

class ArduinoCommunication:
    def __init__(self):
        self.arduino = None

    def start_communication(self, port='COM3', baudrate=9600):
        self.arduino = serial.Serial(port, baudrate)

    def stop_communication(self):
        if self.arduino:
            self.arduino.close()

    # ... any other methods related to Arduino communication ...
