from Command import Command

class PrepareLaunch(Command):
    def __init__(self, stateRequested):
        self.message = "Prepare Launch"
        self.stateRequested = stateRequested
    def execute(self):
        if (Logic.checkState() == safeToApproach):
            Logic.sendCommand()