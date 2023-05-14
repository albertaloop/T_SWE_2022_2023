import socket
from argparse import ArgumentParser

if __name__ == "__main__":


    parser = ArgumentParser(
        description="Mock the pod electronics"
    )
    parser.add_argument("--t", default="c", help="The behavior included in the mock. c=commands, t=telemetry, h=health-check")


    udp_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

    
    udp_socket.bind(("127.0.0.1", 3000))


    while True:
        data = udp_socket.recvfrom(1024)
        print(data)

