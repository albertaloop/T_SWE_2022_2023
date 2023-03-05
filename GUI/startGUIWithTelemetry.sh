#!/bin/sh

# python3 main2.py --server_ip $1 --server_port $2 &  python3 ../telemetry_module/mock-pod.py --server_ip $1 --server_port $2  &  python3 ../telemetry_module/mock-pod-receiver.py
# python3 main.py --server_ip $1 --server_port $2 &  python3 ../telemetry_module/mock-pod.py --server_ip $1 --server_port $2
python3 main.py & python3 ../telemetry_module/mock-pod.py
#python main.py --server_ip 127.0.0.1 --server_port 4000 &  python3 ../telemetry_module/mock-pod.py --server_ip 127.0.0.1 --server_port 4000
