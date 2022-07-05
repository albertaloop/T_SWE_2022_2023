import socket
import struct
from enum import IntEnum


class Command(IntEnum):
    Start = 0
    Stop = 1


class CommandManager:
    def __init__(self, ip_address, port):
        self.server = (ip_address, port)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def start(self):
        packet = struct.pack(">B", Command.Start)
        self.sock.sendto(packet, self.server)

    def stop(self):
        packet = struct.pack(">B", Command.Stop)
        self.sock.sendto(packet, self.server)
