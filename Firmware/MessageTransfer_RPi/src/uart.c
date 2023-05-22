#include <errno.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <termios.h>
#include <poll.h>
// #include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "data_model.h"
#include "uart.h"
#include "priority_queue.h"
#include "chrono_timer.h"
#include "thread_sync.h"

int serial_fd;
int errnum;

extern int errno;

p_chrono_timer telemetry_timer;
p_mutex uart_mtx;
struct prio_queue *uart_pq;

void init_uart(struct prio_queue *pq_, p_mutex mtx_) {

    uart_pq = pq_;
    uart_mtx = mtx_;

    telemetry_timer = chrono_timer_new();
    start_timer(telemetry_timer);
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

void uart_thread_fn() {
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
        // Obtain message
        struct msg cmd;
        char cmd_bytes[CMD_PACKET_LEN];
        while(1) {
            stop_timer(telemetry_timer);
            double duration = get_duration(telemetry_timer);
            mutex_lock(uart_mtx);
            if(pop_msg(uart_pq, &cmd)) {
            mutex_unlock(uart_mtx);
                printf("Sending command\n");
                get_cmd_char(cmd.val, cmd_bytes);
                break;
            }
            mutex_unlock(uart_mtx);

            if(duration >= TLM_BCAST_DELAY) {
                printf("Sending telemetry request\n");
                start_timer(telemetry_timer);
                cmd_bytes[0] = (char)0xDD;
                cmd_bytes[1] = (char)0xDD;
                break;
            }
        }

        // Send a message
        usleep(20000);
        ioctl(serial_fd, TCFLSH, 2);
        // tcflush(serial_fd, TCIOFLUSH); 
        usleep(20000);
        printf("command %x", cmd_bytes[0]);
        printf("%x\n", cmd_bytes[1]);
        int status = write(serial_fd, cmd_bytes, CMD_PACKET_LEN);
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
        status = read(serial_fd, text, CMD_PACKET_LEN);
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
        printf("%x", text[0]);
        printf("%x\n", text[1]);
        // Read message
        // delay
        usleep(250);
    }
    close(serial_fd);
}
