import configparser
import tkinter as tk
from tkinter import ttk
from math import ceil, sqrt

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

class Tab:
    def __init__(self, tab_name, parent, settings):
        """
        Initialize the Settings frame.

        :param parent: The parent widget.
        """
        self.tab_name = tab_name
        self.tab = ttk.Frame(parent)
        parent.add(self.tab, text=self.tab_name)
        self.entries = {}
          
        # Calculate grid dimensions
        total = len(settings)
        rows = ceil(sqrt(total))
        cols = ceil(total / rows)

        counter = -1
        for name, default in settings.items():
            counter += 1
            # Determine the row and column for the setting            
            row = counter // cols
            # "2*" because we have 1 label + 1 text_entry for each Entry 
            col = 2*(counter % cols) 
            # Create entry
            self.entries[name] = Entry(self.tab, name, row, col, default)
            
    def get(self, name):
        """
        Get the current value of a specific setting.

        :param name: The name of the setting.
        :return: The value of the setting.
        """
        return self.entries[name].get()
        
class Settings:
    """
    Represents a frame containing multiple setting entries.
    """
    def __init__(self, parent, config_path):
        """
        Initialize the Settings frame.

        :param parent: The parent widget.
        """
        self.config_path = config_path
        
        # Dictionary to store settings
        self.settings = {}
        self.tabs = {}
        
        self.draw(parent)

    def draw(self, parent):
        """Renders the settings UI frame and populates it with tabs based on the config file."""
        # Create a frame for settings
        frame = ttk.LabelFrame(parent, text="Settings", padding=(10, 5))
        frame.pack(pady=20, padx=20, fill="both", expand=True)

        # Create a notebook for tabs
        notebook = ttk.Notebook(frame)
        notebook.grid(row=0, column=0, columnspan=3, pady=5, padx=5, sticky="nsew")

        # Create tabs based on config file
        config = configparser.ConfigParser()
        config.read(self.config_path)

        for section in config.sections():
            self.tabs[section] = Tab(section, notebook, config[section])
            for name in config[section].keys():
                self.settings[name] = section

    def get(self, name):
        """
        Get the current value of a specific setting.

        :param name: The name of the setting.
        :return: The value of the setting.
        """
        tab_name = self.settings[name]
        return self.tabs[tab_name].get(name)

    def update(self):
        """Update the configuration file with the current settings."""
        config = configparser.ConfigParser()
        for name in self.settings:
            section = self.settings[name]
            value = self.tabs[section].get(name)
            
            # Ensure the section exists.
            if section not in config:
                config[section] = {}
                
            config[section][name] = value
        
        with open(self.config_path, 'w') as configfile:
            config.write(configfile)