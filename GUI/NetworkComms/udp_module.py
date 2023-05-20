import socket


class UDPModule:
    def __init__(self, gui_addr, remote_addr, port_receiver, port_transmitter, receiver, transmitter):
        self.TransmitterSocket = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.ReceiverSocket = socket.socket(
            family=socket.AF_INET, type=socket.SOCK_DGRAM)
        
        self.receiver = receiver
        self.cmdTransmit = transmitter
        self.cmdTransmit.setSocket(remote_addr, port_transmitter, self.TransmitterSocket)
        self.receiver.setSocket(gui_addr, port_receiver, self.ReceiverSocket)
        self.receiver.start()