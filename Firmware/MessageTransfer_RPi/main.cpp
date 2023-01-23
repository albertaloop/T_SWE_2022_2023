#include <termios.h>

int serial_fd;

void pollUart() {
    struct pollfd src;
    src.fd = serial_fd;
    src.events = POLLIN;
    src.revents = 0;   

    int check = poll(&src, 1, -1);
}

int main() {
    //https://raspberrypi.stackexchange.com/questions/57906/uart-interrupt-in-raspberry-pi-with-c-or-c
    // Open a file descriptor for the UART, use NDELAY for nonblocking file descriptor,
    // use O_NOCTTY to ensure this terminal device does not become the controlling terminal for
    // this process.
    serial_fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
    if(serial_fd < 0 || !isatty(serial_fd)) {
        perror("Problem opening termios fd");
    }

    // Flush the port buffer
    usleep(250000);               
    tcflush(serial_fd, TCIOFLUSH); 

    // Configure serial port for 8N1, no parity, no breaks
    // VMIN and VTIME are necessary for nonblocking reads
    struct termios serialSet;
    memset(&serialSet, 0, sizeof(serialSet));
    serialSet.c_iflag = IGNBRK;
    serialSet.c_cflag = CS8 | CREAD | CLOCAL;
    memset(serialSet.c_cc, _POSIX_VDISABLE, NCCS);
    serialSet.c_cc[VMIN] = 0;
    serialSet.c_cc[VTIME] = 0;

    // Set BAUD rate
    speed_t baudRate = B115200;
    cfsetispeed(&serialSet, baudRate);
    cfsetospeed(&serialSet, baudRate);

    if (tcsetattr(serial_fd, TCSANOW, &serialSet) == -1) {
        perror("Problem setting termios attributes");
    }


    return 0;
}