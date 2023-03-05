from enum import IntEnum
import struct
import socket
import select
from threading import Thread

class TelemetryReceiver:

    def __init__(self):


        self.packet_format = ">BB7iI"

        self.telemetry_model = None

    def setSocket(self, ip, port, socket):
        self.socket = socket
        self.ip = ip
        self.port = port
        print("port",port,"ip:",ip)
        self.socket.bind((self.ip, self.port))

    def setDataModel(self, model):
        self.telemetry_model = model

    def checkForPackets(self):
        while True:
            data, addr = self.socket.recvfrom(8192)
            print(data)
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