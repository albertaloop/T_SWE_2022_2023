from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow
from AlbertaLoop_UI import Ui_MainWindow
import telemetry_module
import sys

class MyWindow(QMainWindow):

    def __init__(self):
        super(MyWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(window)
        self.initUI()
        self.telemetry_manager = telemetry_module.TelemetryManager("192.168.1.84", 3000)
        
    def initUI(self):
        self.ui.send_command_button.clicked(self.clicked)

    def clicked(self):
        print("You pressed the command button.")

def window():
    app = QApplication(sys.argv)
    win = MyWindow()
    win.show()
    sys.exit(app.exec_())


window()