import serial
import struct
import time
import numpy as np
from protocol import proto_module
import logging

class ArduinoCommunication:
    """
    Handles communication with the Arduino over a serial connection.
    Provides methods to send and receive structured messages using the defined protocol.
    """
    
    def __init__(self, port='COM3', baudrate=115200, timeout=1):
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
        
        # Ensure payload is in bytes format
        if not isinstance(message.payload_, bytes):
            payload = message.payload_.tobytes()
        else:
            payload = message.payload_
        
        print("header: ", header)
        print("payload: ", payload[:message.payloadSize_])
        self.connection.write(header + payload[:message.payloadSize_])


    def read_message(self):
        """
        Read a structured message from the Arduino.

        :return: A message object or None if no data is available.
        """

        while self.connection.in_waiting == 0:
        # while self.connection.in_waiting < 3:
            print(self.connection.in_waiting)
            time.sleep(0.02)
            
        
        # Read the header first
        header = self.connection.read(3)  # 3 bytes for the header
        operationType, messageType, payloadSize = struct.unpack('BBB', header)
            
        # Now that we have the header, check if enough data is available for the payload
        # while self.connection.in_waiting < payloadSize:
            # time.sleep(0.01)  # Wait until there is enough data
            
        # Read the payload
        print("header: ", str(header))
        payload = self.connection.read(payloadSize)
        print("payload: ", payload[:payloadSize])
            
        # Convert payload to numpy array
        payload_array = np.frombuffer(payload, dtype=np.uint8)

        # Construct the message from the header and payload
        message = proto_module.Message()
        message.operationType_ = operationType
        message.messageType_ = messageType
        message.payloadSize_ = payloadSize
        message.payload_ = payload_array  # Assigning the numpy array

        return message

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
        msg = self.read_message()
        response = proto_module.CalibrateResponse()
        response.FromMessage(msg)
        
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
        msg = self.read_message()
        response = proto_module.WeightResponse()
        response.FromMessage(msg)
        return response
    
    def setup(self, request):
        """
        Send setup data to the Arduino and receive a response.

        :param scale_index: The index of the scale to setup.
        :param baud: The baud rate.
        :param setup_structure: The setup structure.

        :return: A response message from the Arduino.
        """
        
        # Serialize the request to a Message
        message = request.ToMessage()
        
        # Send the message to Arduino
        self.send_message(message)

        # Read the response from Arduino
        msg = self.read_message()
        response = proto_module.AddScaleResponse()
        response.FromMessage(msg)
        
        return response
    
    def close(self):
        """
        Close the connection to the Arduino.
        """
        if self.connection:
            self.connection.close()
