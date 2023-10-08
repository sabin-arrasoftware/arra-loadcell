import tkinter as tk
from controllers.app_controller import ArduinoAppController

if __name__ == "__main__":
    root = tk.Tk()
    app = ArduinoAppController(root) 
    root.mainloop()