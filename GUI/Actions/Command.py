class Command:
    def __init__(self, message, receiver):
        self.message = message
    def execute(self):
        self.receiver.printmessage(self.message)
        pass

class Crawl(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Crawl"

class EStop(Command):
    def __init__(self, receiver):
        self.message = "estop"
        self.receiver = receiver


class Launch(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Launch"

class PrepareLaunch(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Prepare Launch"
