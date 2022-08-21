from Command import Command

class PrepareLaunch(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Prepare Launch"
    def execute(self):
        #if (Logic.checkState() == safeToApproach):
         #   Logic.sendCommand()
        self.receiver.printmessage(self.message)