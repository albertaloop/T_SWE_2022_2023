import threading
from threading import Thread

class UDPModule():
    import socket
    global udpsocket
    udpsocket= socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    def __init__(self):
        True



class ServerThread (Thread):
    def __init__(self):
        Thread.__init__(self)
    def run(self):
        Server().trysend()

class ClientThread (Thread):
    def __init__(self):
        Thread.__init__(self)
    def run(self):
        Client().tryreceive()

class Server():
    def __init__(self):
        True
    def trysend(self):
        localIP = "127.0.0.1"

        localPort = 20001

        bufferSize = 1024

        msgFromServer = "Hello UDP Client"

        bytesToSend = str.encode(msgFromServer)

        # Bind to address and ip

        udpsocket.bind((localIP, localPort))

        print("UDP server up and listening")

        # Listen for incoming datagrams

        while (True):
            bytesAddressPair = udpsocket.recvfrom(bufferSize)

            message = bytesAddressPair[0]

            address = bytesAddressPair[1]

            clientMsg = "Message from Client:{}".format(message)
            clientIP = "Client IP Address:{}".format(address)

            print(clientMsg)
            print(clientIP)

            # Sending a reply to client

            udpsocket.sendto(bytesToSend, address)

class Client():
    def __init__(self):
        True
    def tryreceive(self):
        msgFromClient = "Hello UDP Server"

        bytesToSend = str.encode(msgFromClient)

        serverAddressPort = ("127.0.0.1", 20001)

        bufferSize = 1024

        # Create a UDP socket at client side

        # Send to server using created UDP socket

        udpsocket.sendto(bytesToSend, serverAddressPort)

        msgFromServer = udpsocket.recvfrom(bufferSize)

        msg = "Message from Server {}".format(msgFromServer[0])

        print(msg)



# New Threads
server = ServerThread()
client = ClientThread()

# Start new Threads
server.start()
client.start()

