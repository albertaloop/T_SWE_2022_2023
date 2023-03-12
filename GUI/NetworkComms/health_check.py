class HealthCheckReq():
    def setSocket(self, ip, port, socket):
        self.healthCheckSocket = socket
        self.ip = ip
        self.port = port
        self.packetModel = None
        self.last_message = None

    def setModel(self, packetModel):
        self.packetModel = packetModel

    def sendRequest(self, message):
        bytesToSend = str.encode(message)
        self.healthCheck.sendto(bytesToSend, (self.ip, self.port))
        packetModel.update(message)
        self.last_message = message

    def recvAck(self, timeout, ack_msg):
        sucess = self.healthCheckSocket.recv_from()
        if success:
            packetModel.update(self.last_message, "acked")
        return success

