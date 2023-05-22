#include <chrono>

#include "chrono_timer.h"

#define NUM_TIMER 2

typedef struct chrono_timer {
  std::chrono::time_point<std::chrono::system_clock> start_time;
  std::chrono::time_point<std::chrono::system_clock> stop_time;
} chrono_timer;

struct chrono_timer timer_array[NUM_TIMER];
int timer_count = 0;

#ifdef __cplusplus
extern "C"
#endif
p_chrono_timer chrono_timer_new() {
    p_chrono_timer new_timer;
    if(timer_count < NUM_TIMER) {
        new_timer = &timer_array[timer_count];
        timer_count++;
    }
    return new_timer;
}

#ifdef __cplusplus
extern "C"
#endif
void start_timer(p_chrono_timer ct) {
    ct->start_time = std::chrono::system_clock::now();
}

#ifdef __cplusplus
extern "C"
#endif
void stop_timer(p_chrono_timer ct) {
    ct->stop_time = std::chrono::system_clock::now();
}

#ifdef __cplusplus
extern "C"
#endif
double get_duration(p_chrono_timer ct) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                ct->stop_time - ct->start_time).count();
}

