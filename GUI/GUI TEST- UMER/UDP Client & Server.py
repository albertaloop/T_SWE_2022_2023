class UDPModule():
    def __init__(self, UDPsocket):
        import socket
        self.UDPsocket= socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

class Client(UDPModule):
    def __init__(self, UDPsocket):
        self.msgFromClient = "Hello UDP Server"

        self.bytesToSend = str.encode(self.msgFromClient)

        self.serverAddressPort = ("127.0.0.1", 20001)

        self.bufferSize = 1024
        # Send to server using created UDP socket
        UDPModule.UDPsocket.sendto(self.bytesToSend, self.serverAddressPort)
        self.msgFromServer = UDPModule.UDPsocket.recvfrom(self.bufferSize)

        self.msg = "Message from Server {}".format(self.msgFromServer[0])

        print(self.msg)

class Server(UDPModule):
    def __init__(self, UDPsocket):
        self.localIP = "127.0.0.1"

        self.localPort = 20001

        self.bufferSize = 1024

        self.msgFromServer = "Hello UDP Client"

        self.bytesToSend = str.encode(self.msgFromServer)
        # Bind to address and ip
        UDPModule.UDPsocket.bind((self.localIP, self.localPort))

        print("UDP server up and listening")

        # Listen for incoming datagrams

        while (True):
            self.bytesAddressPair = UDPModule.UDPsocket.recvfrom(self.bufferSize)

            self.message = self.bytesAddressPair[0]

            self.address = self.bytesAddressPair[1]

            self.clientMsg = "Message from Client:{}".format(self.message)
            self.clientIP = "Client IP Address:{}".format(self.address)

            print(clientMsg)
            print(clientIP)

            # Sending a reply to client

            UDPModule.UDPsocket.sendto(self.bytesToSend, self.address)
