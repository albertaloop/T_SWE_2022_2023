import socket
import struct
from enum import IntEnum
import sys

sock = None
server = None

class CommandType(IntEnum):
    Start = 0
    Stop = 1

def init(ip_address, port):
    global server
    global sock
    server = (ip_address, port)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def start():
    packet = struct.pack(">BB7iI", CommandType.Start)
    sock.sendto(packet, server)

def stop():
    packet = struct.pack(">BB7iI", CommandType.Stop)
    sock.sendto(packet, server)