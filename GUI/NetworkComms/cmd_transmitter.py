import socket
import sys
from threading import Thread

class CmdTransmitter:

    def setSocket(self, ip, port, socket):
        self.commandSocket = socket
        self.ip = ip
        self.port = port
        self.last_message = None
        self.packetModel = None

    def setModel(self, packetModel):
        self.packetModel = packetModel

    def sendCommand(self, message):
        # print(message)
        # bytesToSend = str.encode(message)
        self.commandSocket.sendto(message, (self.ip, self.port))
        # packetModel.update(message)
        self.last_message = message

    def recvAck(self, timeout, ack_msg):
        self.commandSocket.settimeout(timeout)
        try:
            data = self.commandSocket.recvfrom(2)
        except socket.timeout as e:
            print(e)
            return False
        except socket.error as e:
            print(e)
            sys.exit(1)
        else:
            if data[0][0] is ack_msg[0] and data[0][1] is ack_msg[1]:
                # packetModel.update(self.last_message, "acked")
                self.commandSocket.settimeout(0)
                return True
            else:
                print(f"Invalid message response {message=}, expected ")
                return False


