import tkinter as tk
from controllers.app_controller import ArduinoAppController
import logging

if __name__ == "__main__":
    root = tk.Tk()
    app = ArduinoAppController(root, 'config/settings.ini') 
    root.protocol("WM_DELETE_WINDOW", app.on_closing)  # Bind the on_closing method to the window's close event
    root.mainloop()