# AlbertaLoop GUI

---

## How to use

### How to start GUI

1. Install pyqt5 on your computer. For example for mac, simply run `brew install pyqt5`
2. Navigate to the directory containing this README and run `python3 main.py`
3. To exit out, press Ctrl+C
4. To launch the GUI with the fake telemetry connection, use the included shell script. `sh startGUIWithTelemetry.sh 127.0.0.1 8000` where 127.0.0.1 is your localhost IP and port refers to an open port on your computer. Feel free to change these numbers if they don't work for you. Moreover, this shell script has been tested on Mac. If it doesn't work on your system, feel free to make a new script and please push that to the repo so others can use it as well.

### How to edit GUI
If you intend to edit a UI using QTDesigner, download the .ui version of it from github, open it on QTDesigner, edit, and submit a pull request for both the .ui and .py versions.

If you are just editing non-UI methods, do not alter the .ui file and class Ui_MainWindow in the .py version unless changes were made in those locations.

1. The GUI's user interface was built using QT Designer. Therefore, to edit the UI, you will also need to download the QT Designer. You can find a lightweight QT designer program at (https://build-system.fman.io/qt-designer-download), which doesn't include the gigabytes of unnecessary stuff you get with the regular QTCreator installation.

2. Then use QT Designer tp open the AlbertaLoop_UI.ui file, and make whatever edits you want

3. After making the edits, run the command `pyuic5 -x AlbertaLoop_UI.ui -o AlbertaLoop_UI.py` to auto generate the corresponding python file for the UI.

### GUI Directory
We will be trying to match the convention provided in the Mastering GUI book as much as possible. Feel free to add to this list by making a pull request so other team members can review the change.

1. File names will follow a lowercase underscores naming convention. For ex. "main_window.py"
2. To remain consistent with the Mastering GUI book, variable names will also follow lowercase underscore naming convention. For ex. health_panel = ...
3. To remain consistent with the Mastering GUI book, method names will follow a camel case naming convention. For ex. doSomething()...
4. Class names will follow a pascal case convention. ex. SomeClass
5. Constants will use an all caps underscore naming convention. ex. SPEED_OF_LIGHT = 3E8

## Coding Notes

_04/29/2020:_ Basic features of Front end of this second GUI complete

This GUI was written in PyQT using QT Editor in Python and QtQuick2 in C++ with QML libraries. This is only the concept and functionalities are to be added in later.

IF you are openning the code in VSCode, right click ui.py and hit _Run Current File in Python Interactive Window_ otherwise the QML widget will not show

## Features

### July 7th, 2020

A major functionality update was done to the GUI. Now, ALL logic/functionality code will go under ui.py and only front end changes will be altered on the AlbertaLoop_UI.ui using QT Designer

This means that if more features are added to the front end of the GUI, the resulting .ui file can be converted to .py and adding widgets and functions is not to be dealt with anymore as that is all in the ui.py file.

#### Functionality Changes

As a test, now when something is written into the command line of the GUI and the button is pressed, text is returned to the console.
Typing 'exit' and pressing the button exits the program completely.

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

Tab 3 contains packet logger information with several buttons that do not have current functionality. A **Seperate Window** button has been added so another window will pop up displaying the packet information at a more comfortable view. This functionality has not been added yet.

## GUI Variable Names

So far, the only variables are the GUI element variables. \_Anything named self.'...'\_label represents titles/labels on the GUI. Variables named self.'...'\_ind or self.'...' are values/text that will be dynamically changed

## Fixes/Actions Needed

July 7th, 2020:
Add the telemetry module stuff in for simulation
