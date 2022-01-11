# State Machine and Base Class Code
This is V0.9 of the combined State Machine and Movement Module code. Additionally
contains several classes for easy CANbus communications.

#### MessageMaster
Message Master allows for very quick CANbus communications using ABloop's CAN protocol.
Please refer to the documentation on Google Drive or code comments for more information.

#### BamocarInterface
BamocarInterface is a class which allows for quick parameter changes in the Bamocar. It
possesses methods with built-in unit conversions, allowing for easy data input.

#### StateMachine 
The class containing the internal logic of the State Machine. Utilizes instances of the
previous two classes, combined with multi-threading to control the pod.