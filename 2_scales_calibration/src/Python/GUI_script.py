import random
import tkinter as tk
from datetime import datetime

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
    1: {"values": scale1_values, "timestamps": scale1_timestamps, "is_displaying": False},
    2: {"values": scale2_values, "timestamps": scale2_timestamps, "is_displaying": False}
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
        #display_numbers(scale_num)

# Function to display the numbers for a scale
def display_numbers(scale_num):
    if scales[scale_num]["is_displaying"]:
        scale_values = scales[scale_num]["values"]
        scale_timestamps = scales[scale_num]["timestamps"]
        scale_checkbox_var = scales[scale_num]["checkbox"]
        number = format(random.uniform(0, 10) if scale_num == 1 else random.uniform(10, 20), ".2f")
        scale_values.append(number)
        scale_timestamps.append(datetime.now().strftime("%H:%M:%S.%f"))
        if len(scale_values) > 10:
            scale_values.pop(0)
            scale_timestamps.pop(0)
        scale_values_text = scales[scale_num]["text"]
        scale_values_text.delete(1.0, tk.END)
        for value, timestamp in zip(scale_values, scale_timestamps):
            line = f"{timestamp}: {value}\n"
            scale_values_text.insert(tk.END, line)            
        write_to_file(scale_num, line)
        global interval
        #window.after(interval, display_numbers, scale_num)  # Display numbers with the specified time interval
        # Schedule the next execution of display_numbers with the specified time interval
    window.after(interval, lambda: display_numbers(scale_num))

# Function to write the last line to a file for a scale
def write_to_file(scale_num, line):
    filename = f"scale{scale_num}_values.txt"
    with open(filename, "a") as file:
        file.write(line)  # Append the latest line to the file

# Create the checkboxes and value displays for each scale
for scale_num in scales:
    scale_values = scales[scale_num]["values"]
    scale_timestamps = scales[scale_num]["timestamps"]
    scale_is_displaying = scales[scale_num]["is_displaying"]

    scale_checkbox_var = tk.BooleanVar()
    scale_checkbox = tk.Checkbutton(
        window, text=f"Display Scale {scale_num}", variable=scale_checkbox_var, command=lambda num=scale_num: toggle_display(num)
    )
    scale_checkbox.grid(row=0, column=scale_num-1, padx=space_between_frames, pady=10)

    scale_frame = tk.Frame(window, width=display_frame_width)
    scale_frame.grid(row=1, column=scale_num-1, padx=space_between_frames, pady=10)

    scale_values_text = tk.Text(scale_frame, relief="solid", width=int(display_frame_width/8), height=10)
    scale_values_text.pack()

    scales[scale_num]["text"] = scale_values_text
    scales[scale_num]["checkbox"] = scale_checkbox_var
    scale_checkbox_var.set(scale_is_displaying)

    display_numbers(scale_num)

## Create the "Settings" section
settings_frame = tk.Frame(window)
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

# Variables to store the checkboxes' states
scale1_checkbox_var = tk.BooleanVar()
scale2_checkbox_var = tk.BooleanVar()
scale1_checkbox_var.set(scales[1]["is_displaying"])
scale2_checkbox_var.set(scales[2]["is_displaying"])

# Function to update the time interval
def update_time_interval():
    new_interval = time_interval.get()
    if new_interval.isdigit():
        new_interval = int(new_interval)
        if new_interval > 0:
            global interval
            interval = new_interval

# Configure the grid layout to adjust the column widths
window.grid_columnconfigure(0, weight=1)
window.grid_columnconfigure(1, weight=1)

# Run the Tkinter event loop
window.mainloop()
