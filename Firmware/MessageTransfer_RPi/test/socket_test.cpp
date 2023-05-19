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

#include "../include/udp_messages.h"
#include "../include/data_model.h"

#define TLM_BCAST_DELAY 200
#define CMD_PACKET_LEN 3

extern int errno;
int cmd_fd;
int tlm_fd;
struct addrinfo *server_addr, *tlm_addr, hints;
int current_cmd;


static int is_valid_cmd(char read[CMD_PACKET_LEN]) {
    if(read[0] == CMD_MSG) {
        if(read[1] == CMD_ESTOP || CMD_PREP || CMD_CRAWL || CMD_LAUNCH || CMD_HEALTHCHK) {
            return 1;
        }
    }
    return 0;
}

static int cmd_ack(int cmd) {
    if(cmd == CMD_ESTOP) {
        return ACK_ESTOP;
    } else if(cmd == CMD_CRAWL) {
        return ACK_CRAWL;
    } else if(cmd == CMD_PREP) {
        return ACK_PREP;
    } else if(cmd == CMD_LAUNCH) {
        return ACK_LAUNCH;
    } else if(cmd == CMD_HEALTHCHK) {
        return ACK_HEALTHCHK;
    }
    return -1;
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

static void udp_cmd_thread_fn() {
    fd_set reads;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(cmd_fd, &master);
    int max_socket = cmd_fd;
    struct sockaddr_storage client_addr;
    socklen_t client_len = sizeof(client_addr);
    while(1) {
        reads = master;
        if(select(max_socket+1, &reads, 0, 0, 0) < 0) {
            perror("Select error");
        }
        if(FD_ISSET(cmd_fd, &reads)) {
            char read[CMD_PACKET_LEN];
            int bytes;
            if((bytes = recvfrom(cmd_fd, read, CMD_PACKET_LEN, 0, (struct sockaddr *)&client_addr, &client_len)) < 1) {
                perror("recvfrom error");
            }
            if(is_valid_cmd(read)) {
                char reply[CMD_PACKET_LEN];
                reply[0] = ACK_MSG;
                reply[1] = cmd_ack(read[1]);
                sendto(cmd_fd, reply, CMD_PACKET_LEN, 0, (struct sockaddr *)&client_addr, client_len);
            }
        }
    }
}

static void udp_tlm_thread_fn() {
    printf("telem size: %d\n", sizeof(telemetry));
    memset(&telemetry, 1, sizeof(telemetry));
    while(1) {
        sendto(tlm_fd, &telemetry, sizeof(telemetry), 0, server_addr->ai_addr, server_addr->ai_addrlen);
        std::this_thread::sleep_for(std::chrono::milliseconds(TLM_BCAST_DELAY));
    }


}

int main() {

    init_socket();

    std::thread udp_cmd_thread(udp_cmd_thread_fn);
    std::thread udp_tlm_thread(udp_tlm_thread_fn);
    
    udp_cmd_thread.join();
    udp_tlm_thread.join();

    close(cmd_fd);
    close(tlm_fd);

    return 0;
}