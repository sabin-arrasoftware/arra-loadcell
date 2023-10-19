from services.arduino_communication import ArduinoCommunication
from protocol import proto_module

def main():
    # Initialize the ArduinoCommunication class
    arduino_comm = ArduinoCommunication(port='/dev/ttyUSB0', baudrate=9600, timeout=1)  # Adjust the port as needed

    try:
        # Send a calibration request
        scale_index = 0  # Example scale index
        calibration_mass = 100.0  # Example calibration mass value
        msg = arduino_comm.calibrate(scale_index, calibration_mass)

        req = proto_module.CalibrateResponse()
        req.FromMessage(msg)

        print(req.success_)

        # Request weight data
        weight_response = arduino_comm.get_weights()
        print(f"Weight Response: {weight_response}")

    except Exception as e:
        print(f"An error occurred: {e}")

    finally:
        # Close the connection
        arduino_comm.close()

if __name__ == "__main__":
    main()
