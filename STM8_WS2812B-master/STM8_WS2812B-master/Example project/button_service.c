#include "pt.h"
#include "button_service.h"

#define LONG_PRESS_DURATION_MS  2000
#define DEBOUNCE_PERIOD_MS      0
#define DOUBLE_CLICK_PAUSE      250


void button_click_cb();
void button_double_click_cb();
void button_long_press_cb();
BitStatus get_button_state(button_t *b);



char button_service(button_t *b, struct pt *pt)
{
  PT_BEGIN(pt);
  while(1)
  {
    PT_WAIT_UNTIL(pt, get_button_state(b)!= RESET);
    timer_set(&b->t,DEBOUNCE_PERIOD_MS);
    PT_WAIT_UNTIL(pt, timer_expired(&b->t));
    if(get_button_state(b) == RESET) continue; // this means it was just bouncing, so no action needed
    
    timer_set(&b->t, LONG_PRESS_DURATION_MS);
    PT_WAIT_UNTIL(pt, timer_expired(&b->t)||get_button_state(b)==RESET);
    if(timer_expired(&b->t))  // Long press detected. Will serve it and them wait until button is released before any further actions
    {
      button_long_press_cb();
      PT_WAIT_UNTIL(pt, get_button_state(b)==RESET);
      continue;
    }
    timer_set(&b->t, DOUBLE_CLICK_PAUSE);
    PT_WAIT_UNTIL(pt, timer_expired(&b->t)||get_button_state(b)!=RESET);
    if(timer_expired(&b->t))  // Double click detected
    {
      button_click_cb();
      continue;
    }
    button_double_click_cb();
    PT_WAIT_UNTIL(pt, get_button_state(b)==RESET);
    }
  PT_END(pt);
}