from scale import Scale

class ScalesHandler:
    def __init__(self, parent, scale_num, display):
        self.scale_num = scale_num
        self.scale = Scale(parent)
        self.display = display

    def is_displaying(self):
        return self.scale.is_displaying.get()

    def handle_display_values(self, values):
        if values:
            self.scale.display_value(values[self.scale_num - 1].split(":")[1].strip())