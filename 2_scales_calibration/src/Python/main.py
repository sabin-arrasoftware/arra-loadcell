import tkinter as tk
from display import Display

if __name__ == "__main__":
    window = tk.Tk()
    window.title("Scale Display")
    app = Display(window)
    window.mainloop()


