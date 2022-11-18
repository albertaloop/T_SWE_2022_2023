from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QDialog, QHBoxLayout
from PyQt5.QtCore import pyqtProperty, QCoreApplication, QObject, QUrl
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlEngine
from PyQt5.QtQuickWidgets import QQuickWidget
import sys
from AlbertaLoop_UI2 import Ui_MainWindow
import telemetry_module
from TelemetryModel import TelemetryModel
from Logic import Logic
from HealthCheckReq import HealthCheckReq
from HealthCheckModel import HealthCheckModel
from Launch import Launch
from PrepareLaunch import PrepareLaunch
from EStop import EStop
from Crawl import Crawl
from HealthCheck import HealthCheck
import time
from threading import Thread
from datetime import datetime
from argparse import ArgumentParser
from TelemetryReceiver import TelemetryReceiver

class MWindowWrapper(Ui_MainWindow):



    def __init__(self, window, ip, port):
        self.setupUi(window)
        self.ip = ip
        self.port = port

        self.command = None
        self.receivers = []

        # -----------------------------------------------------------------
        # Add functionality below!
        # User Added QML Widget for Speed Gauge
        self.spedometerWidget = QQuickWidget()
        self.spedometerWidget.setClearColor(QtCore.Qt.transparent)
        self.spedometerWidget.setResizeMode(QQuickWidget.SizeRootObjectToView)
        self.spedometerWidget.setSource(QUrl("Guage.qml"))
        self.speedGaugeLayout.addWidget(self.spedometerWidget)

        # Connect clicked functions
        self.launchBtn.clicked.connect(self.launchBtn_clicked)
        self.healthChkBtn.clicked.connect(self.healthChkBtn_clicked)
        self.crawlBtn.clicked.connect(self.crawlBtn_clicked)
        self.prepLaunchBtn.clicked.connect(self.crawlBtn_clicked)
        self.eStopBtn.clicked.connect(self.eStopBtn_clicked)


        # logo added
        pixmap = QtGui.QPixmap("img/Albertaloop_logo.png")
        self.albertaloopLogo.setPixmap(pixmap)


    # Clicked function definitions
    def launchBtn_clicked(self):
        print("Launch button pressed")
        self.setCommand(Launch(self.receivers[0]))
        self.executeCommand()
    def healthChkBtn_clicked(self):
        print("Health check button pressed")
        self.setCommand(HealthCheck(self.receivers[1]))
        self.executeCommand()
    def crawlBtn_clicked(self):
        self.setCommand(Crawl(self.receivers[0]))
        self.executeCommand()
        print("Crawl button pressed")
    def prepLaunchBtn_clicked(self):
        self.setCommand(PrepareLaunch(self.receivers[0]))
        self.executeCommand()
        print("Prepare Launch button pressed")
    def eStopBtn_clicked(self):
        self.setCommand(EStop(self.receivers[0]))
        self.executeCommand()
        print("Estop button pressed")

    # Command Pattern definitions
    def setCommand(self, command):
        self.command = command
    def executeCommand(self):
        self.command.execute()
    def setReceivers(self, rcv1, rcv2):
        # Receiver 1 is Logic
        self.receivers.append(rcv1)
        # Receiver 2 is HealthChkReq
        self.receivers.append(rcv2)

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
    #updates label colors if state is not equal to current_state
    def updateCurrentState(state):
        current_state = "fault"
        if state== current_state:
            True
        elif state!= current_state:
            current_state=state
            if state== 'fault':
                self.label12.setStylesheet("background-color: red")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'safe':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: #89CFF0")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'ready':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: green")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'launch':
                self.label12.setStylesheet("background-color: #89CFF0")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: green")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'coast':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: green")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'break':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: yellow")
                self.label6.setStylesheet("background-color: gray")
            if state== 'crawl':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: yellow")

        def notify(state):
            updateCurrentstate(state)

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

    # Model Classes
    TelemetryModel = TelemetryModel()
    HealthCheckModel = HealthCheckModel()
    TelemetryReceiver = TelemetryReceiver(args.server_ip, args.server_port)
    # Controller Classes
    Logic = Logic(TelemetryModel)
    TelemetryReceiver.setDataModel(TelemetryModel)
    HealthCheckReq = HealthCheckReq(HealthCheckModel)

    TelemetryReceiver.start()
    # View Classes
    MainWindow = QMainWindow()
    mWindowWrapper = MWindowWrapper(MainWindow, args.server_ip, args.server_port)
    mWindowWrapper.setReceivers(Logic, HealthCheckReq)
    TelemetryModel.attach(mWindowWrapper)
    

    # TelemetryModel.recieveData([9]*10)
    # TelemetryModel.notify()
    # TelemetryModel.update([0.123]*10)
    
    
    
    MainWindow.show()
    sys.exit(app.exec_())
