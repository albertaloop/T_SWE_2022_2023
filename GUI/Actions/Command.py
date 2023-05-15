class Command:
    def __init__(self, message, receiver, ack_msg, cmd_lock, command_requested):
        self.message = message
        self.ack_msg = ack_msg
        self.receiver = receiver
        self.command_requested = command_requested
        self.cmd_lock = cmd_lock
    def execute(self):
        cmd_lock.acquire()
        print(self.message)
        while True:
            self.receiver.cmdTransmit.sendCommand(self.message)
            status = self.receiver.cmdTransmit.recvAck(10, self.ack_msg)
            if (status):
                break
        command_requested = "none"
        cmd_lock.release()        


class Crawl(Command):
    def __init__(self, receiver, cmd_lock, command_requested):
        self.receiver = receiver
        self.command_requested = command_requested
        self.cmd_lock = cmd_lock
        msg = [0xC0, 0xC6]
        amsg = [0xA0, 0xA6]
        self.message = bytes(msg)
        self.ack_msg = bytes(amsg)

class EStop(Command):
    def __init__(self, receiver, cmd_lock, command_requested):
        self.receiver = receiver
        self.command_requested = command_requested
        self.cmd_lock = cmd_lock
        msg = [0xC0, 0xC0]
        amsg = [0xA0, 0xA0]
        self.message = bytes(msg)
        self.ack_msg = bytes(amsg)

class Launch(Command):
    def __init__(self, receiver, cmd_lock, command_requested):
        self.receiver = receiver
        self.command_requested = command_requested
        self.cmd_lock = cmd_lock
        msg = [0xC0, 0xC4]
        amsg = [0xA0, 0xA4]
        self.message = bytes(msg)
        self.ack_msg = bytes(amsg)

class PrepareLaunch(Command):
    def __init__(self, receiver, cmd_lock, command_requested):
        self.receiver = receiver
        self.command_requested = command_requested
        self.cmd_lock = cmd_lock
        msg = [0xC0, 0xC2]
        amsg = [0xA0, 0xA2]
        self.message = bytes(msg)
        self.ack_msg = bytes(amsg)

