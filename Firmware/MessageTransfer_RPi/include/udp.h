#ifndef __UDP_H__
#define __UDP_H__

#include "priority_queue.h"
#include "thread_sync.h"

void init_socket(struct prio_queue *pq_, p_mutex mtx_);

void udp_cmd_thread_fn();

void udp_tlm_thread_fn();

#endif