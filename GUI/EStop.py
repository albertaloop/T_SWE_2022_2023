from Command import Command

class EStop(Command):
    def __init__(self, receiver, stateRequested):
        self.message = "estop"
        self.receiver = receiver
        self.stateRequested = stateRequested
    def execute(self):
        self.receiver.sendCommand(self.message)

