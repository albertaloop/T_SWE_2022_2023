import socket
from threading import Thread


class TelemetryReceiver(Thread):

    def __init__(self, bufferSize):
        self.bufferSize = bufferSize
        Thread.__init__(self)

    def setSocket(self, ip, port, socket):
        self.receiveSocket = socket
        self.ip = ip
        self.port = port
        self.receiveSocket.bind((self.ip, self.port))

    def run(self):
        while (1):
            self.checkForPackets()
        # handlePackets()

    def checkForPackets(self):

        bytesAddressPair = self.receiveSocket.recvfrom(self.bufferSize)
        message = bytesAddressPair[0]

        address = bytesAddressPair[1]

        clientMsg = "Message from Client:{}".format(message)
        clientIP = "Client IP Address:{}".format(address)

        print(clientMsg)
        print(clientIP)

    # def handlePackets():
