import tkinter as tk
from tkinter import ttk
from datetime import datetime
import serial
import time
from tkinter import messagebox

# Global variables
MAXIMUM_DISPLAY_LINES = 40
SCALES_VALUES_SEPARATOR = "  "
SPACE_BETWEEN_FRAMES = 10

class ScaleDisplayApp:
    def __init__(self, window):
        self.window = window
        self.current_display_tab = ttk.Frame(window)  # Define current_display_tab here
        self.scales = {
            1: {"values": [], "timestamps": [], "is_displaying": False, "calibration_factor": 1.0},
            2: {"values": [], "timestamps": [], "is_displaying": False, "calibration_factor": 1.0}
        }
        self.interval = 1000
        self.calibration_in_progress = False
        self.serial_port = serial.Serial("/dev/ttyUSB0", 9600)

        self.create_widgets()

    def create_widgets(self):
        self.create_current_display_tab()
        self.create_settings_section()

    def create_current_display_tab(self):
        self.current_display_tab.pack(fill="both", expand=True)

        for scale_num in self.scales:
            scale_frame = ttk.Frame(self.current_display_tab, width=self.get_display_frame_width())
            scale_frame.grid(row=1, column=scale_num - 1, padx=SPACE_BETWEEN_FRAMES, pady=10)

            self.scales[scale_num]["text"] = tk.Text(scale_frame, relief="solid", width=self.get_text_width(), height=40)
            self.scales[scale_num]["text"].pack(expand=True, fill="both")

            self.scales[scale_num]["checkbox"] = tk.BooleanVar()
            scale_checkbox = tk.Checkbutton(
                self.current_display_tab, text=f"Display Scale {scale_num}", variable=self.scales[scale_num]["checkbox"],
                command=lambda num=scale_num: self.toggle_display(num)
            )
            scale_checkbox.grid(row=0, column=scale_num - 1, padx=SPACE_BETWEEN_FRAMES, pady=10)
            self.scales[scale_num]["checkbox"].set(self.scales[scale_num]["is_displaying"])

            self.process_display(scale_num)

            clear_button = tk.Button(scale_frame, text="Clear", command=lambda num=scale_num: self.clear_values(num))
            clear_button.pack()

            calibrate_button = tk.Button(
                scale_frame, text="Calibrate", command=lambda num=scale_num: self.send_calibration_command(num)
            )
            calibrate_button.pack()

    def create_settings_section(self):
        settings_frame = ttk.Frame(self.current_display_tab)
        settings_frame.grid(row=2, column=0, columnspan=2, padx=10, pady=10)

        settings_label = tk.Label(settings_frame, text="Settings", font=("Arial", 12))
        settings_label.grid(row=0, column=0, columnspan=3, padx=5, pady=5)

        time_interval_label = tk.Label(settings_frame, text="Time Interval (ms):")
        time_interval_label.grid(row=1, column=0, padx=5, pady=5)

        self.time_interval_entry = tk.Entry(settings_frame, width=10)
        self.time_interval_entry.insert(tk.END, "1000")
        self.time_interval_entry.grid(row=1, column=1, padx=5, pady=5)

        update_button = tk.Button(settings_frame, text="Update", command=self.update_time_interval)
        update_button.grid(row=1, column=2, padx=5, pady=5)

    def get_display_frame_width(self):
        return (self.window.winfo_screenwidth() * 45) // 100

    def get_text_width(self):
        return self.get_display_frame_width() // 8

    def toggle_display(self, scale_num):
        self.scales[scale_num]["is_displaying"] = not self.scales[scale_num]["is_displaying"]
        if self.scales[scale_num]["is_displaying"]:
            filename = f"scale{scale_num}_values.txt"
            with open(filename, "a") as file:
                file.write("-----\n")

    def process_display(self, scale_num):
        if self.scales[scale_num]["is_displaying"]:
            values = self.read_serial_values()
            if values:
                if self.calibration_in_progress:
                    self.handle_calibration_messages(values)
                else:
                    self.handle_display_values(values, scale_num)

        self.window.after(self.interval, lambda: self.process_display(scale_num))

    def handle_calibration_messages(self, values):
        if values == ['Calling callback']:
            self.calibration_in_progress = False

    def handle_display_values(self, values, scale_num):
        if values not in [[''], ['cmd: Calibrate_1'], ['cmd: Calibrate_2'], ['Calling callback']]:
            self.display_value(scale_num, values[scale_num - 1].split(":")[1].strip())

    def display_value(self, scale_num, value):
        self.add_value(scale_num, value)
        self.remove_excess_values(scale_num)
        self.update_display(scale_num)

    def add_value(self, scale_num, value):
        self.scales[scale_num]["values"].append(round(float(value), 2))
        self.scales[scale_num]["timestamps"].append(datetime.now().strftime("%H:%M:%S.%f"))

    def remove_excess_values(self, scale_num):
        scale_values = self.scales[scale_num]["values"]
        scale_timestamps = self.scales[scale_num]["timestamps"]
        if len(scale_values) > MAXIMUM_DISPLAY_LINES:
            scale_values.pop(0)
            scale_timestamps.pop(0)

    def update_display(self, scale_num):
        scale_values = self.scales[scale_num]["values"]
        scale_timestamps = self.scales[scale_num]["timestamps"]
        scale_values_text = self.scales[scale_num]["text"]
        scale_values_text.delete(1.0, tk.END)
        for value, timestamp in zip(scale_values, scale_timestamps):
            line = f"{timestamp}: {value}\n"
            scale_values_text.insert(tk.END, line)
        self.write_to_file(scale_num, line)
    
    # Function to write the last line to a file for a scale
    def write_to_file(self, scale_num, line):
        filename = f"scale{scale_num}_values.txt"
        with open(filename, "a") as file:
            file.write(line)  # Append the latest line to the file

    def clear_values(self, scale_num):
        self.scales[scale_num]["values"].clear()
        self.scales[scale_num]["timestamps"].clear()
        scale_values_text = self.scales[scale_num]["text"]
        scale_values_text.delete(1.0, tk.END)

    def start_monitoring(self, scale_num):
        command = bytes([0, scale_num - 1, 1])  # Command to Start Monitoring
        self.serial_port.write(command)
        print("Start Monitoring Command sent")
    
    def close_monitoring(self, scale_num):
        command = bytes([0, scale_num - 1, 0])  # Command to Close Monitoring
        self.serial_port.write(command)
        print("Close Monitoring Command sent")

    def send_calibration_command(self, scale_num):
        if scale_num == 1 or scale_num == 2:
            self.calibration_in_progress = True
            # command = bytearray([0, scale_num - 1]) # Prepare to send the command as bytes
            command = bytes([0, scale_num -1])
            print("Command: ", command)

            message = f"Place known mass on scale {scale_num} ..."
            response = messagebox.askokcancel("Calibration", message)

            if response:
                self.serial_port.write(command)
                # self.start_monitoring(scale_num)
                # time.sleep(0.1)
                # self.close_monitoring(scale_num)
                self.calibration_in_progress = False
            else:
                print("Calibration canceled")
        else:
            print("Invalid scale number")

    def read_serial_values(self):
        if self.serial_port.in_waiting > 0:
            line = self.serial_port.readline().decode().strip()
            values = line.split(SCALES_VALUES_SEPARATOR)
            return values
        return None

    def update_time_interval(self):
        new_interval = self.time_interval_entry.get()
        if new_interval.isdigit():
            new_interval = int(new_interval)
            if new_interval > 0:
                self.interval = new_interval

if __name__ == "__main__":
    window = tk.Tk()
    window.title("Scale Display")
    app = ScaleDisplayApp(window)
    window.mainloop()