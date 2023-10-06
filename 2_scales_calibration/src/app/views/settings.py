import tkinter as tk
from tkinter import ttk

class SettingsFrame(ttk.LabelFrame):
    def __init__(self, root, text_area):
        super().__init__(root, text="Settings")
        self.pack(pady=20, padx=20, fill="x")
        self.text_area = text_area
        # ... rest of the logic ...
