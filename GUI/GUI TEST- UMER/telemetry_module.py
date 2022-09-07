from enum import IntEnum
import struct
import socket
import select
from threading import Thread


class TelemetryManager:
    def __init__(self, ip_address, port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind((ip_address, port))
        self.team_id = -1
        self.status = -1
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

        thread = Thread(target=self.run)
        thread.start()

    def get_team_id(self):
        return self.team_id

    def get_status(self):
        return self.status

    def get_acceleration(self):
        return self.acceleration

    def get_position(self):
        return self.position

    def get_velocity(self):
        return self.velocity

    def get_battery_voltage(self):
        return self.battery_voltage

    def get_battery_current(self):
        return self.battery_current

    def get_battery_temperature(self):
        return self.battery_temperature

    def get_pod_temperature(self):
        return self.pod_temperature

    def get_stripe_count(self):
        return self.stripe_count

    def get_highest_velocity(self):
        return self.highest_velocity

    def handle_packet(self, data):
        if len(data) != struct.calcsize(self.packet_format):
            return

        (
            self.team_id,
            self.status,
            self.acceleration,
            self.position,
            self.velocity,
            self.battery_voltage,
            self.battery_current,
            self.battery_temperature,
            self.pod_temperature,
            self.stripe_count,
        ) = struct.unpack(self.packet_format, data)
        self.highest_velocity = max(self.highest_velocity, self.velocity)

    def run(self):
        while True:
            ready_sockets, _, _ = select.select([self.socket], [], [], 0.1)

            if len(ready_sockets) > 0:
                received_data = ready_sockets[0].recv(
                    struct.calcsize(self.packet_format) + 1
                )
                self.handle_packet(received_data)
