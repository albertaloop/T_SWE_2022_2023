import telemetry_module
from subprocess import call
import time

# Run "python3 mock-pod.py --server_ip 192.168.1.84 --server_port 3000" in another shell

telemetry_manager = telemetry_module.TelemetryManager("192.168.1.84", 3000)

# call(["python3", "mock-pod.py", "--server_ip", "192.168.1.84", "--server_port", "3000", "&"])

while True:
    print("Team ID: ", telemetry_manager.get_team_id())
    print("Status: ", telemetry_manager.get_status())
    print("Acceleration: ", telemetry_manager.get_acceleration())
    print("Velocity: ", telemetry_manager.get_velocity())
    print("Position: ", telemetry_manager.get_position())
    print("Battery Voltage: ", telemetry_manager.get_battery_voltage())
    print("Battery Current: ", telemetry_manager.get_battery_current())
    print("Battery Temperature: ", telemetry_manager.get_battery_temperature())
    print("Pod Temperature: ", telemetry_manager.get_pod_temperature())
    print("Stripe Count: ", telemetry_manager.get_stripe_count())
    print("Highest Velocity: ", telemetry_manager.get_highest_velocity())
    print()
    print()
    time.sleep(1)
