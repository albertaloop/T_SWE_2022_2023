import socket
from argparse import ArgumentParser

if __name__ == "__main__":


    parser = ArgumentParser(
        description="Mock the pod electronics"
    )
    parser.add_argument("--t", default="c", help="The behavior included in the mock. c=commands, t=telemetry, h=health-check")


    udp_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

    
    udp_socket.bind(("127.0.0.1", 3000))
    reply = [None] * 2
    reply[0] = 0xA0
    while True:
        data = udp_socket.recvfrom(1024)
        print(data)
        if data[0][0] == 0xC0:
            valid = True
            if data[0][1] == 0xC0:
                reply[1] = 0xA0
            elif data[0][1] == 0xC2:
                reply[1] = 0xA2
            elif data[0][1] == 0xC4:
                reply[1] = 0xA4
            elif data[0][1] == 0xC6:
                reply[1] = 0xA6
            else:
                valid = False
        else:
            valid = False
        if valid:
            msg = bytes(reply)
            udp_socket.sendto(msg, data[1])
            