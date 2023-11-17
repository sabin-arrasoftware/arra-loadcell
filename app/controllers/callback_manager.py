from enum import Enum

class Events(Enum):
    """
    Enumerates the types of events that can trigger callbacks.
    """
    TOGGLE_START = "toggle_start"
    CLEAR_TEXT = "clear_text"
    CALIBRATE = "calibrate"
    SETUP = "setup"


class CallbackManager:
    """
    Manages the registration and execution of callbacks based on specific events.
    """
    def __init__(self):
        """
        Initializes a new instance of the CallbackManager.
        """
        self.callbacks = {}

    def register(self, event: Events, callback):
        """
        Registers a callback to be executed when a specific event occurs.

        Args:
            event (Events): The event that will trigger the callback.
            callback (function): The function to be executed when the event occurs.
        """
        self.callbacks[event] = callback

    def execute(self, event: Events):
        """
        Executes the callback associated with the specified event.

        Args:
            event (Events): The event for which to execute the associated callback.
        """
        if event in self.callbacks:
            self.callbacks[event]()
