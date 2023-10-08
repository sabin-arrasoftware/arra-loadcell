import tkinter as tk
import serial
import threading
import time
from views.view_manager import ViewManager
from controllers.callback_manager import CallbackManager, Events
from services.arduino_communication import ArduinoCommunication

class ArduinoAppController:
    """
    Controller class for the Arduino application. Manages the main application logic,
    including handling user interactions, managing views, and communicating with the Arduino.
    """
    
    def __init__(self, root):
        """
        Initialize the ArduinoAppController.

        :param root: The root tkinter window.
        """
        self.root = root

        # Initialize CallbackManager
        self.callback_manager = CallbackManager()

        # Register events
        self.callback_manager.register(Events.TOGGLE_START, self.toggle_start)
        self.callback_manager.register(Events.CLEAR_TEXT, self.clear_text)
        self.callback_manager.register(Events.CALIBRATE, self.calibrate)
        self.callback_manager.register(Events.UPDATE_SETTINGS, self.update_settings)

        # Initialize the ViewManager
        self.view_manager = ViewManager(self.root, self.callback_manager)

        # Initialize Communication
        self.communication = self.initializeArduinoCommunication()

    def initializeArduinoCommunication(self):
        """
        Initialize the Arduino communication based on the settings provided in the view.

        :return: An instance of ArduinoCommunication.
        """
        port = self.view_manager.get_setting_val("Connection Port")
        baudrate = int(self.view_manager.get_setting_val("Baud Rate"))
        return ArduinoCommunication(port, baudrate)

    def toggle_start(self):
        """
        Toggle the start/stop state of the weight reading loop.
        """
        self.thread_stop = self.view_manager.get_toggle_val()
        if self.thread_stop:
            self.thread = threading.Thread(target=self.read_weights_loop)
            self.thread.start()
        else:
            if hasattr(self, 'thread'):
                self.thread.join()  # Wait for the thread to finish

    def read_weights_loop(self):
        """
        Continuously read weights from the Arduino at a specified interval and update the view.
        """
        interval = int(self.view_manager.get_setting_val("Update Interval"))
        while self.thread_stop:
            resp = self.communication.get_weights()
            self.view_manager.insert_text_area(resp.ToString())
            time.sleep(interval)

    def clear_text(self):
        """
        Clear the text area in the view.
        """
        self.view_manager.clear_text_area()

    def calibrate(self):
        """
        Start the calibration process for the Arduino scales.
        """
        # Start calibration
        txt = "Place known mass on each scale..."
        gui_response = self.view_manager.ask("Calibration", txt)
    
        if gui_response:
            refMass = float(self.view_manager.get_setting_val("Calibration Mass"))
            response = self.communication.calibrate(0, refMass)
            
            txt = "Calibration failed. Please try again."
            if response.success:  
                txt = "Calibration was successful!"
    
            self.view_manager.inform("Calibration", txt)  # Fixed the variable name from 'text' to 'txt'

    def update_settings(self):
        """
        Notify the user that the settings have been updated.
        """
        message = f"Settings were updated!"
        response = self.view_manager.ask("Update Settings", message)
