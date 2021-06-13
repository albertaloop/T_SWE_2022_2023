# Starter CANBus network
* On the left : _Arduino Serial Monitor_. Shows CANBus nodes with IDs 0x{100,500} transmitting and receiving on the CANBus network
* On the right: _pulseview_. D1 is connected to the CANL line. Note the CAN row is garbage (expects CANRX, I set it to CANL)

<img src="https://user-images.githubusercontent.com/11367325/116731921-57614080-a9b8-11eb-9808-dafb58018f95.gif" alt="CANBus gif" width="640"/>

## Setup

Arrange your CANBus network per the schematic below

<img src="https://user-images.githubusercontent.com/11367325/116735135-1ec36600-a9bc-11eb-9a2d-f38ecac2b1d4.png" alt="Devices drawio" width="640"/>

Open mike.ino in the _Arduino IDE_, plug in your teensy and upload the program to your board

Open the _Serial Monitor_. You should see TX/RX printouts

* Teensy 4.0
* 2x CANBus transceiver breakout boards [src -  CANBUS-BRKMINI of CAN transceiver SN65HVD230](https://copperhilltech.com/can-bus-mini-breakout-board/)
* Logic analyser [src - AZDelivery Logic Analyzer 24MHz 8CH with USB Cable](https://www.amazon.ca/AZDelivery-%E2%AD%90%E2%AD%90%E2%AD%90%E2%AD%90%E2%AD%90-Logic-Analyzer-24MHz/dp/B07F8C1PMQ/ref=sr_1_10?dchild=1&keywords=logic+analyzer&qid=1619794302&sr=8-10)
  * `sudo apt install sigrok` which install `pulseview`
  * Useful for debugging but not necessary

<img src="https://user-images.githubusercontent.com/11367325/116730943-1b79ab80-a9b7-11eb-952c-117d68bf394b.jpg" alt="Devices" width="640"/>

I tried to make all source .drawio files, images, videos accesible in [_Drive_](https://drive.google.com/drive/folders/1yV4G9WH04ELTwpJJdDj5AyTUAZnZolfz?usp=sharing)