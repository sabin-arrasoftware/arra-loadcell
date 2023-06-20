import tkinter as tk
from tkinter import ttk
from datetime import datetime
import serial
from tkinter import messagebox

# Global variables
calibration_in_progress = False

MAXIMUM_DISPLAY_LINES = 40
SCALES_VALUES_SEPARATOR = "  "

# Create the Tkinter window
window = tk.Tk()
window.title("Scale Display")

# Get the screen width and height
screen_width = window.winfo_screenwidth()
screen_height = window.winfo_screenheight()

# Calculate the dimensions for the window and display frames
window_width = screen_width // 2
window_height = screen_height
display_frame_width = (window_width * 45) // 100
space_between_frames = (window_width * 4) // 100

# Set the window size and position
window.geometry(f"{window_width}x{window_height}+{window_width // 2}+{window_height // 2}")

# Variables to store the last 10 values and timestamps for each scale
scale1_values = []
scale1_timestamps = []
scale2_values = []
scale2_timestamps = []

# Dictionary to map scale numbers to their respective values, timestamps, and variables
scales = {
    1: {"values": scale1_values, "timestamps": scale1_timestamps, "is_displaying": False, "calibration_factor": 1.0},
    2: {"values": scale2_values, "timestamps": scale2_timestamps, "is_displaying": False, "calibration_factor": 1.0}
}

global interval
interval = 1000

# Function to start or stop displaying numbers for a scale
def toggle_display(scale_num):
    scales[scale_num]["is_displaying"] = not scales[scale_num]["is_displaying"]
    if scales[scale_num]["is_displaying"]:
        filename = f"scale{scale_num}_values.txt"
        with open(filename, "a") as file:
            file.write("-----\n")

# Function to process the display of the numbers for a scale
def process_display(scale_num):
    global calibration_in_progress

    if scales[scale_num]["is_displaying"]:
        values = read_serial_values()
        print("values from process_display: ", values)
        if values:
            print("cip: ", calibration_in_progress)

            if calibration_in_progress:
                handle_calibration_messages(values)
            else:
                handle_display_values(values, scale_num)

    global interval
    window.after(interval, lambda: process_display(scale_num))

def handle_calibration_messages(values):
    global calibration_in_progress
    print("values from handle_calibration_messages: ", values)
    if values == ['Calling callback']:
        calibration_in_progress = False

# Function to handle display values
def handle_display_values(values, scale_num):    
    print("values from handle_display_values: ", values)
    if values not in [[''], ['cmd: Calibrate_1'], ['cmd: Calibrate_2'], ['Calling callback']]:
        display_value(scale_num, values[scale_num - 1].split(":")[1].strip())

def display_value(scale_num, value):
    add_value(scale_num, value)
    remove_excess_values(scale_num)
    update_display(scale_num)

def add_value(scale_num, value):
    scale_values = scales[scale_num]["values"]
    scale_timestamps = scales[scale_num]["timestamps"]
    scale_values.append(round(float(value), 2))
    scale_timestamps.append(datetime.now().strftime("%H:%M:%S.%f"))

def remove_excess_values(scale_num):
    scale_values = scales[scale_num]["values"]
    scale_timestamps = scales[scale_num]["timestamps"]
    if len(scale_values) > MAXIMUM_DISPLAY_LINES:
        scale_values.pop(0)
        scale_timestamps.pop(0)

def update_display(scale_num):
    scale_values = scales[scale_num]["values"]
    scale_timestamps = scales[scale_num]["timestamps"]
    scale_values_text = scales[scale_num]["text"]
    scale_values_text.delete(1.0, tk.END)
    for value, timestamp in zip(scale_values, scale_timestamps):
        line = f"{timestamp}: {value}\n"
        scale_values_text.insert(tk.END, line)
    write_to_file(scale_num, line)

# Function to write the last line to a file for a scale
def write_to_file(scale_num, line):
    filename = f"scale{scale_num}_values.txt"
    with open(filename, "a") as file:
        file.write(line)  # Append the latest line to the file

# Function to clear the values and timestamps for a scale
def clear_values(scale_num):
    scale_values = scales[scale_num]["values"]
    scale_timestamps = scales[scale_num]["timestamps"]
    scale_values.clear()
    scale_timestamps.clear()
    scale_values_text = scales[scale_num]["text"]
    scale_values_text.delete(1.0, tk.END)

# Create the "Current Display" tab
current_display_tab = ttk.Frame(window)
current_display_tab.pack(fill="both", expand=True)

# Create the checkboxes and value displays for each scale in the "Current Display" tab
for scale_num in scales:
    scale_values = scales[scale_num]["values"]
    scale_timestamps = scales[scale_num]["timestamps"]
    scale_is_displaying = scales[scale_num]["is_displaying"]

    scale_checkbox_var = tk.BooleanVar()
    scale_checkbox = tk.Checkbutton(
        current_display_tab, text=f"Display Scale {scale_num}", variable=scale_checkbox_var,
        command=lambda num=scale_num: toggle_display(num)
    )
    scale_checkbox.grid(row=0, column=scale_num - 1, padx=space_between_frames, pady=10)

    scale_frame = tk.Frame(current_display_tab, width=display_frame_width)
    scale_frame.grid(row=1, column=scale_num - 1, padx=space_between_frames, pady=10)

    scale_values_text = tk.Text(scale_frame, relief="solid", width=display_frame_width // 8, height=40)
    scale_values_text.pack(expand=True, fill="both")

    scales[scale_num]["text"] = scale_values_text
    scales[scale_num]["checkbox"] = scale_checkbox_var
    scale_checkbox_var.set(scale_is_displaying)

    process_display(scale_num)

    clear_button = tk.Button(scale_frame, text="Clear", command=lambda num=scale_num: clear_values(num))
    clear_button.pack()

    calibrate_button = tk.Button(
        scale_frame, text="Calibrate", command=lambda num=scale_num: send_calibration_command(num)
    )
    calibrate_button.pack()


# Create the "Settings" section in the "Current Display" tab
settings_frame = tk.Frame(current_display_tab)
settings_frame.grid(row=2, column=0, columnspan=2, padx=10, pady=10)

settings_label = tk.Label(settings_frame, text="Settings", font=("Arial", 12))
settings_label.grid(row=0, column=0, columnspan=3, padx=5, pady=5)

time_interval_label = tk.Label(settings_frame, text="Time Interval (ms):")
time_interval_label.grid(row=1, column=0, padx=5, pady=5)

time_interval = tk.Entry(settings_frame, width=10)
time_interval.insert(tk.END, "1000")
time_interval.grid(row=1, column=1, padx=5, pady=5)

update_button = tk.Button(settings_frame, text="Update", command=lambda: update_time_interval())
update_button.grid(row=1, column=2, padx=5, pady=5)

# Function to update the time interval
def update_time_interval():
    new_interval = time_interval.get()
    if new_interval.isdigit():
        new_interval = int(new_interval)
        if new_interval > 0:
            global interval
            interval = new_interval

# Configure the grid layout to adjust the column widths
current_display_tab.grid_columnconfigure(0, weight=1)
current_display_tab.grid_columnconfigure(1, weight=1)

# Open the serial port
ser = serial.Serial("/dev/ttyUSB0", 9600)  # The appropriate serial port via USB cable

def send_calibration_command(scale_num):
    if scale_num == 1 or scale_num == 2:
        global calibration_in_progress
        calibration_in_progress = True
        command = f"Calibrate_{scale_num}" 
        
        message = f"Place known mass on scale {scale_num} ..."
        response = messagebox.askokcancel("Calibration", message)
        
        if response:
            ser.write(command.encode())
            print(command)
        else:
            print("Calibration canceled")
    else:
        print("Invalid scale number")


# Function to read values from the serial port and update the textboxes
def read_serial_values():
    if ser.in_waiting > 0:
        line = ser.readline().decode().strip()
        print("line: ", line)
        values = line.split(SCALES_VALUES_SEPARATOR)
        return values
    return None

# Run the Tkinter event loop
window.mainloop()
