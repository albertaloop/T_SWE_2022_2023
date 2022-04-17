#!/bin/sh

python3 main.py --server_ip $1 --server_port $2 & python3 ../telemetry_module/mock-pod.py --server_ip $1 --server_port $2
