class Command:
    def __init__(self, message):
        self.message = message
    def execute(self):
        self.receiver.printmessage(self.message)
        pass