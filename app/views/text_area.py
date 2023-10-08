import tkinter as tk
from tkinter import ttk

class TextArea:
    """
    Represents a text area widget in the GUI.
    """
    def __init__(self, parent):
        """
        Initialize the TextArea.

        :param parent: The parent widget.
        """
        self.text_area = tk.Text(parent, height=40, width=100)
        self.text_area.pack(pady=20)

    def insert(self, text: str):
        """
        Inserts the given text at the end of the text area.

        :param text: The text to be inserted.
        """
        self.text_area.insert(tk.END, text)

    def clear(self):
        """
        Clears all the content from the text area.
        """
        self.text_area.delete(1.0, tk.END)
