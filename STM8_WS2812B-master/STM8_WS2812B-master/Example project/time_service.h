#ifndef __TIME_SERVICE
#define __TIME_SERVICE

#include "stm8s.h"

typedef struct 
{
u32 start_time;
u32 expire_time;
}
timer_t;

void timer_set(timer_t* t, u32 interval_ms);
u8 timer_expired(timer_t* t);


#endif