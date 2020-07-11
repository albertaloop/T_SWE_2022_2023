from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QDialog, QHBoxLayout
from PyQt5.QtCore import pyqtProperty, QCoreApplication, QObject, QUrl
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlEngine
from PyQt5.QtQuickWidgets import QQuickWidget
import sys
from AlbertaLoop_UI import Ui_MainWindow
import telemetry_module


class Logic(Ui_MainWindow):

    def __init__(self, window):
        self.setupUi(window)
        #-----------------------------------------------------------------
        #Add functionality below!
        #User Added QML Widget for Speed Gauge
        spedometerWidget = QQuickWidget()
        spedometerWidget.setClearColor(QtCore.Qt.transparent)
        spedometerWidget.setResizeMode(QQuickWidget.SizeRootObjectToView)
        spedometerWidget.setSource(QUrl("Guage.qml"))
        self.speed_guage_layout.addWidget(spedometerWidget)

        #logo added
        pixmap = QtGui.QPixmap('img/Albertaloop_logo.png')
        self.albertaloop_logo.setPixmap(pixmap)

        # command button connects
        self.send_command_button.clicked.connect(self.command_button_clicked)
        self.send_command_button.clicked.connect(self.command_button_input)

        #Emergency button and Simulation button connects
        self.estop_button.clicked.connect(self.e_stop_button_clicked)
        self.simulation_button.clicked.connect(self.simulation_button_clicked)
        

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
