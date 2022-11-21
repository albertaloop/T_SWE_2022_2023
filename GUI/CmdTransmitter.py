class CmdTransmitter:

    def setSocket(self, ip, port, socket):
        self.commandSocket = socket
        self.ip = ip
        self.port = port

    def sendPacket(self, message):
        bytesToSend = str.encode(message)
        self.commandSocket.sendto(bytesToSend, (self.ip, self.port))