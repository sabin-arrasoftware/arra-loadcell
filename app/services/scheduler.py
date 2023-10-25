import threading
import time

class Scheduler:
    def __init__(self):
        self.tasks = {}
        self.stop_events = {}

    def schedule(self, task_name, target_function, interval=0, delay=0):
        """
        Schedule a task to run periodically or after a delay.

        :param task_name: Unique name for the task.
        :param target_function: The function to execute.
        :param args: Arguments to pass to the function.
        :param interval: Time in milliseconds between task executions (0 for one-time execution).
        :param delay: Time in milliseconds before the first execution.
        """
        if task_name in self.tasks:
            raise ValueError(f"Task with name '{task_name}' already exists.")

        stop_event = threading.Event()
        self.stop_events[task_name] = stop_event
        
        task = threading.Thread(
            target=self._task_wrapper,
            args=(task_name, target_function, float(interval/1000), float(delay/1000), stop_event),  # Convert to seconds
            daemon=True,
        )

        self.tasks[task_name] = task
        task.start()

    def stop(self, task_name):
        """
        Stop a scheduled task and remove it from the scheduler.

        :param task_name: Name of the task to stop.
        """
        if task_name in self.tasks:
            self.stop_events[task_name].set()  # Signal the thread to stop
            del self.tasks[task_name]
            del self.stop_events[task_name]

    def _task_wrapper(self, task_name, target_function, interval, delay, stop_event):
        if delay > 0:
            time.sleep(delay)

        while task_name in self.tasks:
            target_function()

            if interval > 0 and not stop_event.is_set():
                time.sleep(interval)

            if stop_event.is_set():  # Check if we should stop the thread
                break
