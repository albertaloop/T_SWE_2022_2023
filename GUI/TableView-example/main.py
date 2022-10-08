from TableModel import TableModel
from TableViewEx import Ui_MainWindow
from PyQt5 import QtCore, QtGui, QtWidgets


class TableWindow():
    def __init__(self, window):
        self.MainWindow = window
        self.tableModel = TableModel()
        self.tableView = QtWidgets.QTableView()
        self.tableView.setModel(self.tableModel)
        self.MainWindow.setCentralWidget(self.tableView)
        print(self.tableView.model)


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    tableModel = TableModel()
    tableWindow = TableWindow(MainWindow)
    MainWindow.show()
    # print(TableWindow.tableModel.items[0][0])
    # ui.tableModel.dataChanged.emit()
    app.exec_()
