#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "data_model.h"
#include "udp.h"
#include "chrono_timer.h"
#include "thread_sync.h"

char *gui_address = "192.168.1.11";
char *rpi_address = "192.168.1.10";
int cmd_fd;
int tlm_fd;
struct addrinfo *server_addr, *tlm_addr, hints;

p_chrono_timer cmd_timer;
p_mutex udp_mtx;
struct prio_queue *udp_pq;

int last_cmd = 0;

struct telemetry{
    uint8_t team_id;
    uint8_t status;
    int32_t accel;
    int32_t position;
    int32_t velocity;
    int32_t batt1_v;
    int32_t batt1_c;
    int32_t batt1_temp;
    int32_t pod_temp;
} telemetry;

void init_socket(struct prio_queue *pq_, p_mutex mtx_) {

    udp_pq = pq_;
    udp_mtx = mtx_;

    cmd_timer = chrono_timer_new();
    start_timer(cmd_timer);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    if(getaddrinfo(rpi_address, "3000", &hints, &server_addr) < 0) {
        perror("Getaddrinfo error");
    }
    cmd_fd = socket(server_addr->ai_family, server_addr->ai_socktype, server_addr->ai_protocol);
    if(bind(cmd_fd, server_addr->ai_addr, server_addr->ai_addrlen) != 0) {
        perror("Bind error");
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if(getaddrinfo(gui_address, "4000", &hints, &tlm_addr) < 0) {
        perror("Getaddrinfo error");
    }
    tlm_fd = socket(tlm_addr->ai_family, tlm_addr->ai_socktype, 0);
}

void udp_cmd_thread_fn() {
    fd_set reads;
    fd_set master;
    FD_ZERO(&master);
    FD_SET(cmd_fd, &master);
    int max_socket = cmd_fd;
    struct sockaddr_storage client_addr;
    socklen_t client_len = sizeof(client_addr);
    while(1) {
        reads = master;
        if(select(max_socket+1, &reads, 0, 0, NULL) < 0) {
            perror("Select error");
        }
        printf("select ready\n");
        if(FD_ISSET(cmd_fd, &reads)) {
            printf("reading cmd socket\n");
            char read[CMD_PACKET_LEN];
            memset(read, 0, sizeof(read));
            int bytes;
            if((bytes = recvfrom(cmd_fd, read, CMD_PACKET_LEN, 0, (struct sockaddr *)&client_addr, &client_len)) < 1) {
                perror("recvfrom error");
            }
            printf("read %x", (uint8_t)read[0]);
            printf("%x\n", (uint8_t)read[1]);
            if(is_valid_cmd(read)) {
                printf("cmd valid\n");
                stop_timer(cmd_timer);
                double duration = get_duration(cmd_timer);
                if(read[1] != last_cmd && duration > CMD_PROCESS_TIME) {
                    start_timer(cmd_timer);
                    // Put received command in priority queue
                    struct msg tmp = {get_cmd_prio((uint8_t*)read), get_cmd_int(read)};
                    mutex_lock(udp_mtx);
                    push_msg(udp_pq, tmp);
                    mutex_unlock(udp_mtx);
                }
                char reply[CMD_PACKET_LEN];
                reply[0] = ACK_MSG;
                reply[1] = cmd_ack(read[1]);
                sendto(cmd_fd, reply, CMD_PACKET_LEN, 0, (struct sockaddr *)&client_addr, client_len);
            }
        }
    }
    close(cmd_fd);
}


void udp_tlm_thread_fn() {
    printf("telem size: %ld\n", sizeof(telemetry));
    memset(&telemetry, 1, sizeof(telemetry));
    while(1) {
        // printf("broadcasting telemetry\n");
        sendto(tlm_fd, &telemetry, 30, 0, tlm_addr->ai_addr, tlm_addr->ai_addrlen);
        sleep_thread(TLM_BCAST_DELAY);
    }
    close(tlm_fd);
}