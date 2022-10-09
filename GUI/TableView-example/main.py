from TableModel import TableModel
from TableViewEx import Ui_MainWindow
from PyQt5 import QtCore, QtGui, QtWidgets


class TableWindow(Ui_MainWindow):
    def __init__(self, window):
        self.setupUi(window)
        self.MainWindow = window
        self.tableModel = TableModel()
        self.tableView.setModel(self.tableModel)
        self.addDataBtn.clicked.connect(self.addDataBtnHandler)

    def addDataBtnHandler(self):
        self.tableModel.incrementCell(1, 1.2364283)


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    tableModel = TableModel()
    tableWindow = TableWindow(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
