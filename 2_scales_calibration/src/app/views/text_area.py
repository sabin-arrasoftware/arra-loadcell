import tkinter as tk

class TextArea(tk.Text):
    def __init__(self, root):
        super().__init__(root, height=10, width=40)
        self.pack(pady=20)
