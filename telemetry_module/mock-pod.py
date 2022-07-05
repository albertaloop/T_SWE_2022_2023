"""
@brief Hyperloop team mock pod program
@author Harmjan Treep <harmjan@boringcompany.com>
Subject to the existing rights of third parties, THE BORING COMPANY
is the owner of the copyright in this work and no portion thereof is
to be copied, reproduced or communicated to any person without
written permission.
"""
# What need to be done:
# battery voltage,
# battery current,
# battery temperature,
# pod temperature,
# and stripe count.

from argparse import ArgumentParser
from time import sleep
import math
import socket
import struct
import random
from enum import IntEnum
from time import time


class Status(IntEnum):
    Fault = 0
    SafeToApproach = 1
    ReadyToLaunch = 2
    Launching = 3
    Coasting = 4
    Braking = 5
    Crawling = 6


def get_position(seconds, run_length, tube_length):
    return (1 - 1 * math.cos(seconds * math.pi / run_length)) / 2.0 * tube_length


def get_velocity(seconds, run_length, tube_length):
    return (
        get_position(seconds, run_length, tube_length)
        - get_position(seconds - 0.1, run_length, tube_length)
    ) * 10


def get_acceleration(seconds, run_length, tube_length):
    return (
        get_velocity(seconds, run_length, tube_length)
        - get_velocity(seconds - 0.1, run_length, tube_length)
    ) * 10


# In general, battery voltage for hyperloop is around 36, voltage will increase as voltage increase


def get_battery_voltage(seconds, initial_temp):
    return 36.0 + get_battery_temperature(seconds, initial_temp) / 100.0


def get_battery_current(seconds, initial_temp, resistance):
    return get_battery_voltage(seconds, initial_temp) / resistance


# Highest battery temperature is around 150C


def get_battery_temperature(seconds, initial_temp):
    temperature = initial_temp + math.sqrt(seconds * 2)
    if temperature < 150.0:
        return temperature
    else:
        return 150.0


def get_pod_temperature(seconds, initial_pod_temp):
    return initial_pod_temp + math.sin(seconds / 10) / (seconds / 10)


def get_stripe_count(seconds, initial_stripe_count):
    prob = random.uniform(0, 1)
    if prob >= 0.5:
        return initial_stripe_count + math.sin(seconds) / seconds
    else:
        return initial_stripe_count - math.sin(seconds) / seconds


if __name__ == "__main__":
    parser = ArgumentParser(
        description="Mock the run of a pod to test the Hyperloop system"
    )
    parser.add_argument("--team_id", type=int, default=0, help="The team id to send")
    parser.add_argument(
        "--frequency", type=int, default=25, help="The frequency to send packets at"
    )
    parser.add_argument(
        "--server_ip", default="192.168.0.1", help="The ip to send the packets to"
    )
    parser.add_argument(
        "--server_port", type=int, default=3000, help="The UDP port to send packets to"
    )
    parser.add_argument(
        "--tube_length",
        type=int,
        default=125000,
        help="The length of the tube in centimeters",
    )

    args = parser.parse_args()

    if args.frequency < 10:
        print("Send frequency should be higher than 10Hz")
    if args.frequency > 50:
        print("Send frequency should be lower than 50Hz")

    team_id = args.team_id
    tube_length = args.tube_length
    run_length = 30
    # initial temperature is set to 20C, change this value if need
    initial_temp = 20
    # 500 ohms for 12V battery, change this value if need
    resistance = 500
    initial_pod_temp = 30
    initial_stripe_count = 3048

    wait_time = 1 / args.frequency
    server = (args.server_ip, args.server_port)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    position = 0
    velocity = 0
    acceleration = 0
    battery_temperature = 0.0
    pod_temperature = 0.0
    battery_voltage = 0.0
    battery_current = 0.0
    stripe_count = 0
    status = Status.SafeToApproach

    should_launch = True

    seconds = 0
    while True:
        start_time = time()

        # Run a simple test scenario where the pod first is in Safe to Approach
        # for 10 seconds, in Ready to Launch for 5 seconds after which the pod
        # goes into the Launching state where the position changes upto the tube
        # length. When the acceleration goes negative the pod status goes to Braking,
        # when the pod stops the status goes back to Safe to Approach. The position
        # is based on a quarter of the cosine curve, the velocity and acceleration
        # are calculated from the position function.
        if status == Status.SafeToApproach and should_launch:
            if seconds > 10:
                status = Status.ReadyToLaunch
                seconds = 0
                should_launch = False
        elif status == Status.ReadyToLaunch:
            if seconds > 5:
                status = Status.Launching
                seconds = 0
        elif status == Status.Launching:
            position = get_position(seconds, run_length, tube_length)
            velocity = get_velocity(seconds, run_length, tube_length)
            acceleration = get_acceleration(seconds, run_length, tube_length)
            battery_temperature = get_battery_temperature(seconds, initial_temp)
            battery_voltage = get_battery_voltage(seconds, initial_temp)
            battery_current = get_battery_current(seconds, initial_temp, resistance)
            pod_temperature = get_pod_temperature(seconds, initial_pod_temp)
            stripe_count = get_stripe_count(seconds, initial_stripe_count)

            if acceleration < 0:
                status = Status.Braking
        elif status == Status.Braking:
            position = get_position(seconds, run_length, tube_length)
            velocity = get_velocity(seconds, run_length, tube_length)
            acceleration = get_acceleration(seconds, run_length, tube_length)
            battery_temperature = get_battery_temperature(seconds, initial_temp)
            battery_voltage = get_battery_voltage(seconds, initial_temp)
            battery_current = get_battery_current(seconds, initial_temp, resistance)
            pod_temperature = get_pod_temperature(seconds, initial_pod_temp)
            stripe_count = get_stripe_count(seconds, initial_stripe_count)

            if seconds >= run_length:
                status = Status.SafeToApproach
        elif status == Status.SafeToApproach:
            position = tube_length
            velocity = 0
            acceleration = 0
            battery_temperature = get_battery_temperature(seconds, initial_temp)
            battery_voltage = get_battery_voltage(seconds, initial_temp)
            battery_current = get_battery_current(seconds, initial_temp, resistance)
            pod_temperature = get_pod_temperature(seconds, initial_pod_temp)
            stripe_count = get_stripe_count(seconds, initial_stripe_count)

        # Packet layout
        # team_id             uint8  Identifier for the team, assigned by the
        #                            organization. Required.
        # status              uint8  Pod status, indicating the current state the pod
        #                            is in. Required.
        # acceleration        int32  Acceleration in centimeters per second squared.
        #                            Required.
        # position            int32  Position in centimeters. Required.
        # velocity            int32  Velocity in centimeters per second. Required.
        # battery_voltage     int32  Battery voltage in millivolts. Optional
        # battery_current     int32  Battery current in milliamps. Optional
        # battery_temperature int32  Battery temperature in tenths of a degree
        #                            Celsius. Optional
        # pod_temperature     int32  Pod temperature in tenths of a degree Celsius.
        #                            Optional
        # stripe_count        uint32 Count of optical navigation stripes detected in
        #                            the tube. Optional
        packet = struct.pack(
            ">BB7iI",
            team_id,
            status,
            int(acceleration),
            int(position),
            int(velocity),
            0,
            0,
            0,
            0,
            int(position) // 3048,
        )
        print(packet)
        sock.sendto(packet, server)

        end_time = time()
        # Sleep the required time
        sleep(wait_time - (end_time - start_time))
        seconds += wait_time
