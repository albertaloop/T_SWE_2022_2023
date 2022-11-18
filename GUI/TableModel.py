from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt
# Note: This version of TableModel uses dictionary now than list.
class TableModel(QtCore.QAbstractTableModel):
    def __init__(self, data):
        super(TableModel, self).__init__()
        self._data = data
    def rowCount(self, index):
        return len(self._data)
    def columnCount(self, index):
        return (2) # a Key and Value
    def flags(self, index):
        return QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsEditable|QtCore.Qt.ItemIsSelectable      

    def data(self, index, role):
        if role == Qt.DisplayRole:
            if index.column() == 0:
                return list(self._data.keys())[index.row()]
            elif index.column() == 1:
                return list(self._data.values())[index.row()]
            print("Yo, this ain't working!!")
    def setData(self, index, value, role):
        print(role,"WTH"*10)
    def update(self,new_data):
        self._data = new_data
        index1 = self.createIndex(
            0, 0, QtCore.QModelIndex())
        index2 = self.createIndex(
            self.rowCount(0), self.columnCount(0), QtCore.QModelIndex())
        self.dataChanged.emit(index1,index2)