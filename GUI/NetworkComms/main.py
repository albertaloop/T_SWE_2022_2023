from cmd_transmitter import CmdTransmitter
from telemetry_receiver import TelemetryReceiver
from udp_module import UDPModule


if __name__ == "__main__":
    telemetryReceiver = TelemetryReceiver(1024)
    cmdTransmitter = CmdTransmitter()
    udpModule = UDPModule("127.0.0.1", 65432,
                          telemetryReceiver, cmdTransmitter)

    udpModule.transmitter.sendPacket("message")
    udpModule.transmitter.sendPacket("message1")
    udpModule.transmitter.sendPacket("message2")
    udpModule.transmitter.sendPacket("message3")