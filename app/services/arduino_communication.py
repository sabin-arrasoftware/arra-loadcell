import serial
import struct
from protocol import proto_module

class ArduinoCommunication:
    """
    Handles communication with the Arduino over a serial connection.
    Provides methods to send and receive structured messages using the defined protocol.
    """
    
    def __init__(self, port='COM3', baudrate=9600, timeout=1):
        """
        Initialize the serial connection to the Arduino.

        :param port: The port to which the Arduino is connected.
        :param baudrate: The baud rate for the serial connection.
        :param timeout: Read timeout in seconds.
        """
        self.connection = serial.Serial(port, baudrate, timeout=timeout)
        self.connection.flush()

    def send_message(self, message):
        """
        Send a structured message to the Arduino.

        :param message: The message object to send.
        """
        # Convert the message to bytes and send to Arduino
        header = struct.pack('BBB', message.operationType_, message.messageType_, message.payloadSize_)
        self.connection.write(header + message.payload_[:message.payloadSize_])

    def read_message(self):
        """
        Read a structured message from the Arduino.

        :return: A message object or None if no data is available.
        """
        if self.connection.in_waiting > 0:
            # Read the header first
            header = self.connection.read(3)  # 3 bytes for the header
            operationType, messageType, payloadSize = struct.unpack('BBB', header)
            payload = self.connection.read(payloadSize)
            
            # Construct the message from the header and payload
            message = proto_module.Message()
            message.operationType_ = operationType
            message.messageType_ = messageType
            message.payloadSize_ = payloadSize
            message.payload_ = payload  # Adjust if payload_ expects a fixed-size array
            
            return message
        return None

    def calibrate(self, scale_index, calibration_mass):
        """
        Send calibration data to the Arduino and receive a response.

        :param scale_index: The index of the scale to calibrate.
        :param calibration_mass: The calibration mass value.
        :return: A response message from the Arduino.
        """
        calibrate_request = proto_module.CalibrateRequest()
    
        # Set the necessary attributes
        calibrate_request.scaleIndex_ = scale_index
        calibrate_request.calibrationMass_ = calibration_mass
        
        # Serialize the request to a Message
        message = calibrate_request.ToMessage()
        
        # Send the message to Arduino
        self.send_message(message)

        # Read the response from Arduino
        response = self.read_message()
        return response

    def get_weights(self):
        """
        Request weight data from the Arduino and receive a response.

        :return: A response message from the Arduino.
        """
        weight_request = proto_module.WeightRequest()
        message = weight_request.ToMessage()
        self.send_message(message)

        # Read the response from Arduino
        response = self.read_message()
        return response

    def close(self):
        """
        Close the connection to the Arduino.
        """
        if self.connection:
            self.connection.close()
