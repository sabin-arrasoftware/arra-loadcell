import serial

# Configure the serial port
port = "/dev/ttyUSB0"  # Replace with the correct port for your system
baudrate = 9600  # Set the baudrate to match the configuration of your Arduino

# Open the serial port
ser = serial.Serial(port, baudrate)

# Send a message as bytes
# First byte is command type: 0 - calibration, 1 - config
# Second byte is scale index; for now we have 2 scales: 0 and 1
# Third and fourth byte form the calibration mass multiplied by 100
# Fifth byte is the number of readings
# message = [0x01, 0x01, 0x01, 0x90, 0x0F]  # 4.0 # Replace with your desired message bytes
# message = [0x01, 0x00, 0x02, 0x62, 0X0A]  # 6.1
message = [0x00, 0x00]


# message = [0x02, 0x01]
ser.write(bytes(message))

# Close the serial port
ser.close()
