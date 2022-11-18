
import struct
"""
TODO:
    get function for each telemetry value
    call self.notify() within recieveData, should I?
    query: what's the difference between update and recieveData functions?
    self-talk lol: do I really need to initialise the self.data to default telemetry
    delete the comment lines (self.telemetryTable1.setColumnCount(0)) in AlbertaLoop_UI2.py
    set port to self.port in the receiver function
    define headerData in the TableModel
    Hmm.. what are flags, should I implement it? # Source: https://www.pythonguis.com/faq/qtableview-cell-edit/
        def flags(self, index):
            if not index.isValid():
                return Qt.ItemIsEnabled

            return super().flags(index) | Qt.ItemIsEditable  # add editable flag.
"""
class TelemetryModel():

    def __init__(self):
        self.observers = []
        self.telemetryPacket = []
        self.packet_format = ">BB7iI"
        self.data = {
            "team_id": 0,
            "status": 0,
            "acceleration": 0,
            "position": 0,
            "velocity": 0,
            "battery_voltage": 0,
            "battery_current": 0,
            "battery_temperature": 0,
            "pod_temperature":0,
            "strip_count": 0,
            "highest velocity": 0
        }

    def attach(self, subscriber):
        print('Subscribed:',subscriber)
        self.observers.append(subscriber)

    def detach(self, subscriber):
        self.observers.remove(subscriber)


    def update(self,data):
        # print(data)
        self.telemetryPacket = data
        self.data = data
        self.notifyObservers()
    
    def getState(self):
        return self.data

    def notifyObservers(self):
        for observe in self.observers:
            observe.updateTelemetry(self.data)