from PyQt5 import QtCore
from PyQt5.QtCore import QAbstractTableModel


class TableModel(QAbstractTableModel):
    def __init__(self):
        self.num_cols = 2
        self.num_rows = 4
        self.items = [self.num_rows][self.num_cols]
        self.items[0][0] = "Item 1"
        self.items[1][0] = "Item 2"
        self.items[2][0] = "Item 3"
        self.items[3][0] = "Item 4"

    def rowCount(self):
        return self.num_rows

    def columnCount(self):
        return self.num_cols

    def data(self, index, role):
        if role == Qt.DisplayRole:
            text = self.items[index.row()][index.column()]
            return text
