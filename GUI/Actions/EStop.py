from Command import Command

class EStop(Command):
    def __init__(self, receiver):
        self.message = "estop"
        self.receiver = receiver
    def execute(self):
        self.receiver.printmessage(self.message)

