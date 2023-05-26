#ifndef __TSYNC_H__
#define __TSYNC_H__

typedef struct mutex *p_mutex;

#ifdef __cplusplus
extern "C"
#endif
p_mutex mutex_new();

#ifdef __cplusplus
extern "C"
#endif
void mutex_lock(p_mutex mtx);

#ifdef __cplusplus
extern "C"
#endif
void mutex_unlock(p_mutex mtx);

#ifdef __cplusplus
extern "C"
#endif
void sleep_thread(int ms);

#endif