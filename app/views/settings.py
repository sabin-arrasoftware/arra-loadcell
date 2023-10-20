import tkinter as tk
from tkinter import ttk
from controllers.callback_manager import CallbackManager
from controllers.callback_manager import Events

class Entry:
    """
    Represents an individual setting entry in the GUI.
    """
    def __init__(self, parent, label, row, column, default_value):
        """
        Initialize the Entry.

        :param parent: The parent widget.
        :param label: The label for the setting.
        :param row: The row number where the entry should be placed.
        :param column: The column number where the entry should be placed.
        :param default_value: The default value for the setting.
        """
        self.label = ttk.Label(parent, text=label)
        self.label.grid(row=row, column=column, padx=10, pady=5, sticky="w")
        self.entry = ttk.Entry(parent)
        self.entry.grid(row=row, column=column + 1, padx=10, pady=5)
        self.entry.insert(0, default_value)

    def get(self):
        """
        Get the current value of the setting entry.

        :return: The value of the setting entry.
        """
        return self.entry.get()

class Settings:
    """
    Represents a frame containing multiple setting entries.
    """
    def __init__(self, parent):
        """
        Initialize the Settings frame.

        :param parent: The parent widget.
        """
        self.row = -1

        # Dictionary to store settings
        self.settings = {}

        # Create a frame for settings
        frame = ttk.LabelFrame(parent, text="Settings", padding=(10, 5))
        frame.pack(pady=20, padx=20, fill="both", expand=True)

        # Create a notebook for tabs
        notebook = ttk.Notebook(frame)
        notebook.grid(row=0, column=0, columnspan=3, pady=5, padx=5, sticky="nsew")

        # Create tabs for General settings and Arduino settings
        self.create_tab(notebook, "General Settings", self.get_general_settings())
        self.create_tab(notebook, "Arduino Settings", self.get_arduino_settings())

    def create_tab(self, notebook, tab_name, initial_settings):
        """
        Create a tab for settings in the notebook.

        :param notebook: The notebook which contains the tab.
        :param tab_name: The name of the tab.
        :param initial_settings: The dictionary with the initial settings values.
        """

        tab = ttk.Frame(notebook)
        notebook.add(tab, text=tab_name)

        # These settings are grouped in columns with 2 rows each
        col = 0
        settings_index = 0

        for name, default_value in initial_settings.items():
            # Determine the row and column for the setting            
            row = self.next_row() % 2
            # Create an Entry object for the setting
            self.settings[name] = Entry(tab, name, row, col, default_value)
            settings_index += 1
            col = settings_index // 2 * 2

    def next_row(self):
        """
        Get the next available row number for placing a widget.

        :return: The next available row number.
        """
        self.row += 1
        return self.row

    def get(self, name):
        """
        Get the current value of a specific setting.

        :param name: The name of the setting.
        :return: The value of the setting.
        """
        return self.settings[name].get()
    
    def get_general_settings(self):
        """
        Define initial values for General settings

        :return: A dictionary with general settings.
        """
        return {
            "Calibration Mass": "6.1", 
            "Update Interval": "1"
        }

    def get_arduino_settings(self):
        """
        Define initial values for Arduino settings

        :return: A dictionary with Arduino settings.
        """
        return {
            "Connection Port": "/dev/ttyUSB0",
            "Baud Rate": "9600",
            "DT1": "14",
            "SCK1": "15",
            "DT2": "18",
            "SCK2": "19"
        }

