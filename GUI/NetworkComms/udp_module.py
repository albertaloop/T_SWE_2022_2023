import socket


class UDPModule:
    def __init__(self, address, port_transmitter, port_receiver, receiver, transmitter):
        self.TransmitterSocket = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.ReceiverSocket = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM)
        
        self.receiver = receiver
        self.transmitter = transmitter
        # self.UDPSocket.bind(address, port)
        self.transmitter.setSocket(address, port_transmitter, self.TransmitterSocket)
        self.receiver.setSocket(address, port_receiver, self.ReceiverSocket)
        self.receiver.start()
        # self.transmitter.start()