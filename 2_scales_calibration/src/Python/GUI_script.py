import tkinter as tk
from scale_display_app import ScaleDisplayApp

if __name__ == "__main__":
    window = tk.Tk()
    window.title("Scale Display")
    app = ScaleDisplayApp(window)
    window.mainloop()


