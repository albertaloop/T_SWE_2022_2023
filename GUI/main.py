###################################################################
# The main file where internal signals and slots can be connected #
###################################################################

import sys
from mainwindow import Ui_MainWindow
from PyQt5 import QtWidgets as qtw
from PyQt5 import QtGui as qtg
from PyQt5 import QtCore as qtc
from PyQt5.QtWidgets import QApplication, QMainWindow


#Derived from page 18 of Mastering GUI Program

class MainWindow(qtw.QWidget, Ui_MainWindow):

    def __init__(self):
        """MainWindow constructor"""
        super().__init__()
        #Main UI code here--
        self.setupUi(self)




        #End main UI code--
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = QMainWindow()

    ui = Ui_MainWindow()
    ui.setupUi(window)

    window.show()
    sys.exit(app.exec_())
