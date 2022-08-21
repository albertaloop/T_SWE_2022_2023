from Command import Command

class Crawl(Command):
    def __init__(self, receiver):
        self.receiver = receiver
        self.message = "Crawl"
    def execute(self):
        self.receiver.printmessage(self.message)
        pass