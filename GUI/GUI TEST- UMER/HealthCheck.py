from Command import Command

class HealthCheck(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Health Check"
    def execute(self):
        self.receiver.printmessage(self.message)
        pass
