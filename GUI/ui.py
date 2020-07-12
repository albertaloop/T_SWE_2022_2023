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

class Logic(Ui_MainWindow):

    def __init__(self, window):
        self.setupUi(window)
        #-----------------------------------------------------------------
        #Add functionality below!
        #User Added QML Widget for Speed Gauge
        self.spedometerWidget = QQuickWidget()
        self.spedometerWidget.setClearColor(QtCore.Qt.transparent)
        self.spedometerWidget.setResizeMode(QQuickWidget.SizeRootObjectToView)
        self.spedometerWidget.setSource(QUrl("Guage.qml"))
        self.speed_guage_layout.addWidget(self.spedometerWidget)

        #logo added
        pixmap = QtGui.QPixmap('img/Albertaloop_logo.png')
        self.albertaloop_logo.setPixmap(pixmap)

        # command button connects
        self.send_command_button.clicked.connect(self.command_button_clicked)
        self.send_command_button.clicked.connect(self.command_button_input)

        #Emergency button and Simulation button connects
        self.estop_button.clicked.connect(self.e_stop_button_clicked)
        self.simulation_button.clicked.connect(self.simulation_button_clicked)
        
        thread = Thread(target=self.getTelemetryData)
        thread.start()

    def getTelemetryData(self):
        MY_IP_ADDRESS = "127.0.0.1"
        PORT = 8820 
        telemetry_manager = telemetry_module.TelemetryManager(MY_IP_ADDRESS, PORT)
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
            print()
            print()
            self.battery_1_temp.setText(str(telemetry_manager.get_battery_voltage()))
            self.position_ind.setText(str(telemetry_manager.get_position()))
            self.bat_1_volt.setText(str(telemetry_manager.get_battery_voltage()))
            # self.progressBar.setProperty("value", telemetry_manager.get_position())
            elapsed_time = time.time() - start_time
            self.time_elapsed_label.setText("Time Elapsed: %.2f" % elapsed_time)
            # self.self.spedometerWidget.engine().setContextProperty('gauge_value', telemetry_manager.get_velocity())
            time.sleep(1)
    #functionality definitions   
 
    def command_button_clicked(self):
        print('Command button pressed')

    def command_button_input(self):
        text = self.command_line.text()

        #exits program (remove later plz)
        if text.lower() == 'exit':
            sys.exit()
        print('command >> ', text)
        

    def e_stop_button_clicked(self):
        print('EMERGENCY STOP BUTTON HAS BEEN PUSHED')

    def simulation_button_clicked(self):
        print('Entering simulation')

app = QApplication(sys.argv)
MainWindow = QMainWindow()
ui = Logic(MainWindow)
MainWindow.show()
sys.exit(app.exec_())
