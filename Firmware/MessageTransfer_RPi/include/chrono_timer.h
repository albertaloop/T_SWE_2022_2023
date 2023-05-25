#ifndef __CHRONO_TIM_H__
#define __CHRONO_TIM_H__


typedef struct chrono_timer *p_chrono_timer;

#ifdef __cplusplus
extern "C"
#endif
p_chrono_timer chrono_timer_new();

#ifdef __cplusplus
extern "C"
#endif
void start_timer(p_chrono_timer ct);

#ifdef __cplusplus
extern "C"
#endif
void stop_timer(p_chrono_timer ct);

#ifdef __cplusplus
extern "C"
#endif
double get_duration(p_chrono_timer ct);

#endif