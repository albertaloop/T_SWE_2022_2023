from enum import IntEnum
import struct
import socket
import select
from threading import Thread

class TelemetryReciever:

    def __init__(self, host, port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind((host, port))
        self.team_id = 0
        self.status = 0
        self.acceleration = 0
        self.position = 0
        self.velocity = 0
        self.battery_voltage = 0
        self.battery_current = 0
        self.battery_temperature = 0
        self.pod_temperature = 0
        self.stripe_count = 0
        self.highest_velocity = 0
        self.packet_format = ">BB7iI"

    def checkForPackets(self):
        while True:
            data, addr = self.socket.recvfrom(8192)
            self.handlePacket(data)
    
    def handlePacket(self, data):
        self.team_id, self.status, self.acceleration, self.position, self.velocity, self.battery_voltage, self.battery_current, 
        self.battery_temperature, self.pod_temperature, self.stripe_count = struct.unpack(self.packet_format, data)
