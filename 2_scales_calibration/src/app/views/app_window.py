from .text_area import TextArea
from .buttons import StartButton, ClearButton, CalibrateButton
from .settings import SettingsFrame

class ArduinoApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Arduino Communication App")

        self.text_area = TextArea(root)
        self.start_button = StartButton(root, self.text_area)
        self.clear_button = ClearButton(root, self.text_area)
        self.calibrate_button = CalibrateButton(root, self.text_area)
        self.settings_frame = SettingsFrame(root, self.text_area)
