import tkinter as tk
from tkinter import ttk

class StartButton(ttk.Button):
    def __init__(self, root, text_area):
        super().__init__(root, text="Start", command=self.toggle_start)
        self.pack(pady=10)
        self.text_area = text_area
        self.is_started = False
        # ... rest of the logic ...

    def toggle_start(self):
        if self.is_started:
            # Stop communication
            self.is_started = False
            self.start_button.config(text="Start")
            # Close the serial connection
            if self.arduino:
                self.arduino.close()
        else:
            # Start communication
            self.is_started = True
            self.start_button.config(text="Stop")
            # Open the serial connection
            self.arduino = serial.Serial('COM3', 9600)  # Change 'COM3' to your Arduino's port

class ClearButton(ttk.Button):
    def __init__(self, root, text_area):
        super().__init__(root, text="Clear", command=self.clear_text)
        self.pack(pady=10)
        self.text_area = text_area

    def clear_text(self):
        self.text_area.delete(1.0, tk.END)

class CalibrateButton(ttk.Button):
    def __init__(self, root, text_area):
        super().__init__(root, text="Calibrate", command=self.calibrate)
        self.pack(pady=10)
        self.text_area = text_area

    def calibrate(self):
        # Placeholder for calibration logic
        self.text_area.insert(tk.END, "Calibration started...\n")
