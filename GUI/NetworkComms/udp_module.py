import socket


class UDPModule:
    def __init__(self, address, port_receiver, port_transmitter, receiver, transmitter):
        self.TransmitterSocket = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.ReceiverSocket = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM)
        
        self.receiver = receiver
        self.cmdTransmit = transmitter
        self.cmdTransmit.setSocket(address, port_transmitter, self.TransmitterSocket)
        self.receiver.setSocket(address, port_receiver, self.ReceiverSocket)
        self.receiver.start()