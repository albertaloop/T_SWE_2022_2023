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

#include <thread>
#include <mutex>
#include <chrono>

extern "C" {
    #include "priority_queue.h"
    #include "uart.h"
    #include "udp.h"
}

#include "chrono_timer.h"
#include "thread_sync.h"

struct prio_queue p_queue;

p_mutex mtx;

int main() {

    mtx = mutex_new();

    init_pqueue(&p_queue);
    init_uart(&p_queue, mtx);
    init_socket(&p_queue, mtx);

    std::thread uart_thread(uart_thread_fn);
    std::thread udp_cmd_thread(udp_cmd_thread_fn);
    std::thread udp_tlm_thread(udp_tlm_thread_fn);
    
    uart_thread.join();
    udp_cmd_thread.join();
    udp_tlm_thread.join();


    return 0;
}
