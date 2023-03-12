class CmdTransmitter:

    def setSocket(self, ip, port, socket):
        self.commandSocket = socket
        self.ip = ip
        self.port = port
        self.last_message = None
        self.packetModel = None

    def setModel(self, packetModel):
        self.packetModel = packetModel

    def sendCommand(self, message):
        print(message)
        bytesToSend = str.encode(message)
        self.commandSocket.sendto(bytesToSend, (self.ip, self.port))
        packetModel.update(message)
        self.last_message = message

    def recvAck(self, timeout, ack_msg):
        sucess = self.commandSocket.recv_from()
        if success:
            packetModel.update(self.last_message, "acked")
        return success
