#ifndef __BUTTON_SERVICE
#define __BUTTON_SERVICE

#include "stm8s_gpio.h"
#include "time_service.h"

typedef struct
{
GPIO_TypeDef* port; 
GPIO_Pin_TypeDef pin;
u8 state;
timer_t t;
}button_t;

char button_service(button_t *b, struct pt *pt);

#endif