from Commands import EStop

class MockInvoker:
    def __init__(self):
        pass
    def setCommand(self, command) :
        self.command = command
    def executeCommand(self):
        self.command.execute()

class MockReceiver:
    def __init__(self):
        pass
    def printMessage(self, message):
        self.message = message
        print(self.message)

if __name__ == "__main__":
    mockInvoker = MockInvoker()
    mockReceiver = MockReceiver()
    estop = EStop(mockReceiver)
    mockInvoker.setCommand(estop)
    mockInvoker.executeCommand()