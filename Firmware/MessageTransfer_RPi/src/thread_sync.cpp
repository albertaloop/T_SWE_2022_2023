#include <mutex>
#include <chrono>
#include <thread>

#include "thread_sync.h"

#define NUM_MUTEX 1

typedef struct mutex {
    std::mutex mtx;
} mutex;

struct mutex mtx_array[NUM_MUTEX];
int mtx_count = 0;

#ifdef __cplusplus
extern "C"
#endif
p_mutex mutex_new() {
    p_mutex new_mutex;
    if(mtx_count < NUM_MUTEX) {
        new_mutex = &mtx_array[mtx_count];
        mtx_count++;
    }
    return new_mutex;
}

#ifdef __cplusplus
extern "C"
#endif
void mutex_lock(p_mutex mtx_) {
    mtx_->mtx.lock();
}

#ifdef __cplusplus
extern "C"
#endif
void mutex_unlock(p_mutex mtx_) {
    mtx_->mtx.unlock();
}

#ifdef __cplusplus
extern "C"
#endif
void sleep_thread(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}