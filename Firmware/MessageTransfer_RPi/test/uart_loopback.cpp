#include <termios.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <thread>
// #include "../include/messages.h"

#define UART_TIMEOUT 100

int serial_fd;
int errnum;
extern int errno;



static void init_uart() {
    //https://raspberrypi.stackexchange.com/questions/57906/uart-interrupt-in-raspberry-pi-with-c-or-c
    // Open a file descriptor for the UART, use NDELAY for nonblocking file descriptor,
    // use O_NOCTTY to ensure this terminal device does not become the controlling terminal for
    // this process.

    // Could also use ttyS0
    serial_fd = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if(serial_fd < 0 || !isatty(serial_fd)) {
        perror("Problem opening termios fd");
        exit(1);
    }

    // Need to flush the port buffer before setting the UART settings
    usleep(250000);
    // TCIOFLUSH -> clear input buffer   
    // tcflush(serial_fd, TCIOFLUSH); 

    // Configure serial port for 8N1, no parity, no breaks
    // VMIN and VTIME are necessary for nonblocking reads
    struct termios serialSet;
    memset(&serialSet, 0, sizeof(serialSet));
    // tcgetattr(serial_fd, &serialSet);
    // serialSet.c_iflag = IGNBRK;
    serialSet.c_iflag = 0;
    serialSet.c_oflag = 0;
    serialSet.c_lflag = 0;
    // seerialSet.c_iflag = IGNPAR;
    // CS8 -> 8 bits, CREAD -> Enable receiver, CLOCAL -> Ignore modem status
    // Could also set BAUD rate here eg. B9600
    serialSet.c_cflag = CS8 | CREAD | CLOCAL;
    // memset(serialSet.c_cc, _POSIX_VDISABLE, NCCS);
    serialSet.c_cc[VMIN] = 1;
    serialSet.c_cc[VTIME] = 5;

    // Set BAUD rate
    speed_t baudRate = B9600;
    cfsetispeed(&serialSet, baudRate);
    cfsetospeed(&serialSet, baudRate);
    // tcflush(serial_fd, TCIOFLUSH); 


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
    char text[255];
    // memset(text, 0, 255);
    usleep(20000);
    tcflush(serial_fd, TCIOFLUSH); 
    usleep(20000);
    ioctl(serial_fd, TCFLSH, 2);
    read(serial_fd, text, 255);
    printf("%s\n", text);
    // memset(text, 0, 255);
    read(serial_fd, text, 255);
    printf("%s\n", text);
    usleep(20000);
    tcflush(serial_fd, TCIOFLUSH); 
    while(1) {
        // Send a message
        usleep(20000);
        ioctl(serial_fd, TCFLSH, 2);
        // tcflush(serial_fd, TCIOFLUSH); 
        printf("Sending UART message\n");
        usleep(20000);
        int status = write(serial_fd, "TMTRY", strlen("TLMTRY"));
        if(status < 0) {
            printf("Write failure\n");
        } else if (status == 0) {
            printf("No write\n");
            errnum = errno;
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("Error printed by perror");
            fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        }
        // Wait UART_TIMEOUT ms for acknowledgement
        // Resend message if timeout
        bool success = false;
        // https://stackoverflow.com/questions/67644306/how-can-we-determine-whether-a-socket-is-ready-to-read-write
        while(!success) {
            printf("Checking for UART reply\n");
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
        usleep(2500000);
        printf("Reading UART reply\n");
        status = read(serial_fd, text, 255);
        if(status < 0) {
            printf("Read failure\n");
            errnum = errno;
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("Error printed by perror");
            fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        } else if (status == 0) {
            printf("No read\n");
            errnum = errno;
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("Error printed by perror");
            fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        }
        printf("%s\n", text);
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