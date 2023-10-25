import tkinter as tk
from controllers.app_controller import ArduinoAppController
import logging

if __name__ == "__main__":
    # Configure logging
    logging.basicConfig(
        level=logging.DEBUG,  # Change this to the desired log level
        format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
        filename='app.log',  # Change the filename to your preferred log file
        filemode='w'  # 'w' for write mode, 'a' for append mode
    )

    root = tk.Tk()
    app = ArduinoAppController(root) 
    root.mainloop()