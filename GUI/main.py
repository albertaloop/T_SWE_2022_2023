from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QDialog, QHBoxLayout
from PyQt5.QtCore import pyqtProperty, QCoreApplication, QObject, QUrl
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlEngine
from PyQt5.QtQuickWidgets import QQuickWidget
import sys
from AlbertaLoop_UI import Ui_MainWindow
# import telemetry_module
from TelemetryModel import TelemetryModel

from HealthCheckModel import HealthCheckModel


from Actions.Command import Launch
from Actions.Command import PrepareLaunch
from Actions.Command import EStop
from Actions.Command import Crawl
from Actions.HealthCheck import HealthCheck


import time
import threading
from datetime import datetime
from argparse import ArgumentParser

from NetworkComms.udp_module import UDPModule
from NetworkComms.telemetry_receiver import TelemetryReceiver
from NetworkComms.cmd_transmitter import CmdTransmitter

import signal # Make Ctrl+C work with PyQt5 Applications
signal.signal(signal.SIGINT, signal.SIG_DFL)

class MWindowWrapper(Ui_MainWindow):

    def __init__(self, window, telemetry_model, udp_module):
        self.setupUi(window)

        self.command = None
        self.udp_module = udp_module
        self.current_state = "fault"
        self.command_requested = ["none"]
        self.healthchk_requested = ["none"]
        self.estop_requested = ["none"]
        self.cmd_lock = threading.Lock()

        # -----------------------------------------------------------------
        # Add functionality below!
        # User Added QML Widget for Speed Gauge
        self.spedometerWidget = QQuickWidget()
        self.spedometerWidget.setClearColor(QtCore.Qt.transparent)
        self.spedometerWidget.setResizeMode(QQuickWidget.SizeRootObjectToView)
        self.spedometerWidget.setSource(QUrl("assets/Guage.qml"))
        self.speedGaugeLayout.addWidget(self.spedometerWidget)

        # Connect clicked functions
        self.launchBtn.clicked.connect(self.launchBtn_clicked)
        self.healthChkBtn.clicked.connect(self.healthChkBtn_clicked)
        self.crawlBtn.clicked.connect(self.crawlBtn_clicked)
        self.prepLaunchBtn.clicked.connect(self.prepLaunchBtn_clicked)
        self.eStopBtn.clicked.connect(self.eStopBtn_clicked)

        # logo added
        pixmap = QtGui.QPixmap("img/Albertaloop_logo.png")
        self.albertaloopLogo.setPixmap(pixmap)

        self.telemetryTable1.setModel(telemetry_model)

    # Clicked function definitions
    def launchBtn_clicked(self):
        print("Launch button pressed")
        if self.command_requested == ["none"]:
            if self.current_state == ["ready_to_launch"] :
                self.command_requested = ["launch"]
                self.executeCommand(Launch(self.udp_module), self.command_requested)
            else :
                print("Not ready to launch")
        else:
            print("Waiting for another command: ")
            print(self.command_requested)
            print("\n")


    def healthChkBtn_clicked(self):
        print("Health check button pressed")
        if self.healthchk_requested == ["none"]:
            self.healthchk_requested = ["yes"]
            self.executeCommand(HealthCheck(self.udp_module), self.healthchk_requested)
            print("Health check requested")
        else :
            print("Health check already requested")

    def crawlBtn_clicked(self):
        print("Crawl button pressed")
        if self.command_requested == ["none"]:
            if self.current_state == ["idle"]:
                self.command_requested = ["crawl"]
                self.executeCommand(Crawl(self.udp_module), self.command_requested)
                print("Crawl requested")
            else :
                print("Not ready to crawl, pod must be idle")
        else:
            print("Waiting for another command: ")
            print(self.command_requested)
            print("\n")

    def prepLaunchBtn_clicked(self):
        print("Prepare Launch button pressed")
        if self.command_requested == ["none"]:
            if self.current_state == ["idle"] :
                self.command_requested = ["prep_launch"]
                self.executeCommand(PrepareLaunch(self.udp_module), self.command_requested)
                print("Prepare to launch requested")
            else :
                print("Not ready for prepare to launch, pod must be idle")
        else:
            print("Waiting for another command: ")
            print(self.command_requested)
            print("\n")

    def eStopBtn_clicked(self):
        print("Estop button pressed")
        self.cmd_lock.acquire()
        if self.estop_requested == ["none"]:
            self.estop_requested = ["yes"]
            self.cmd_lock.release()
            self.executeCommand(EStop(self.udp_module), self.estop_requested)
            print("Emegency stop requested")
        else :
            print(self.estop_requested)
            self.cmd_lock.release()
            print("EStop already sending")


    # Command Pattern definitions
    def executeCommand(self, command, cmd_state):
        self.command = command
        self.cmd_thread = threading.Thread(target=self.command.execute, args=[cmd_state, self.cmd_lock])
        self.cmd_thread.run()
        

    def command_input(self):
        text = self.command_line.text()
        # exits program TODO (remove later plz)
        if text.lower() == "exit":
            sys.exit()
        print("command >> ", text)

    
        
if __name__ == "__main__":
    parser = ArgumentParser(description="Albertaloop GUI launch")
    parser.add_argument(
        "--gui_addr", default="192.168.1.11", help="The ip of the GUI host machine"
    )
    parser.add_argument(
        "--remote_addr", default="192.168.1.10", help="The ip to send packets to"
    )
    parser.add_argument(
        "--server_port", type=int, default=4000, help="The UDP port to get updates from the pod"
    )
    parser.add_argument(
        "--client_port", type=int, default=3000, help="The UDP port to send packets to"
    )

    args = parser.parse_args()
    app = QApplication(sys.argv)

    # Model Classes
    TelemetryModel = TelemetryModel()
    HealthCheckModel = HealthCheckModel()
    TelemetryReceiver = TelemetryReceiver()
    CmdTransmitter = CmdTransmitter()

    # Controller Classes
    TelemetryReceiver.setDataModel(TelemetryModel)
    # HealthCheckReq = HealthCheckReq(HealthCheckModel)

    UDPModule = UDPModule(args.gui_addr, args.remote_addr, args.server_port, args.client_port,
                          TelemetryReceiver, CmdTransmitter)

    # View Classes
    MainWindow = QMainWindow()
    mWindowWrapper = MWindowWrapper(MainWindow, TelemetryModel,
        UDPModule)
    

    TelemetryReceiver.start()
    
    MainWindow.show()
    sys.exit(app.exec_())
