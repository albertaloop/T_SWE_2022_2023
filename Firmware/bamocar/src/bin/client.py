import argparse
import logging
import serial
import sys
import pathlib

logger = logging.getLogger(__name__)


class Client:
    def __init__(self, serial: serial.Serial):
        serial.write(b"a")
        logger.info(serial.readline())
        serial.write(b"b")
        logger.info(serial.readline())
        serial.write(b"c")
        logger.info(serial.readline())


def main(default_port=None, default_baudrate=9600):
    """Command line tool, entry point"""

    parser = argparse.ArgumentParser(description="A simple serial client")

    parser.add_argument(
        "port", nargs="?", help="serial port name", default=default_port
    )

    parser.add_argument(
        "baudrate",
        nargs="?",
        type=int,
        help="set baud rate, default: %(default)s",
        default=default_baudrate,
    )

    args = parser.parse_args()

    if not args.port:
        CONFIG_PATH = pathlib.Path(pathlib.Path.home() / ".config/bamocar/slave")
        try:
            with open(CONFIG_PATH, "r") as f:
                args.port = f.readline()
            logger.debug(f"Port automatically detected: {args.port}")
        except FileNotFoundError:
            parser.error(f"Port is not given, nor can it be sourced from {CONFIG_PATH}")

    try:
        ser = serial.Serial(args.port, args.baudrate, timeout=1)
        if not ser.is_open:
            ser.open()
        Client(ser)
    except serial.SerialException as e:
        logger.error("could not open port {!r}: {}\n".format(args.port, e))
        sys.exit(1)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
