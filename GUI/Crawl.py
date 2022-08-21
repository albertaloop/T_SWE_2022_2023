from Command import Command

class Crawl(Command):
    def __init__(self, stateRequested):
        self.message = "Crawl"
        self.stateRequested = stateRequested
    def execute(self):
        pass