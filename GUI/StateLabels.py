class StateLabels():

    def __init__(self):


#updates label colors if state is not equal to current_state
    def updateCurrentState(state):
        current_state = "fault"
        if state== current_state:
            True
        elif state!= current_state:
            current_state=state
            if state== 'fault':
                self.label12.setStylesheet("background-color: red")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'safe':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: #89CFF0")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'ready':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: green")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'launch':
                self.label12.setStylesheet("background-color: #89CFF0")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: green")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'coast':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: green")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: gray")
            if state== 'break':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: yellow")
                self.label6.setStylesheet("background-color: gray")
            if state== 'crawl':
                self.label12.setStylesheet("background-color: gray")
                self.label11.setStylesheet("background-color: gray")
                self.label10.setStylesheet("background-color: gray")
                self.label9.setStylesheet("background-color: gray")
                self.label8.setStylesheet("background-color: gray")
                self.label7.setStylesheet("background-color: gray")
                self.label6.setStylesheet("background-color: yellow")
