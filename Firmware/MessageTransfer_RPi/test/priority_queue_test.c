#include <check.h>
#include <stdio.h>
#include "priority_queue.h"

int main() {
    // 1. Create queue of size 10
    struct prio_queue p;

    init_pqueue(&p);

    // 2. Instert to queue, remove from queue
    struct msg msg_arr[7];
    msg_arr[0].prio = PRIO_LOW;
    msg_arr[0].val = 4;
    msg_arr[1].prio = PRIO_LOW;
    msg_arr[1].val = 5;
    msg_arr[2].prio = PRIO_LOW;
    msg_arr[2].val = 6;
    msg_arr[3].prio = PRIO_HIGH;
    msg_arr[3].val = 0;
    msg_arr[4].prio = PRIO_HIGH;
    msg_arr[4].val = 1;
    msg_arr[5].prio = PRIO_MED;
    msg_arr[5].val = 2;
    msg_arr[6].prio = PRIO_MED;
    msg_arr[6].val = 3;


    // 3. Insert until full
    for(int i = 0; i < 10; i++) {
        if(!push_msg(&p, msg_arr[0])) {
            printf("Push failed on attempt %d\n", i);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
    }
    printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));

    if(push_msg(&p, msg_arr[0])) {
        printf("Test failed. Inserted to full queue\n");
        printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
    }

    // 4. Remove until empty
    struct msg tmp;
    for(int i = 0; i < 10; i++) {
        if(!pop_msg(&p, &tmp)) {
            printf("Pop failed on attempt %d\n", i);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
        msg_complete(&p, tmp.prio);
    }

    if(pop_msg(&p, &tmp)) {
        printf("Test failed. Removed from empty queue\n");
        printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
    }


    // 5. Insert 3 low prios, 2 med prios, 2 high prios
    for(int i = 0; i < 7; i++) {
        if(!push_msg(&p, msg_arr[i])) {
            printf("Test 5 failed, push failed on attempt %d", i);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
        if(get_count(&p) != i+1) {
            printf("Test 5 failed, invalid count on attempt %d\n", i);
        }
    }



    // 6. Remove

    for(int i = 0; i < 2; i++) {
        if(!pop_msg(&p, &tmp)) {
            printf("Test 6 failed, push failed on attempt %d", i);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
        if(tmp.prio != PRIO_HIGH || tmp.val != i) {
            printf("Test 6 failed, wrong message popped");
            printf("Message obtained: priority = %d, value = %d\n", tmp.prio, tmp.val);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
    }

    for(int i = 2; i < 4; i++) {
        if(!pop_msg(&p, &tmp)) {
            printf("Test 6 failed, push failed on attempt %d", i);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
        if(tmp.prio != PRIO_MED || tmp.val != i) {
            printf("Test 6 failed, wrong message popped");
            printf("Message obtained: priority = %d, value = %d\n", tmp.prio, tmp.val);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
    }

    for(int i = 4; i < 7; i++) {
        if(!pop_msg(&p, &tmp)) {
            printf("Test 6 failed, push failed on attempt %d", i);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
        if(tmp.prio != PRIO_LOW || tmp.val != i) {
            printf("Test 6 failed, wrong message popped");
            printf("Message obtained: priority = %d, value = %d\n", tmp.prio, tmp.val);
            printf("Queue size = %d, Queue count = %d\n", QUEUE_SIZE, get_count(&p));
        }
    }


    // 7. Insert 2 low prio, remove 1 low prio, insert high prio, insert at front popped low prio
    // remove 3 messages -> order: high, 1st low prio, 2nd low prio

    push_msg(&p, msg_arr[0]);
    push_msg(&p, msg_arr[1]);
    pop_msg(&p, &tmp);
    push_msg(&p, msg_arr[3]);
    insert_front(&p, tmp);
    pop_msg(&p, &tmp);
    if(tmp.prio != msg_arr[3].prio || tmp.prio != msg_arr[3].val) {
        printf("Test failed, expected high prio msg to pop first\n");
    }
    pop_msg(&p, &tmp);
    if(tmp.prio != msg_arr[0].prio || tmp.val != msg_arr[0].val) {
        printf("Test failed, expected first low prio msg to pop\n");
        printf("Got msg: val = %d, prio = %d\n", tmp.val, tmp.prio);
        printf("Expected msg: val = %d, prio = %d\n", msg_arr[0].val, msg_arr[0].prio);
    }

    pop_msg(&p, &tmp);
    if(tmp.prio != msg_arr[1].prio || tmp.val != msg_arr[1].val) {
        printf("Test failed, expected second low prio msg to pop\n");
        printf("Got msg: val = %d, prio = %d\n", tmp.val, tmp.prio);
        printf("Expected msg: val = %d, prio = %d\n", msg_arr[1].val, msg_arr[1].prio);
    }


    return 0;
}