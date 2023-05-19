#include <termios.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netdb.h>

#include <thread>
#include <chrono>
// #include "../include/messages.h"

#define UART_TIMEOUT 100

int serial_fd;
int errnum;
extern int errno;
int cmd_fd;
int tlm_fd;
struct addrinfo *server_addr, *tlm_addr, hints;


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


static void init_socket() {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    
    if(getaddrinfo(0, "3000", &hints, &server_addr) < 0) {
        perror("Getaddrinfo error");
    }

    cmd_fd = socket(server_addr->ai_family, server_addr->ai_socktype, 0);

    if(bind(cmd_fd, server_addr->ai_addr, server_addr->ai_addrlen) != 0) {
        perror("Bind error");
    }

    if(getaddrinfo(0, "4000", &hints, &tlm_addr) < 0) {
        perror("Getaddrinfo error");
    }

    tlm_fd = socket(tlm_addr->ai_family, server_addr->ai_socktype, 0);




}

static void uart_thread_fn() {
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
}

static void udp_cmd_thread_fn() {
    fd_set reads;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(cmd_fd, &master);
    int max_socket = cmd_fd;
    while(1) {
        reads = master;
        if(select(max_socket+1, &reads, 0, 0, 0) < 0) {
            perror("Select error");
        }
        if(FD_ISSET(cmd_fd, &reads)) {
            char read[1024];
            int bytes;
            if((bytes = recvfrom(cmd_fd, read, 1024, 0, (struct sockaddr *)&client_addr, &client_len)) < 1) {
                perror("recvfrom error");
            }
            
        }
    }
}

static void udp_tlm_thread_fn() {
    sendto(tlm_fd, tlm_buf, 1024, 0, (struct sockaddr *)tlm_addr, &tlm_addr_len)
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

}

int main() {
    init_uart();
    init_socket();

    std::thread uart_thread(uart_thread_fn);
    std::thread udp_cmd_thread(udp_cmd_thread_fn);
    std::thread udp_tlm_thread(udp_tlm_thread_fn);
    
    uart_thread.join();
    udp_cmd_thread.join();
    udp_tlm_thread.join();

    close(cmd_fd);
    close(tlm_fd);
    close(serial_fd);
    return 0;
}

