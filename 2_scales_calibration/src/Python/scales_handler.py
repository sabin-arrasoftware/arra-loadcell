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
    
    def handle_scale_frames(self):
        for scale_num in range(1, NUMBER_OF_SCALES + 1):
            scale_frame = self.create_scale_frame(scale_num)
            print("Created scale frame ", scale_num)
            self.create_scale(scale_frame) 
            print("Created scale")
            self.create_scale_checkbox(scale_num)
        
    def create_scale_checkbox(self, scale_num):
        scale_checkbox = tk.Checkbutton(
            self.parent, text=f"Display Scale {scale_num}", variable=self.scales[scale_num - 1].is_displaying.get(),
            command=self.scales[scale_num - 1].toggle_display()
        )
        scale_checkbox.grid(row=0, column=scale_num - 1, padx=SPACE_BETWEEN_FRAMES, pady=10)
        self.scales[scale_num - 1].is_displaying.set(False)
  
    
    def create_scale_frame(self, scale_num):
        width_ = self.display_instance.get_display_frame_width()
        print("width_ = ", width_)
        scale_frame = ttk.Frame(self.parent, width=width_)       
        scale_frame.grid(row=1, column=scale_num - 1, padx=SPACE_BETWEEN_FRAMES, pady=10)
        return scale_frame
    
    def create_scale(self, scale_frame):
        scale = Scale(scale_frame)
        self.scales.append(scale) 
        scale.create_text_widget()
    
    

    def handle_display_values(self, values):
        if values:
            for scale_num, scale in enumerate(self.scales):
                scale.display_value(values[scale_num].split(":")[1].strip())