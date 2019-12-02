#ifndef __WS2812B_FX
#define __WS2812B_FX
#include "stm8s.h"
#include "ws2812b_LLD.h"
#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

extern const RGBColor_t RED;
extern const RGBColor_t GREEN;
extern const RGBColor_t BLUE;
extern const RGBColor_t BLACK;
extern const RGBColor_t WHITE;
extern const RGBColor_t YELLOW;
extern const RGBColor_t MAGENTA;

extern const RGBColor_t * pure_colors[];
RGBColor_t Wheel(unsigned char WheelPos);
RGBColor_t get_next_color();
void breathe_setup(u16 d, u8 flag, u8 s);
void display_color(RGBColor_t c);
void animation_set_state(u8 state);
void animation_next();
#endif