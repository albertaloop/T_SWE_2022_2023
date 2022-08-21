from Command import Command

class Launch(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Launch"
        self.stateRequested = "launching"
    def execute(self):
        self.receiver.printmessage(self.message)
        pass