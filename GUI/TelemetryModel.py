
import struct
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt

"""
TODO:
    get function for each telemetry value
    call self.notify() within recieve_data, should I?
    query: what's the difference between update and recieve_data functions?
    set port to self.port in the receiver function
    define header_data in the TableModel
    
"""
class TelemetryModel(QtCore.QAbstractTableModel):

    def __init__(self):
        super(TelemetryModel, self).__init__()

        self.packet_format = ">BB7i"
        self._data = {
            "team_id": 0,
            "status": 0,
            "acceleration": 0,
            "position": 0,
            "velocity": 0,
            "battery_voltage": 0,
            "battery_current": 0,
            "battery_temperature": 0,
            "pod_temperature":0,
        }

    def rowCount(self, index):
        return len(self._data)
    def columnCount(self, index):
        return (2) # a Key and Value
    def flags(self, index):
        return QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsEditable|QtCore.Qt.ItemIsSelectable      
    

    def update(self,_data):
        self._data = _data
        index1 = self.createIndex(
            0, 0, QtCore.QModelIndex())
        index2 = self.createIndex(
            self.rowCount(0), self.columnCount(0), QtCore.QModelIndex())
        self.dataChanged.emit(index1,index2)

    def getState(self):
        return self._data

    def data(self, index, role):
        if role == Qt.DisplayRole:
            if index.column() == 0:
                return list(self._data.keys())[index.row()]
            elif index.column() == 1:
                return list(self._data.values())[index.row()]
