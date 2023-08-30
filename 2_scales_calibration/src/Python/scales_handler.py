import tkinter as tk
from tkinter import ttk
from scale import Scale

NUMBER_OF_SCALES = 2
SPACE_BETWEEN_FRAMES = 10
WIDTH = 45

class ScalesHandler:
    def __init__(self, parent, display_instance):
        self.parent = parent
        self.display_instance = display_instance
        self.scales = []   
        self.scale_frames = [] 
    
    def handle_scale_frames(self):
        for scale_num in range(1, NUMBER_OF_SCALES + 1):
            scale_frame = self.create_scale_frame(scale_num)
            print("Created scale frame ", scale_num)            
            scale = self.create_scale(scale_frame, scale_num) 
            print("Created scale")
            self.create_checkbox_and_buttons(scale)          
    
    
    def create_scale_frame(self, scale_num):
        width_ = self.display_instance.get_display_frame_width()
        print("width_ = ", width_)
        scale_frame = ttk.Frame(self.parent, width=width_)       
        scale_frame.grid(row=1, column=scale_num - 1, padx=SPACE_BETWEEN_FRAMES, pady=10)
        self.scale_frames.append(scale_frame)
        return scale_frame
    
    def create_scale(self, scale_frame, scale_num):
        scale = Scale(scale_frame, scale_num)
        self.scales.append(scale) 
        return scale
    
    def create_checkbox_and_buttons(self, scale):
        self.create_scale_checkbox(scale)
        self.create_clear_button(scale)
        self.create_calibrate_button(scale)  
    

    def create_scale_checkbox(self, scale):
        index = scale.scale_num - 1
        scale_checkbox = tk.Checkbutton(
            self.scale_frames[index], text=f"Display Scale {scale.scale_num}", variable=self.scales[index].is_displaying.get,
            command=self.scales[index].toggle_display
        )
        scale_checkbox.pack(side="top", padx=SPACE_BETWEEN_FRAMES, pady=10)
        self.scales[index].is_displaying.set(False)
    
    def create_clear_button(self, scale):
        index = scale.scale_num - 1 
        clear_button = tk.Button(self.scale_frames[index], text="Clear", command=self.scales[index].clear_values)
        clear_button.pack()

    def create_calibrate_button(self, scale):
        index = scale.scale_num - 1
        calibrate_button = tk.Button(
            self.scale_frames[index], text="Calibrate", command=self.scales[index].send_calibration_command
        )
        calibrate_button.pack()

    def handle_display_values(self, values):
        if values:
            for scale_num, scale in enumerate(self.scales):
                if scale.is_displaying.get():
                    scale.display_value(values[scale_num].split(":")[1].strip())