from Command import Command

class Launch(Command):
    def __init__(self, stateRequested):
        self.message = "Launch"
        self.stateRequested = stateRequested
    def execute(self):
        pass