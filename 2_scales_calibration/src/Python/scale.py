import tkinter as tk
from datetime import datetime
from tkinter import messagebox

MAXIMUM_DISPLAY_LINES = 40

class Scale:
    def __init__(self, parent):
        self.values = []
        self.timestamps = []
        self.is_displaying = tk.BooleanVar(value=False)
        self.parent = parent
        self.text = None

    def create_text_widget(self, parent):
        self.text = tk.Text(parent, relief="solid", width=self.get_display_frame_width() // 8, height=40)
        self.text.pack(expand=True, fill="both")

    def get_display_frame_width(self):
        return (self.parent.winfo_screenwidth() * 45) // 100

    def toggle_display(self):
        self.is_displaying.set(not self.is_displaying.get())

    def display_value(self, value):
        self.add_value(value)
        self.remove_excess_values()
        self.update_display()

    def add_value(self, value):
        self.values.append(round(float(value), 2))
        self.timestamps.append(datetime.now().strftime("%H:%M:%S.%f"))

    def remove_excess_values(self):
        if len(self.values) > MAXIMUM_DISPLAY_LINES:
            self.values.pop(0)
            self.timestamps.pop(0)

    def update_display(self):
        self.text.delete(1.0, tk.END)
        for value, timestamp in zip(self.values, self.timestamps):
            line = f"{timestamp}: {value}\n"
            self.text.insert(tk.END, line)

    def clear_values(self):
        self.values.clear()
        self.timestamps.clear()
        self.text.delete(1.0, tk.END)

    def send_calibration_command(self):
        if self.scale_num == 1 or self.scale_num == 2:
            # app.calibration_in_progress = True
            command = bytes([0, self.scale_num - 1])
            print("Command: ", command)

            message = f"Place known mass on scale {self.scale_num} ..."
            response = messagebox.askokcancel("Calibration", message)

            if response:
                # app.serial_port.write(command)
                self.parent.after(100, self.stop_monitoring)
                # app.calibration_in_progress = False
            else:
                print("Calibration canceled")
        else:
            print("Invalid scale number")

    def stop_monitoring(self):
        command = bytes([0, self.scale_num - 1, 0])
        # app.serial_port.write(command)
        print("Close Monitoring Command sent")