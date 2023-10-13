import tkinter as tk
from tkinter import ttk
from controllers.callback_manager import CallbackManager
from controllers.callback_manager import Events

class Entry:
    """
    Represents an individual setting entry in the GUI.
    """
    def __init__(self, parent, label, row, default_value):
        """
        Initialize the Entry.

        :param parent: The parent widget.
        :param label: The label for the setting.
        :param row: The row number where the entry should be placed.
        :param default_value: The default value for the setting.
        """
        self.label = ttk.Label(parent, text=label)
        self.label.grid(row=row, column=0, padx=10, pady=5, sticky="w")
        self.entry = ttk.Entry(parent)
        self.entry.grid(row=row, column=1, padx=10, pady=5)
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
    def __init__(self, parent, callback_manager: CallbackManager):
        """
        Initialize the Settings frame.

        :param parent: The parent widget.
        :param callback_manager: The callback manager to handle button events.
        """
        self.row = -1

        # Settings Frame
        frame = ttk.LabelFrame(parent, text="Settings", padding=(10, 5))
        frame.pack(pady=20, padx=20, fill="both", expand=True)

        # Settings
        initial_settings = {
            "Calibration Mass": "6.1", 
            "Update Interval": "1",
            "Connection Port": "/dev/ttyUSB0",
            "Baud Rate": "9600"
        }
        self.settings = {}
        for name, default_value in initial_settings.items():
            self.settings[name] = Entry(frame, name, self.next_row(), default_value)

        # Update Button
        # self.update_btn = ttk.Button(frame, text="Update", 
        #     command=lambda: callback_manager.execute(Events.UPDATE_SETTINGS))
        # self.update_btn.grid(row=self.next_row(), column=0, columnspan=2, pady=10)

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
