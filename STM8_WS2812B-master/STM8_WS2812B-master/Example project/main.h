#ifndef __MAIN
#define __MAIN

#define BUTTON_port             GPIOD
#define BUTTON_pin              GPIO_PIN_4
#define BUTTON_EN_port          GPIOD
#define BUTTON_EN_pin           GPIO_PIN_5

#define BUTTON_ENABLE()   (BUTTON_EN_port->ODR |= (uint8_t)BUTTON_EN_pin)
#define BUTTON_DISABLE()   (BUTTON_EN_port->ODR &= ~(uint8_t)BUTTON_EN_pin)

#define POWER_OFF_DELAY_S       180

#endif