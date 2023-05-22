#ifndef __UART_H__
#define __UART_H__

#include "priority_queue.h"
#include "thread_sync.h"

void init_uart(struct prio_queue *pq_, p_mutex mtx_);

void uart_thread_fn();

#endif