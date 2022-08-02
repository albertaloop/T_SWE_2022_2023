from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QDialog, QHBoxLayout
from PyQt5.QtCore import pyqtProperty, QCoreApplication, QObject, QUrl
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlEngine
from PyQt5.QtQuickWidgets import QQuickWidget
import sys
from AlbertaLoop_UI2 import Ui_MainWindow
import telemetry_module
import time
from threading import Thread
from datetime import datetime
from argparse import ArgumentParser


class MWindowWrapper(Ui_MainWindow):
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
        self.speedGaugeLayout.addWidget(self.spedometerWidget)

        # logo added
        pixmap = QtGui.QPixmap("img/Albertaloop_logo.png")
        self.albertaloopLogo.setPixmap(pixmap)


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
    mWindowWrapper = MWindowWrapper(MainWindow, args.server_ip, args.server_port)
    MainWindow.show()
    sys.exit(app.exec_())
