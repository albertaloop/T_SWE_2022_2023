from enum import IntEnum
import struct
import socket
import select
from threading import Thread

class TelemetryReceiver:

    def __init__(self):
        self.packet_format = ">BB7i"
        self.telemetry_model = None
        self.packetModel = None

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
            data, addr = self.socket.recvfrom(30)
            print(data)
            self.handlePacket(data)
    
    def handlePacket(self, data):
        # rdata = []
        # rdata.append(struct.unpack('B', data[0:1])[0]) # team_id (uint8_t)
        # rdata.append(struct.unpack('B', data[1:2])[0]) # status (uint8_t)
        # rdata.append(struct.unpack('i', data[2:6])[0]) # accel (int32_t)
        # rdata.append(struct.unpack('i', data[6:10])[0]) # position (int32_t)
        # rdata.append(struct.unpack('i', data[10:14])[0]) # velocity (int32_t)
        # rdata.append(struct.unpack('i', data[14:18])[0]) # batt1_v (int32_t)
        # rdata.append(struct.unpack('i', data[18:22])[0]) # batt1_c (int32_t)
        # rdata.append(struct.unpack('i', data[22:26])[0]) # batt1_temp (int32_t)
        # rdata.append(struct.unpack('i', data[26:30])[0]) # pod_temp (int32_t)






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
        ) = struct.unpack(self.packet_format, data)
        
        self.telemetry_model.update(rdata)
    def start(self):
        print("We are started!!")
        thread = Thread(target=self.checkForPackets)
        thread.start()