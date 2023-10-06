import tkinter as tk
from gui.app_window import ArduinoApp

if __name__ == "__main__":
    root = tk.Tk()
    app = ArduinoApp(root)
    root.mainloop()
