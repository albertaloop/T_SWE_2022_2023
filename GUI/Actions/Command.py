class Command:
    def __init__(self, message, receiver):
        self.message = message
        self.receiver = receiver
    def execute(self):
        print(self.message)
        while True:
            self.receiver.cmdTransmit.sendCommand(self.message)
            status = self.receiver.cmdTransmit.recvAck(10, "cmd_ack")
            if (status):
                break
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
