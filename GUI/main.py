from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QDialog, QHBoxLayout
from PyQt5.QtCore import pyqtProperty, QCoreApplication, QObject, QUrl
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlEngine
from PyQt5.QtQuickWidgets import QQuickWidget
import sys
from AlbertaLoop_UI import Ui_MainWindow
import telemetry_module
import time
from threading import Thread
from datetime import datetime
from argparse import ArgumentParser


class Logic(Ui_MainWindow):
    def __init__(self, window, ip, port):
        self.setupUi(window)
        self.ip = ip
        self.port = port
        # -----------------------------------------------------------------
        # Add functionality below!
        # User Added QML Widget for Speed Gauge
        self.spedometerWidget = QQuickWidget()
        self.spedometerWidget.setClearColor(QtCore.Qt.transparent)
        self.spedometerWidget.setResizeMode(QQuickWidget.SizeRootObjectToView)
        self.spedometerWidget.setSource(QUrl("Guage.qml"))
        self.speed_guage_layout.addWidget(self.spedometerWidget)

        # logo added
        pixmap = QtGui.QPixmap("img/Albertaloop_logo.png")
        self.albertaloop_logo.setPixmap(pixmap)

        # command button connects
        self.send_command_button.clicked.connect(self.command_button_clicked)
        self.send_command_button.clicked.connect(self.command_button_input)

        # Emergency button and Simulation button connects
        self.estop_button.clicked.connect(self.e_stop_button_clicked)
        self.simulation_button.clicked.connect(self.simulation_button_clicked)

        thread = Thread(target=self.getTelemetryData)
        thread.start()

    def getTelemetryData(self):
        telemetry_manager = telemetry_module.TelemetryManager(self.ip, self.port)
        start_time = time.time()
        self.current_state_ind_2.setText("Active Connection")
        self.pod_connect_ind.setText("Active Connection")
        self.thing_2_label.setText("Stripe Count")

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
            print("\n")

            self.packetTextBrowser.append(
                "Team ID: " + str(telemetry_manager.get_team_id())
            )
            self.packetTextBrowser.append(
                "Status: " + str(telemetry_manager.get_status())
            )
            self.packetTextBrowser.append(
                "Acceleration: " + str(telemetry_manager.get_acceleration())
            )
            self.packetTextBrowser.append(
                "Velocity: " + str(telemetry_manager.get_velocity())
            )
            self.packetTextBrowser.append(
                "Position: " + str(telemetry_manager.get_position())
            )
            self.packetTextBrowser.append(
                "Battery Voltage: " + str(telemetry_manager.get_battery_voltage())
            )
            self.packetTextBrowser.append(
                "Battery Current: " + str(telemetry_manager.get_battery_current())
            )
            self.packetTextBrowser.append(
                "Battery Temperature: "
                + str(telemetry_manager.get_battery_temperature())
            )
            self.packetTextBrowser.append(
                "Pod Temperature: " + str(telemetry_manager.get_pod_temperature())
            )
            self.packetTextBrowser.append(
                "Stripe Count: " + str(telemetry_manager.get_stripe_count())
            )
            self.packetTextBrowser.append(
                "Highest Velocity: " + str(telemetry_manager.get_highest_velocity())
            )
            self.packetTextBrowser.append("\n")
            self.packetTextBrowser.moveCursor(QtGui.QTextCursor.End)

            self.battery_1_temp.setText(str(telemetry_manager.get_battery_voltage()))
            self.position_ind.setText(str(telemetry_manager.get_position()))
            self.bat_1_volt.setText(str(telemetry_manager.get_battery_voltage()))
            # self.progressBar.setProperty("value", telemetry_manager.get_position())
            elapsed_time = time.time() - start_time
            self.time_elapsed_label.setText("Time Elapsed: %.2f" % elapsed_time)
            # self.self.spedometerWidget.engine().setContextProperty('gauge_value', telemetry_manager.get_velocity())
            time.sleep(1)

    # functionality definitions

    def command_button_clicked(self):
        # TODO send general command to pod
        print("Command button pressed")

    def command_button_input(self):
        text = self.command_line.text()

        # exits program TODO (remove later plz)
        if text.lower() == "exit":
            sys.exit()
        print("command >> ", text)

    def e_stop_button_clicked(self):
        # TODO send stop packet to pod
        print("EMERGENCY STOP BUTTON HAS BEEN PUSHED")

    def simulation_button_clicked(self):
        # TODO luanch new window to start simulation testing on pod
        print("Entering simulation")


if __name__ == "__main__":
    parser = ArgumentParser(description="Albertaloop GUI launch")
    parser.add_argument(
        "--server_ip", default="192.168.0.1", help="The ip to send the packets to"
    )
    parser.add_argument(
        "--server_port", type=int, default=3000, help="The UDP port to send packets to"
    )

    args = parser.parse_args()

    app = QApplication(sys.argv)
    MainWindow = QMainWindow()
    ui = Logic(MainWindow, args.server_ip, args.server_port)
    MainWindow.show()
    sys.exit(app.exec_())
