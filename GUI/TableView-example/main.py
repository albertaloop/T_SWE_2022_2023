from TableModel import TableModel
from TableViewEx import Ui_MainWindow
from PyQt5 import QtCore, QtGui, QtWidgets


class TableWindow(Ui_MainWindow):
    def __init__(self, window, tableModel):
        self.setupUi(window)
        self.tabelModel = tableModel
        self.tableView.setModel(self.tableModel)


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    tableModel = TableModel()
    ui = TableWindow(MainWindow, tableModel)
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
