#include "time_service.h"

volatile u32 uptime = 0;

void uptime_routine(void)
{
 uptime++;
 if(uptime == 0xFFFFFFFF)
 {
 uptime  = 0;
 }
}

u32 get_uptime()
{
return uptime;
}


void timer_set(timer_t* t, u32 interval)
{
t->expire_time = get_uptime() + interval;
}

u8 timer_expired(timer_t* t)
{
if (get_uptime() > t->expire_time) return 1;
return 0;
}
