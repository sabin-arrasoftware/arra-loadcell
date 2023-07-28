import tkinter as tk
from tkinter import ttk
from datetime import datetime
import serial
from tkinter import messagebox

# Global variables
MAXIMUM_DISPLAY_LINES = 40
SCALES_VALUES_SEPARATOR = "  "
SPACE_BETWEEN_FRAMES = 10
NUMBER_OF_SCALES = 2

class Scale:
    # sa tina datele si metodele legate de fiecare cantar
    def __init__(self, parent, scale_num):
        self.scale_num = scale_num
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
        if self.is_displaying.get():
            filename = f"scale{self.scale_num}_values.txt"
            with open(filename, "a") as file:
                file.write("-----\n")

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
            app.calibration_in_progress = True
            command = bytes([0, self.scale_num -1])
            print("Command: ", command)

            message = f"Place known mass on scale {self.scale_num} ..."
            response = messagebox.askokcancel("Calibration", message)

            if response:
                app.serial_port.write(command)
                self.parent.after(100, self.stop_monitoring)
                app.calibration_in_progress = False
            else:
                print("Calibration canceled")
        else:
            print("Invalid scale number")

    def stop_monitoring(self):
        command = bytes([0, self.scale_num - 1, 0])
        app.serial_port.write(command)
        print("Close Monitoring Command sent")


class ScaleDisplayApp:
    # Sa se ocupe display-ul datelor
    def __init__(self, window):
        self.window = window
        self.current_display_tab = ttk.Frame(window)
        self.scales = {}
        self.interval = 20
        self.calibration_in_progress = False
        self.serial_port = serial.Serial("/dev/ttyUSB0", 9600)

        self.create_widgets()

    def create_widgets(self):
        self.create_current_display_tab()
        self.create_settings_section()

    def create_current_display_tab(self):
        self.current_display_tab.pack(fill="both", expand=True)

        for scale_num in range(1, NUMBER_OF_SCALES + 1):
            scale_frame = self.create_scale_frame(scale_num)
            self.create_scale_widgets(scale_frame, scale_num)
            self.process_display(scale_num)
    
    def create_scale_frame(self, scale_num):
        scale_frame = ttk.Frame(self.current_display_tab, width=self.get_display_frame_width())
        scale_frame.grid(row=1, column=scale_num - 1, padx=SPACE_BETWEEN_FRAMES, pady=10)
        return scale_frame
    
    def create_scale_widgets(self, scale_frame, scale_num):
        self.create_scale_text_widget(scale_frame, scale_num)
        self.create_scale_checkbox(scale_num)            
        self.create_clear_button(scale_frame, scale_num)
        self.create_calibrate_button(scale_frame, scale_num)

    def create_scale_text_widget(self, parent, scale_num):
        self.scales[scale_num] = Scale(self.current_display_tab, scale_num)
        self.scales[scale_num].create_text_widget(parent)

    def create_scale_checkbox(self, scale_num):
        scale_checkbox = tk.Checkbutton(
            self.current_display_tab, text=f"Display Scale {scale_num}", variable=self.scales[scale_num].is_displaying,
            command=self.scales[scale_num].toggle_display()
        )
        scale_checkbox.grid(row=0, column=scale_num - 1, padx=SPACE_BETWEEN_FRAMES, pady=10)
        self.scales[scale_num].is_displaying.set(False)

    def process_display(self, scale_num):
        if self.scales[scale_num].is_displaying.get():
            values = self.read_serial_values()
            if values:
                if self.calibration_in_progress:
                    self.handle_calibration_messages(values)
                else:
                    self.handle_display_values(values, scale_num)

        self.window.after(self.interval, lambda: self.process_display(scale_num))

    def create_clear_button(self, parent, scale_num):
        clear_button = tk.Button(parent, text="Clear", command=self.scales[scale_num].clear_values)
        clear_button.pack()

    def create_calibrate_button(self, parent, scale_num):
        calibrate_button = tk.Button(
            parent, text="Calibrate", command=self.scales[scale_num].send_calibration_command
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

    def handle_calibration_messages(self, values):
        if values == ['Calling callback']:
            self.calibration_in_progress = False

    def handle_display_values(self, values, scale_num):
        if values not in [[''], ['cmd: Calibrate_1'], ['cmd: Calibrate_2'], ['Calling callback']]:
            self.scales[scale_num].display_value(values[scale_num - 1].split(":")[1].strip())

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


