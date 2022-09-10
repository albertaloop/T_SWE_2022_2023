class Command:
    def __init__(self, message):
        self.message = message
    def execute(self):
        pass

class Crawl(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Crawl"
    def execute(self):
        self.receiver.printMessage(self.message)
        pass

class EStop(Command):
    def __init__(self, receiver):
        self.message = "estop"
        self.receiver = receiver
    def execute(self):
        self.receiver.printMessage(self.message)

class HealthCheck(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Health Check"
    def execute(self):
        self.receiver.printMessage(self.message)
        pass

class Launch(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Launch"
        self.stateRequested = "launching"
    def execute(self):
        self.receiver.printMessage(self.message)
        pass