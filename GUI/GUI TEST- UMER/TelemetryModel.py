class TelemetryModel:

    def __init__(self):
        self.observers = []
        self.telemetryPacket = []
        self.team_id = 0
        self.status = 0
        self.acceleration = 0
        self.position = 0
        self.velocity = 0
        self.battery_voltage = 0
        self.battery_current = 0
        self.battery_temperature = 0
        self.pod_temperature = 0
        self.stripe_count = 0

    def recieveData(self, data):
        self.telemetryPacket = data

    def attach(self, subscriber):
        self.observers.append(subscriber)

    def detach(self, subscriber):
        self.observers.remove(subscriber)

    def notify(self):
        for observe in self.observer:
            observe.updateTelemetry(self)