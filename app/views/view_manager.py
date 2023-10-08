from tkinter import messagebox
from views.text_area import TextArea
from views.buttons import ButtonsFrame
from views.settings import Settings

class ViewManager:
    """
    Manages the main GUI components of the Arduino Communication App.
    Provides methods to interact with the text area, buttons, and settings components.
    """
    
    def __init__(self, root, callback_manager):
        """
        Initialize the ViewManager with the main GUI components.

        :param root: The root tkinter window.
        :param callback_manager: The callback manager to handle user interactions.
        """
        self.root = root
        self.root.title("Arduino Communication App")
        self.text_area = TextArea(root)
        self.buttons = ButtonsFrame(root, callback_manager)
        self.settings = Settings(root, callback_manager)

    def clear_text_area(self):
        """
        Clear the content of the text area.
        """
        self.text_area.clear()
    
    def insert_text_area(self, txt):
        """
        Insert text into the text area.

        :param txt: The text to insert.
        """
        self.text_area.insert(txt)

    def get_setting_val(self, setting_name):
        """
        Get the value of a specific setting.

        :param setting_name: The name of the setting.
        :return: The value of the setting.
        """
        return self.settings.get(setting_name)

    def get_toggle_val(self):
        """
        Get the value of the toggle button.

        :return: The state of the toggle button (True/False).
        """
        return self.buttons.get_toggle_val()

    def ask(self, title, text):
        """
        Display a confirmation dialog with a specified title and text.

        :param title: The title of the dialog.
        :param text: The text content of the dialog.
        :return: True if the user clicks "OK", False otherwise.
        """
        return messagebox.askokcancel(title, text)

    def inform(self, title, text):
        """
        Display an information dialog with a specified title and text.

        :param title: The title of the dialog.
        :param text: The text content of the dialog.
        """
        messagebox.showinfo(title, text)
