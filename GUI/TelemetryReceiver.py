from enum import IntEnum
import struct
import socket
import select
from threading import Thread

class TelemetryReceiver:

    def __init__(self):
        # self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # self.socket.bind((host, 8000))
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

        self.telemetry_model = None

    def setSocket(self, ip, port, socket):
        self.socket = socket
        self.ip = ip
        self.port = port
        self.socket.bind((self.ip, self.port))

    def setDataModel(self, model):
        self.telemetry_model = model

    def checkForPackets(self):
        while True:
            data, addr = self.socket.recvfrom(8192)
            # print(data)
            self.handlePacket(data)
    
    def handlePacket(self, data):
        rdata = {}
        (
            rdata["team_id"],
            rdata["status"],
            rdata["acceleration"],
            rdata["position"],
            rdata["velocity"],
            rdata["battery_voltage"],
            rdata["battery_current"],
            rdata["battery_temperature"],
            rdata["pod_temperature"],
            rdata["stripe_count"],
        ) = struct.unpack(self.packet_format, data)
        
        self.telemetry_model.update(rdata)
    def start(self):
        print("We are started!!")
        thread = Thread(target=self.checkForPackets)
        thread.start()