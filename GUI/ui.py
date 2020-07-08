from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QDialog
from PyQt5.QtCore import pyqtProperty, QCoreApplication, QObject, QUrl
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import qmlRegisterType, QQmlComponent, QQmlEngine
from PyQt5.QtQuickWidgets import QQuickWidget
import sys
from AlbertaLoop_UI import Ui_MainWindow


class Logic(Ui_MainWindow):

    def __init__(self, window):
        self.setupUi(window)

        #Add functionality below!
        self.send_command_button.clicked.connect(self.command_button_clicked)
        
    def command_button_clicked(self):
        print('button pressed')


app = QApplication(sys.argv)
MainWindow = QMainWindow()
ui = Logic(MainWindow)
MainWindow.show()
sys.exit(app.exec_())


    #add more definitons here for functionality!!
