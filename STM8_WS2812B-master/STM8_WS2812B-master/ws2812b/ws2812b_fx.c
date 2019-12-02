#include "ws2812b_fx.h"
#include "pt.h"

#include "time_service.h"
#include <stdlib.h>


const RGBColor_t RED            = {255,0,0};
const RGBColor_t GREEN          = {0,255,0};
const RGBColor_t BLUE           = {0,0,255};
const RGBColor_t YELLOW         = {255,255,0};
const RGBColor_t MAGENTA        = {255,0,255};
const RGBColor_t BLACK          = {0,0,0};
const RGBColor_t WHITE          = {255,255,255};

const RGBColor_t * pure_colors[] = {&RED, &GREEN, &BLUE, &YELLOW, &MAGENTA, &WHITE};


RGBColor_t get_next_color()
{
static u8 index = 0;
RGBColor_t t = *pure_colors[index++];
if (index == ARRAY_LENGTH(pure_colors)) index = 0;
return t;
}

RGBColor_t Wheel(unsigned char WheelPos) 
{
  WheelPos = 255 - WheelPos;
  RGBColor_t color;
  if(WheelPos < 85) 
  {
    color.R = 255 - WheelPos * 3;
    color.G = 0;
    color.B = WheelPos * 3;
  return color;
  }
  if(WheelPos < 170) 
  {
    WheelPos -= 85;
    color.R = 0;
    color.G = WheelPos * 3;
    color.B = 255 - WheelPos * 3;
  return color;
  }
  
  WheelPos -= 170;
  color.R = WheelPos * 3; 
  color.G = 255 - WheelPos * 3;
  color.B = 0;
  return color;
}



static u8 animation_delay = 15;
static timer_t t = {0};
RGBColor_t animation_color = {0};
char rainbowCycle(struct pt *pt)
{
  PT_BEGIN(pt);
  
  static u16 n = 0;
  
  while(1){
  while(n++ < 256)
  {
    for(u8 i=0; i < NB_LEDS; i++) 
    {
    rgb_SetColor(i,Wheel(((i * 256 / 10) + n) & 255));
    }
    rgb_SendArray();
    timer_set(&t,animation_delay);
    PT_WAIT_UNTIL(pt, timer_expired(&t));
  }
  n=0;
  }
  PT_END(pt);
}


static u8 brightness = 128; //max brightness
static u8 change_color_flag = 0;
static u8 step = 1;

void breathe_setup(u16 d, u8 flag, u8 s)
{
step = s;
animation_delay = d;
change_color_flag=flag;
}

char breathe(struct pt *pt)
{
  PT_BEGIN(pt);
  RGBColor_t c;
  while(1)
  {
    while(brightness > 0)
    { 
      if (brightness < step) {brightness = 0;}
      else {brightness -= step;}
      c.B = (animation_color.B * brightness) >> 7;
      c.R = (animation_color.R * brightness) >> 7;
      c.G = (animation_color.G * brightness) >> 7;
      
      for(u8 i=0; i < NB_LEDS; i++) 
        {
          rgb_SetColor(i,c);
        }
      rgb_SendArray();
      timer_set(&t,animation_delay);
      PT_WAIT_UNTIL(pt, timer_expired(&t));
    }
    
    timer_set(&t,50);
    PT_WAIT_UNTIL(pt, timer_expired(&t));
    if (change_color_flag)
      {
        animation_color = get_next_color();
      }
    while(brightness < 128)
    { 
      if (brightness + step > 128) {brightness = 128;}
      else {brightness += step;}
      c.B = (animation_color.B * brightness) >> 7;
      c.R = (animation_color.R * brightness) >> 7;
      c.G = (animation_color.G * brightness) >> 7;
      
      for(u8 i=0; i < NB_LEDS; i++) 
        {
        rgb_SetColor(i,c);
       }
      rgb_SendArray();
      timer_set(&t,animation_delay);
      PT_WAIT_UNTIL(pt, timer_expired(&t));
    }
    
    timer_set(&t,50);
    PT_WAIT_UNTIL(pt, timer_expired(&t));
  }
PT_END(pt);
}



static u8 animation_running_flag = 1;
static struct pt anim_pt;
static u8 index = 0;

void animation(void)
{
  if(!animation_running_flag) return;
  switch (index)
  {
  case 0:{
          rainbowCycle(&anim_pt);
          break;
         }
  case 1:
        {
         breathe(&anim_pt);
         break;
        }
  case 2:
        {
         breathe(&anim_pt);
         break;
        }
  case 3:
        {
         breathe(&anim_pt);
         break;
        }
  }
}

void animation_next()
{
  if(index++ > 3) index = 0;
  if(index == 0)
    {
    breathe_setup(15, 0, 1); // single color slow breathe animation
    } 
    
  if(index == 1) // single color slow breathe animation
    {
    breathe_setup(15, 0, 1);
    }
  if(index == 2) // multi color slow breathe animation
    {
    breathe_setup(15, 1, 1);
    }
  if(index == 3) // epileptic breathe animation
    {
    breathe_setup(3, 1, 10);
    }
}

void animation_set_state(u8 state)
{
  if(state)
  {
  animation_running_flag = 1;
  PT_INIT(&anim_pt);
  }
  else 
  {
  animation_running_flag = 0;
  }
}


void display_color(RGBColor_t c)
{
    for(u8 i=0; i < NB_LEDS; i++) 
    {
    rgb_SetColor(i, c);
    }
    rgb_SendArray();
}
