#include <termios.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "messages.h"

#define UART_TIMEOUT 100

int serial_fd;

void pollUart() {
    struct pollfd src;
    src.fd = serial_fd;
    src.events = POLLIN;
    src.revents = 0;   

    int check = poll(&src, 1, -1);
}

static int read_uart(int uart_fd, void *data, size_t count) {
    return 
}

static int write_uart(int uart_fd, void *data, size_t count) {
    return write(uart_fd, data, count);
}

static int init_uart() {
    //https://raspberrypi.stackexchange.com/questions/57906/uart-interrupt-in-raspberry-pi-with-c-or-c
    // Open a file descriptor for the UART, use NDELAY for nonblocking file descriptor,
    // use O_NOCTTY to ensure this terminal device does not become the controlling terminal for
    // this process.

    // Could also use ttyS0
    serial_fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
    if(serial_fd < 0 || !isatty(serial_fd)) {
        perror("Problem opening termios fd");
        exit(1);
    }

    // Need to flush the port buffer before setting the UART settings
    usleep(250000);
    // TCIOFLUSH -> clear input buffer   
    tcflush(serial_fd, TCIOFLUSH); 

    // Configure serial port for 8N1, no parity, no breaks
    // VMIN and VTIME are necessary for nonblocking reads
    struct termios serialSet;
    memset(&serialSet, 0, sizeof(serialSet));
    serialSet.c_iflag = IGNBRK;
    // seerialSet.c_iflag = IGNPAR;
    // CS8 -> 8 bits, CREAD -> Enable receiver, CLOCAL -> Ignore modem status
    // Could also set BAUD rate here eg. B9600
    serialSet.c_cflag = CS8 | CREAD | CLOCAL;
    memset(serialSet.c_cc, _POSIX_VDISABLE, NCCS);
    serialSet.c_cc[VMIN] = 0;
    serialSet.c_cc[VTIME] = 0;

    // Set BAUD rate
    speed_t baudRate = B115200;
    cfsetispeed(&serialSet, baudRate);
    cfsetospeed(&serialSet, baudRate);

    tcgetattr(serial_fd, &serialSet);

    // TCSANOW -> apply the settings immediately
    if (tcsetattr(serial_fd, TCSANOW, &serialSet) == -1) {
        perror("Problem setting termios attributes");
    }
}


int main() {
    init_uart();
    
    struct pollfd src;
    src.fd = serial_fd;
    src.events = POLLIN;
    src.revents = 0;

    while(1) {
        // Send a message
        write(serial_fd, "TLMTRY", strlen("TLMTRY"));
        // Wait UART_TIMEOUT ms for acknowledgement
        // Resend message if timeout
        bool success = false;
        // https://stackoverflow.com/questions/67644306/how-can-we-determine-whether-a-socket-is-ready-to-read-write
        while(!success) {
            if(poll(&src, 1, UART_TIMEOUT) < 0) {
                perror("Poll error");
                exit(1);
            }
            if(src.revents != 0) {
                if(src.revents & POLLIN) {
                    success = true;
                } else {
                    printf("UART error");
                }
            }
        }
        char text[255];
        read(serial_fd, text, 255);
        printf("%.*s\n", text, 255);
        // Read message
        // delay
        usleep(250);

    }

    // 

    // strcpy(text, TLMTRY);


    close(serial_fd);
    return 0;
}

/*
#include <termios.h>

#define NUM_FDS         1
#define SERIAL_FD_POS   0
#define UDP_FD_POS      1

static int serial_fd;
// static int udp_fd;

static struct termios serialSet;

static struct pollfd fds[NUM_FDS];



void setup_uart() {
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

    // Register serial file descriptor to be polled
    fds[SERIAL_FD_POS].fd = serial_fd;
    fds[SERIAL_FD_POS].events = POLLIN;
    fds[SERIAL_FD_POS].revents = 0;

}

void setup_udp() {

}

int main() {
    
    setup_uart();
    setup_udp();


    while(1) {
        if(poll(fds, NUM_FDS, -1) == 1) {


        }
    }

    return 0;
}*/