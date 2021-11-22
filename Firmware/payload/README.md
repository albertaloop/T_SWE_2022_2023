# Unified Payload PlatformIO project
TODO

PIO: PlatformIO

# Usage
Requires
* VSCode with PIO extension

Open this folder in VSCode. Click on the PIO extension; in the top left corner you should see "Project Tasks". Select the peripheral whose binary you want to generate from the tasks and click _Build_. It will provide you with a path to a _.hex_ file. You should upload that to the teensy.

If you are uploading to a Teensy connected directly to your computer, just press _Upload_.

If you are uploading to a remote computer, it goes something like:
```
you@pc$ scp bleh.hex abloop@10.0.10.1:/tmp/
abloop@remotepc$ teensy_loader_cli --mcu=TEENSY40 -s /tmp/bleh.hex
abloop@remotepc$ picocom -b 115200 /dev/serial/by-id/usb-Teensyduino_USB_Serial_8601650-if00  # to watch printout
```