class UDPModule():
    def __init__(sel):
        import socket
        udpsocket= socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

class Server(UDPModule):
    def __init__(self, t):
        super().__init__(udpsocket)

        self.localIP = "127.0.0.1"

        self.localPort = 20001

        self.bufferSize = 1024

        self.msgFromServer = "Hello UDP Client"

        self.bytesToSend = str.encode(self.msgFromServer)

        # Bind to address and ip

        udpsocket.bind((self.localIP, self.localPort))

        print("UDP server up and listening")

        # Listen for incoming datagrams

        while (True):
            self.bytesAddressPair = udpsocket.recvfrom(self.bufferSize)

            self.message = self.bytesAddressPair[0]

            self.address = self.bytesAddressPair[1]

            self.clientMsg = "Message from Client:{}".format(self.message)
            self.clientIP = "Client IP Address:{}".format(self.address)

            print(self.clientMsg)
            print(self.clientIP)

            # Sending a reply to client

            udpsocket.sendto(self.bytesToSend, self.address)

class Client(UDPModule):
    def __init__(self, udpsocket):
        super().__init__(udpsocket)
        msgFromClient = "Hello UDP Server"

        bytesToSend = str.encode(msgFromClient)

        serverAddressPort = ("127.0.0.1", 20001)

        bufferSize = 1024

        # Send to server using created UDP socket

        udpsocket.sendto(bytesToSend, serverAddressPort)

        msgFromServer = udpsocket.recvfrom(bufferSize)

        msg = "Message from Server {}".format(msgFromServer[0])

        print(msg)



test1= Server()
test= Client()