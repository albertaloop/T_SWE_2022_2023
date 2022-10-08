from PyQt5 import QtCore

from PyQt5.QtCore import Qt
from PyQt5.QtCore import QAbstractTableModel


class TableModel(QAbstractTableModel):
    def __init__(self):
        super(TableModel, self).__init__()
        # https://www.pythonpool.com/python-2d-list/
        self.items = [
            ["Item 1", "item 1 val"],
            ["Item 2", "item 2 val"],
            ["Item 3", "Item 3 val"],
            ["Item 4", "Item 4 val"],
        ]
        print("table model initialized")

    def rowCount(self, index):
        print("counting rows")
        return len(self.items)

    def columnCount(self, index):
        print("counting columns")
        return len(self.items[0])

    def headerData(self, section, orientation, role):
        if orientation == Qt.Horizontal and role == Qt.DisplayRole:
            return ["Description", "Value"][section]
        if orientation == QtCore.Qt.Vertical:
            return "List " + str(section)

    # def flags(self, index):
    #     if not index.isValid():
    #         return None
    #     return Qt.ItemIsEnabled

    def data(self, index, role):
        print("getting data")
        if role == Qt.DisplayRole:
            text = self.items[index.row()][index.column()]
            print("display role")
            return text

    # def insertRow(self, row: int, parent: QModelIndex = ...) -> bool:
    #     Qt.beginInsertRows()
    #     return super().insertRow(row, parent)

    # def insertRows(self, row: int, count: int, parent: QModelIndex = ...) -> bool:
    #     return super().insertRows(row, count, parent)

    # def insertColumn(self, column: int, parent: QModelIndex = ...) -> bool:
    #     return super().insertColumn(column, parent)

    # def insertColumns(self, column: int, count: int, parent: QModelIndex = ...) -> bool:
    #     return super().insertColumns(column, count, parent)

    # def update_model(self, data):
    #     print('update_model')
    #     index_1 = self.index(0, 0)
    #     index_2 = self.index(0, 1)
    #     self.layoutChanged.emit(index_1, index_2, [Qt.DisplayRole])
