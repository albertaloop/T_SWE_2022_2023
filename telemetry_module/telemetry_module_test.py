import telemetry_module
from subprocess import Popen

telemetry_manager = telemetry_module.TelemetryManager("192.168.1.84", 3000)

Popen("python3 mock-pod.py --server_ip 192.168.1.84 --server_port 3000")

print(telemetry_manager.get_acceleration())