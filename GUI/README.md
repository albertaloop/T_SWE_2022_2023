# AlbertaLoop GUI

-------------------------------------------------------------------------------
## Coding Notes


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

## GUI Variable Names

So far, the only variables are the GUI element variables. _Anything named self.'...'_label represents titles/labels on the GUI. Variables named self.'...'_ind or self.'...' are values/text that will be dynamically changed 

## Fixes Needed










