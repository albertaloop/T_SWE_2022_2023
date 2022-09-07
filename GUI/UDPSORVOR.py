class Server():
    def __init__(self):
        import socket

        localIP = "127.0.0.1"

        localPort = 20001

        bufferSize = 1024

        msgFromServer = "Hello UDP Client"

        bytesToSend = str.encode(msgFromServer)

        # Create a datagram socket

        UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

        # Bind to address and ip

        UDPServerSocket.bind((localIP, localPort))

        print("UDP server up and listening")

        # Listen for incoming datagrams

        while (True):
            bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)

            message = bytesAddressPair[0]

            address = bytesAddressPair[1]

            clientMsg = "Message from Client:{}".format(message)
            clientIP = "Client IP Address:{}".format(address)

            print(clientMsg)
            print(clientIP)

            # Sending a reply to client

            UDPServerSocket.sendto(bytesToSend, address)

class Client(Server):
    def __init__(self):
        super().__init__()
        import socket

        msgFromClient = "Hello UDP Server"

        bytesToSend = str.encode(msgFromClient)

        serverAddressPort = ("127.0.0.1", 20001)

        bufferSize = 1024

        # Create a UDP socket at client side

        UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

        # Send to server using created UDP socket

        UDPClientSocket.sendto(bytesToSend, serverAddressPort)

        msgFromServer = UDPClientSocket.recvfrom(bufferSize)

        msg = "Message from Server {}".format(msgFromServer[0])

        print(msg)

r1= Server()
r2= Client()