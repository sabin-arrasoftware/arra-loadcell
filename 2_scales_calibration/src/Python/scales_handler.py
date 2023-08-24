import tkinter as tk
from tkinter import ttk
from scale import Scale

NUMBER_OF_SCALES = 2
SPACE_BETWEEN_FRAMES = 10
WIDTH = 45

class ScalesHandler:
    def __init__(self, parent):
        self.parent = parent
        self.scales = []    
    
    def handle_scale_frames(self):
        for scale_num in range(1, NUMBER_OF_SCALES + 1):
            scale_frame = self.create_scale_frame(scale_num)
            self.create_scale(scale_frame) 
    
    def create_scale_frame(self, scale_num):
        # previously in display.py: width=self.get_display_frame_width()
        scale_frame = ttk.Frame(self.parent, width=WIDTH)        
        scale_frame.grid(row=1, column=scale_num - 1, padx=SPACE_BETWEEN_FRAMES, pady=10)
        return scale_frame
    
    def create_scale(self, scale_frame):
        scale = Scale(scale_frame)
        self.scales.append(scale) 

    def handle_display_values(self, values):
        if values:
            for scale_num, scale in enumerate(self.scales):
                scale.display_value(values[scale_num].split(":")[1].strip())