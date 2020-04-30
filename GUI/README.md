#AlbertaLoop GUI

*04/29/2020:* Basic features of Front end of this second GUI complete

This GUI was written in PyQT using QT Editor in Python and QtQuick2 in C++ with QML libraries. This is only the concept and functionalities are to be added in later.

## Features 
### April 29, 2020
GUI was made in a dark mode in contrast with the [first GUI ](https://github.com/albertaloop/T_SWE_2019_2020/tree/hsprabhakar-patch-1 "first GUI ")
Moreover, the higher contrast and tri-division design allows ease of access and usage.

#### Main window
[![main_window](https://github.com/albertaloop/T_SWE_2019_2020/blob/hsprabhakar-patch-1_qml/GUI/screenshots/main_window.png?raw=true "main_window")](https://raw.githubusercontent.com/albertaloop/T_SWE_2019_2020/hsprabhakar-patch-1_qml/GUI/screenshots/main_window.png?token=ALYW23764NDGETHBES647DC6WOEOA "main_window")

The first panel on the left shows a dynamic speed gauge written in QML through QtQuick2 with files under /img as its components. Position, time elapsed, and a progress bar are present. 

Second panel contains the Info Panel which displays a tabWidget that shows information about temperature, voltage, and other instrumental data. 
[![Pod Phase](https://raw.githubusercontent.com/albertaloop/T_SWE_2019_2020/hsprabhakar-patch-1_qml/GUI/screenshots/pod_state.png?token=ALYW23ZJUAXSVCWUCF5PUTC6WOE26 "Pod Phase")](https://raw.githubusercontent.com/albertaloop/T_SWE_2019_2020/hsprabhakar-patch-1_qml/GUI/screenshots/pod_state.png?token=ALYW23ZJUAXSVCWUCF5PUTC6WOE26 "Pod Phase")

Second tab of the tabWidget contains a list of phases. **As a demo, all the colours are shown but during implementation, only the colour of the active LCM phase will be displayed while the other stay grey**

[![Packet Logger](https://raw.githubusercontent.com/albertaloop/T_SWE_2019_2020/hsprabhakar-patch-1_qml/GUI/screenshots/packet_logger.png?token=ALYW234P2HQROKCLKQP7JK26WOFFG "Packet Logger")](https://raw.githubusercontent.com/albertaloop/T_SWE_2019_2020/hsprabhakar-patch-1_qml/GUI/screenshots/packet_logger.png?token=ALYW234P2HQROKCLKQP7JK26WOFFG "Packet Logger")

Tab 3 contains packet logger information with several buttons that do not have current functionality.  A **Seperate Window** button has been added so another window will pop up displaying the packet information at a more comfortable view. This functionality has not been added yet. 

## Fixes Needed

Apart from the functionality of the buttons, the speedGauge has an issue with the white background. The following code is used within the **AlbertaLoop_UI.py** main file

```python
      # USER ADDED QML WIDGET ##########################

        #Spedometer
        speed_gauge = QQuickView(QUrl('Guage.qml'))
        speed_gauge.setResizeMode(QQuickView.SizeRootObjectToView)
        quickWidget = QtWidgets.QWidget().createWindowContainer(speed_gauge)
        #why wont the next line work :(
        quickWidget.setStyleSheet("background-color: transparent;")
        self.speed_guage_layout.addWidget(quickWidget)
```
Despite the attempt to fix line 8, it appears that QtWidgets.QWidget().createWindowContainer(speed_gauge) cannot have its color changed. If there is any way to fix it, please submit a pull request and change the box color to either of the following: **background-color: transparent **OR **rgb (45,45,45)**








