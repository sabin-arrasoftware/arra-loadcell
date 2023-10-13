import tkinter as tk
from tkinter import ttk
from datetime import datetime

MAXIMUM_DISPLAY_LINES = 30

class TextArea:
    """
    Represents a text area widget in the GUI.
    """
    def __init__(self, parent):
        """
        Initialize the TextArea.

        :param parent: The parent widget.
        """
        self.text_area = tk.Text(parent, height=MAXIMUM_DISPLAY_LINES, width=100)
        self.text_area.pack(pady=20)
#
    def insert(self, text: str):
        """
        Inserts the given text at the end of the text area.

        :param text: The text to be inserted.
        """
        now = datetime.now().strftime("%H:%M:%S")
        self.text_area.insert(tk.END, now + ": " + text + "\n")
        # Scroll to the end of the Text widget
        self.text_area.see(tk.END)

    def clear(self):
        """
        Clears all the content from the values, timestamps and text area.
        """
        self.text_area.delete(1.0, tk.END)
