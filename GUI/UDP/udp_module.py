import socket


class UDPModule:
    def __init__(self, address, port, receiver, transmitter):
        self.UDPSocket = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.receiver = receiver
        self.transmitter = transmitter
        # self.UDPSocket.bind(address, port)
        self.transmitter.setSocket(address, port, self.UDPSocket)
        self.receiver.setSocket(address, port, self.UDPSocket)
        self.receiver.start()
