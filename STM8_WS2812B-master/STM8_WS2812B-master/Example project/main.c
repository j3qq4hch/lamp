#include "stm8s_conf.h"
#include "ws2812B_fx.h"
#include "pt.h"
#include "main.h"
#include "time_service.h"
#include "button_service.h"

void clk_config(void);
void TIM4_Config(void);
void animation(void);

static struct pt pt1;
static struct pt pt2;
BitStatus get_button_state(button_t *b);
u8 pk_reset_flag = 0;
u8 low_power_mode_flag = 0;
static timer_t t = {0};

button_t b = 
{
BUTTON_port,
BUTTON_pin,
0,
{0,0}
};


extern RGBColor_t animation_color;
char rainbowCycle(struct pt *pt);
char color_breathe(struct pt *pt);
char breathe(struct pt *pt);
char epileptic(struct pt *pt);
void low_power_on();
void low_power_off();
char power_keeper(struct pt *pt);

void TIM4_Config(void)
{
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  TIM4_Cmd(ENABLE);
}


void main( void )
{  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //clock config
  TIM4_Config();
  GPIO_Init (GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init (BUTTON_EN_port, BUTTON_EN_pin, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init (BUTTON_port, BUTTON_pin, GPIO_MODE_IN_FL_NO_IT);
  AWU_Init(AWU_TIMEBASE_1S);
  
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  enableInterrupts();
  BUTTON_ENABLE();
   
  while(1)
  {      
     while(!low_power_mode_flag)
      { 
      animation();
      button_service(&b, &pt2);
  //    power_keeper(&pt1);
      }
      
      display_color(BLACK);
     
    while(low_power_mode_flag)
      {
       low_power_on();
       halt();
       low_power_off();
       timer_set(&t,200);
       while((!timer_expired(&t)) && (get_button_state(&b)==RESET)){}
       
       if(timer_expired(&t)){continue;}
       low_power_mode_flag = 0;
      }
  }
}

///////////////////////////////////////

BitStatus get_button_state(button_t *b)
{
return GPIO_ReadInputPin(b->port, b->pin);
}

void button_click_cb()
{
    animation_set_state(0);
    display_color(get_next_color());
    pk_reset_flag = 1;
}

void button_double_click_cb()
{
animation_set_state(1);
animation_next();
pk_reset_flag = 1;
}

void button_long_press_cb()
{
low_power_mode_flag = 1;
}


void low_power_on()
{
  CLK->PCKENR1 = 0;               
  CLK->PCKENR2 = CLK_PCKENR2_AWU;                //enable clock for AWU only
 // BUTTON_DISABLE();
}

void low_power_off()
{
  CLK->PCKENR1 = CLK_PCKENR1_TIM4;      ////enable clock for TIMNonly         
  CLK->PCKENR2 = 0;                     
//  BUTTON_ENABLE();
}

timer_t pk_timer={0};

char power_keeper(struct pt *pt)
{
PT_BEGIN(pt);  

while(1)
{
  u32 interval = POWER_OFF_DELAY_S * 1000;
  timer_set(&pk_timer,interval);
  
  PT_WAIT_UNTIL(pt, timer_expired(&pk_timer)||pk_reset_flag);
  if(pk_reset_flag)
  {
  pk_reset_flag = 0;
  continue;
  }
  else
  {
  low_power_mode_flag = 1;
  }
  
  }
PT_END(pt);
}
