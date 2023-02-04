class Logic():
    def __init__(self, model, CMDTransmitter):
        self.model = model
        self.CMDTransmitter = CMDTransmitter
    
    def sendCommand(self,msg,stateRequested):
        self.CMDTransmitter.sendPacket(msg)
        pass

    def printmessage(self, message):
        self.sendCommand(message,"")
        print("The universe is a small place.")
        print(message)