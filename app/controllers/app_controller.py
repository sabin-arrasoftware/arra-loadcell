import tkinter as tk
import threading
import time
from protocol import proto_module
from views.view_manager import ViewManager
from controllers.callback_manager import CallbackManager, Events
from services.arduino_communication import ArduinoCommunication
from controllers.scheduler import Scheduler
import logging

class SetupStructure:
    def __init__(self, baud, dt1, sck1, dt2, sck2):
        self.baud = baud
        self.dt1 = dt1
        self.sck1 = sck1
        self.dt2 = dt2
        self.sck2 = sck2

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

        # Initialize Scheduler
        self.scheduler = Scheduler()

        # Initialize CallbackManager
        self.callback_manager = CallbackManager()

        # Register events
        self.callback_manager.register(Events.TOGGLE_START, self.toggle_start)
        self.callback_manager.register(Events.CLEAR_TEXT, self.clear_text)
        self.callback_manager.register(Events.CALIBRATE, self.calibrate)
        self.callback_manager.register(Events.SETUP, self.setup)        
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
        print("thread_stop: ", self.thread_stop)
        if self.thread_stop:
            interval = int(self.view_manager.get_setting_val("Update Interval"))
            print("Update interval: ", interval)
            logging.debug("Update interval: ", interval)
            self.scheduler.schedule("GetWeights", self.read_weights, interval, 0)
        else:
            self.scheduler.stop("GetWeights")


    def read_weights(self):
        """
        Continuously read weights from the Arduino at a specified interval and update the view.
        """
        resp = self.communication.get_weights()
        dispaly_str = str(resp.floatWeight_[0])
        print("display_str: ", dispaly_str)
        logging.debug("display_str: ", dispaly_str)
        self.view_manager.insert_text_area(dispaly_str)


    def clear_text(self):
        """
        Clear the text area in the view.
        """
        self.view_manager.clear_text_area()

    def calibrate(self):
        """
        Start the calibration process for the Arduino scales.
        """
        txt = "Place known mass on each scale..."
        gui_response = self.view_manager.ask("Calibration", txt)
    
        if gui_response:
            refMass = float(self.view_manager.get_setting_val("Calibration Mass"))
            response = self.communication.calibrate(0, refMass)
            txt = "Calibration failed. Please try again."
            if response.success_:  
                txt = "Calibration was successful!"
    
            self.view_manager.inform("Calibration", txt) 
    
    def setup(self):
        """
        Start the setup process for the Arduino scales.
        """
        txt = "Modify settings then press ok..."
        gui_response = self.view_manager.ask("Setup", txt)

        if gui_response:            
            baud = self.view_manager.get_setting_val("Baud Rate")
            dt1 = self.view_manager.get_setting_val("DT1")
            sck1 = self.view_manager.get_setting_val("SCK1")
            dt2 = self.view_manager.get_setting_val("DT2")
            sck2 = self.view_manager.get_setting_val("SCK2")
            setup_structure = SetupStructure(baud, dt1, sck1, dt2, sck2)
            response = self.communication.setup(0, setup_structure)

            # Save the updated settings to settings.ini
            self.view_manager.settings.save_settings()

            txt = "Setup failed. Please try again."
            if response.success_:  
                txt = "Setup was successful!"
    
            self.view_manager.inform("Setup", txt)
            
    def update_settings(self):
        """
        Notify the user that the settings have been updated.
        """
        message = f"Settings were updated!"
        self.view_manager.ask("Update Settings", message)
