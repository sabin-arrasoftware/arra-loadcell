import serial

# Configure the serial port
port = "/dev/ttyUSB0"  # Replace with the correct port for your system
baudrate = 9600  # Set the baudrate to match the configuration of your Arduino

# Open the serial port
ser = serial.Serial(port, baudrate)

# Send a message as bytes
message = [0x00, 0x00]  # Replace with your desired message bytes

# message = [0x02, 0x01]
ser.write(bytes(message))

# Close the serial port
ser.close()
