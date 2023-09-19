import struct

BUFFER_SIZE = 16
MAX_NR_SCALES = 4

class CommandType:
    CALIBRATE = 0
    CONFIG = 1
    WEIGHT = 2
    LAST = 3

class Buffer:
    def __init__(self):
        self.payload = bytearray([0] * BUFFER_SIZE)

class CalibrateMessage:
    def __init__(self, scaleIndex):
        self.scaleIndex = scaleIndex

class ConfigMessage:
    def __init__(self, scaleIndex, calibrationMass, numReadings):
        self.scaleIndex = scaleIndex
        self.calibrationMass = calibrationMass
        self.numReadings = numReadings

class WeightMessage:
    def __init__(self, numberOfScales, floatWeight):
        self.numberOfScales = numberOfScales
        self.floatWeight = floatWeight

def encode_calibrate_command(calibrateMessage, buffer):
    buffer.payload[0] = CommandType.CALIBRATE
    buffer.payload[1] = calibrateMessage.scaleIndex

def encode_config_command(configMessage, buffer):
    buffer.payload[0] = CommandType.CONFIG
    buffer.payload[1] = configMessage.scaleIndex
    calibrationMassInt = int(configMessage.calibrationMass * 100)
    buffer.payload[2] = calibrationMassInt >> 8 & 0xFF
    buffer.payload[3] = calibrationMassInt & 0xFF
    buffer.payload[4] = configMessage.numReadings

def encode_weight_command(weightMessage, buffer):
    buffer.payload[0] = CommandType.WEIGHT
    buffer.payload[1] = weightMessage.numberOfScales
    for i, weight in enumerate(weightMessage.floatWeight):
        intWeight = int(weight * 100)
        buffer.payload[2 * i + 2] = intWeight >> 8 & 0xFF
        buffer.payload[2 * i + 3] = intWeight & 0xFF

def decode_calibrate_command(buffer, calibrateMessage):
    calibrateMessage.scaleIndex = buffer.payload[1]

def decode_config_command(buffer, configMessage):
    configMessage.scaleIndex = buffer.payload[1]
    calibrationMassIntValue = (buffer.payload[2] << 8) | buffer.payload[3]
    configMessage.calibrationMass = float(calibrationMassIntValue) / 100.0
    configMessage.numReadings = buffer.payload[4]

def decode_weight_command(buffer, weightMessage):
    weightMessage.numberOfScales = buffer.payload[1]
    weightMessage.floatWeight = []
    for i in range(weightMessage.numberOfScales):
        intWeight = (buffer.payload[2 * i + 2] << 8) | buffer.payload[2 * i + 3]
        weightMessage.floatWeight.append(float(intWeight) / 100.0)

def is_calibrate_message(buffer):
    return buffer.payload[0] == CommandType.CALIBRATE

def is_config_message(buffer):
    return buffer.payload[0] == CommandType.CONFIG

def is_weight_message(buffer):
    return buffer.payload[0] == CommandType.WEIGHT

def get_command_type(buffer):
    return buffer.payload[0]

if __name__ == "__main__":
    # Sample data
    calibrate_message = CalibrateMessage(1)
    config_message = ConfigMessage(2, 123.45, 5)
    weight_message = WeightMessage(3, [1.23, 4.56, 7.89])

    # Create buffers
    buffer_cal = Buffer()
    buffer_con = Buffer()
    buffer_wei = Buffer()

    # Encode the messages
    encode_calibrate_command(calibrate_message, buffer_cal)
    encode_config_command(config_message, buffer_con)
    encode_weight_command(weight_message, buffer_wei)

    print(f"Encoded Calibrate Message: {buffer_cal.payload}")
    print(f"Encoded Config Message: {buffer_con.payload}")
    print(f"Encoded Weight Message: {buffer_wei.payload}")

    # Decode and print the messages
    decoded_calibrate_message = CalibrateMessage(None)
    decoded_config_message = ConfigMessage(None, None, None)
    decoded_weight_message = WeightMessage(None, None)

    decode_calibrate_command(buffer_cal, decoded_calibrate_message)
    decode_config_command(buffer_con, decoded_config_message)
    decode_weight_command(buffer_wei, decoded_weight_message)

    print(f"Decoded Calibrate Message: Scale Index = {decoded_calibrate_message.scaleIndex}")
    print(f"Decoded Config Message: Scale Index = {decoded_config_message.scaleIndex}, Calibration Mass = {decoded_config_message.calibrationMass}, Num Readings = {decoded_config_message.numReadings}")
    print(f"Decoded Weight Message: Number of Scales = {decoded_weight_message.numberOfScales}")
    for i, weight in enumerate(decoded_weight_message.floatWeight):
        print(f"  Scale {i + 1} Weight = {weight}")

    # Check message types
    if is_calibrate_message(buffer_cal):
        print("Buffer contains a CALIBRATE message.")
    if is_config_message(buffer_con):
        print("Buffer contains a CONFIG message.")
    if is_weight_message(buffer_wei):
        print("Buffer contains a WEIGHT message.")

    # Get command type
    command_type = get_command_type(buffer_cal)
    print(f"Command Type from buffer_cal: {command_type}")

    command_type = get_command_type(buffer_con)
    print(f"Command Type from buffer_con: {command_type}")

    command_type = get_command_type(buffer_wei)
    print(f"Command Type from buffer_wei: {command_type}")


