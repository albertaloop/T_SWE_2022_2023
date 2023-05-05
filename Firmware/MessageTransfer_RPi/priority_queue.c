#include "priority_queue.h"
#include <stdbool.h>



bool is_empty(struct queue *q) {
    if(q->count == 0) return true;
    else return false;
}

bool is_full(struct queue *q) {
    if(q->msg_in_prog) {
        if(q->count == q->size-2) return true;
    } else {
        if(q->count == q->size-1) return true;
    }
    return false;
}

bool push_msg(struct prio_queue *pq, struct msg m) {
    if(m.prio != PRIO_HIGH && m.prio != PRIO_MED && m.prio != PRIO_LOW)
        return false;
    if(!is_full(&(pq->queue_arr[m.prio]))) {
        pq->queue_arr[m.prio].msg_arr[pq->queue_arr[m.prio].count] = m;
        pq->queue_arr[m.prio].count++;
        return true;
    } else {
        return false;
    }
}

bool pop_msg(struct prio_queue *pq, struct msg *m) {
    if(!is_empty(&(pq->queue_arr[PRIO_HIGH]))) {
        *m = pq->queue_arr[PRIO_HIGH].msg_arr[0];
        pq->queue_arr[PRIO_HIGH].msg_in_prog = true;
        return queue_sl(&(pq->queue_arr[PRIO_HIGH]));
    } else if(!is_empty(&(pq->queue_arr[PRIO_MED]))) {
        *m = pq->queue_arr[PRIO_MED].msg_arr[0];
        pq->queue_arr[PRIO_MED].msg_in_prog = true;
        return queue_sl(&(pq->queue_arr[PRIO_MED]));
    } else if(!is_empty(&(pq->queue_arr[PRIO_LOW]))) {
        *m = pq->queue_arr[PRIO_LOW].msg_arr[0];
        pq->queue_arr[PRIO_LOW].msg_in_prog = true;
        return queue_sl(&(pq->queue_arr[PRIO_LOW]));
    }
    // No message to pop
    return false;
}

bool check_prempt(struct prio_queue *pq, struct msg m) {
    if(m.prio == PRIO_HIGH) return false;
    if(m.prio == PRIO_MED) {
        if(!is_empty(&(pq->queue_arr[PRIO_HIGH]))) return true;
    }
    if(m.prio == PRIO_LOW) {
        if(!is_empty(&(pq->queue_arr[PRIO_HIGH])) &&
            !is_empty(&(pq->queue_arr[PRIO_MED]))
        ) return true;
    }
    return false;
}

bool insert_front(struct prio_queue *pq, struct msg m) {
    if(m.prio != PRIO_HIGH && m.prio != PRIO_MED && m.prio != PRIO_LOW)
        return false;
    if(!is_empty(&(pq->queue_arr[m.prio]))) {
        queue_sr(&(pq->queue_arr[m.prio]));
        pq->queue_arr[m.prio].msg_arr[0] = m;
        pq->queue_arr[m.prio].count++;
        return true;
    } else {
        return false;
    }
}

bool queue_sr(struct queue *q) {
    if(!is_empty(q)) {
        for(int i=0; i < q->count; i++) {
            q->msg_arr[i] = q->msg_arr[i+1];
        }
        q->count++;
        return true;
    }
    return false;
}

bool queue_sl(struct queue *q) {
    if(!is_full(q)) {
        for(int i=0; i < q->count; i++) {
            q->msg_arr[i+1] = q->msg_arr[i];
        }
        q->count--;
        return true;
    }
    return false;
}