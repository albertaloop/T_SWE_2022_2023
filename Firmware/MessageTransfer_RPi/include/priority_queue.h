#ifndef _PRIO_Q_
#define _PRIO_Q_

#include <stdbool.h>

#define NUM_PRIO 3
#define QUEUE_SIZE 10
#define PRIO_HIGH 0
#define PRIO_MED 1
#define PRIO_LOW 2

typedef struct msg {
    int prio;
    int val;
} msg;

typedef struct queue {
    struct msg msg_arr[QUEUE_SIZE];
    int count;
    int size;
    bool msg_in_prog;
} queue;

typedef struct prio_queue {
    struct queue queue_arr[NUM_PRIO];
} prio_queue;

bool is_empty(struct queue *q);

bool is_full(struct queue *q);

bool push_msg(struct prio_queue *pq, struct msg m);

bool pop_msg(struct prio_queue *pq, struct msg *m);

bool check_prempt(struct prio_queue *pq, struct msg m);

bool insert_front(struct prio_queue *pq, struct msg m);

bool queue_sl(struct queue *q);

bool queue_sr(struct queue *q);

void msg_complete(struct prio_queue *pq, int prio);

#endif