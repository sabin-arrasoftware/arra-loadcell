import tkinter as tk
from tkinter import ttk
from controllers.callback_manager import CallbackManager
from controllers.callback_manager import Events

class Button:
    """
    Represents a basic button in the GUI.
    """
    def __init__(self, parent, text, cmd):
        """
        Initialize the Button.

        :param parent: The parent widget.
        :param text: The text displayed on the button.
        :param cmd: The command to execute when the button is clicked.
        """
        self.btn = ttk.Button(parent, text=text, command=cmd)
        self.btn.pack(side=tk.LEFT, padx=10)

class ToggleButton(Button):
    """
    Represents a toggle button in the GUI that can switch between two states.
    """
    def __init__(self, parent, text1, text2, cmd):
        """
        Initialize the ToggleButton.

        :param parent: The parent widget.
        :param text1: The text displayed on the button in the first state.
        :param text2: The text displayed on the button in the second state.
        :param cmd: The command to execute when the button is toggled.
        """
        super().__init__(parent, text1, self.toggle)
        self.cmd = cmd
        self.text1 = text1
        self.text2 = text2
        self.state = False  # False indicates the first state (text1)

    def toggle(self):
        """
        Toggle the button's state and update its text.
        """
        txt = self.text1 if self.state else self.text2
        self.btn.config(text=txt)
        self.state = not self.state
        self.cmd()

    def get_state(self):
        """
        Get the current state of the toggle button.

        :return: True if in the second state, False if in the first state.
        """
        return self.state

class ButtonsFrame:
    """
    Represents a frame containing multiple buttons.
    """
    def __init__(self, parent, callback_manager: CallbackManager):
        """
        Initialize the ButtonsFrame.

        :param parent: The parent widget.
        :param callback_manager: The callback manager to handle button events.
        """
        self.callback_manager = callback_manager
        
        # Buttons Frame
        self.frame = ttk.Frame(parent)
        self.frame.pack(pady=20)

        # Buttons
        self.toggle = ToggleButton(self.frame, "Start", "Stop", 
            lambda: callback_manager.execute(Events.TOGGLE_START))
        self.clear = Button(self.frame, "Clear",
            lambda: callback_manager.execute(Events.CLEAR_TEXT))
        self.calibrate = Button(self.frame, "Calibrate",
            lambda: callback_manager.execute(Events.CALIBRATE))
        self.setup = Button(self.frame, "Setup",
            lambda: callback_manager.execute(Events.SETUP))

    def get_toggle_val(self):
        """
        Get the current state of the toggle button.

        :return: True if in the second state, False if in the first state.
        """
        return self.toggle.get_state()
