#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter_timer = 0 ;
int hook_id_timer;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /*To represent the value initially loaded to the timer,
  we have to limit the lowest and maximum
  value for freq, so that it can be represented in 16 bits*/
  if(freq < 19 || freq > TIMER_FREQ){
    printf("ERROR MESSAGE: Frequency input is not valid!");
    return -1;
  }
  uint8_t timer_config;
  if (timer_get_conf(timer, &timer_config) != 0) return -1; /*receives a control word about the con
                                          configuration of the timer*/
  /*Now we will preserve the 4 least significant bits
    of this configuration, and this is done by creating a mask*/
  
  uint8_t mask_to_preserve = BIT(3)| BIT(2) | BIT(1)| BIT(0);
  /*Now we have to create a control word with the 4 least significant
  bits of the timer configuration and the LSB followed by MSB*/
  uint8_t control_word = (mask_to_preserve & timer_config) | TIMER_LSB_MSB;
  /*Add the timer to this control word*/
  if(timer == 0){
    control_word = control_word | TIMER_SEL0;
  }
  else if(timer == 1){
    control_word = control_word | TIMER_SEL1;
  }
  else if(timer == 2){
    control_word = control_word | TIMER_SEL2;
  }
  else{
    printf("ERROR MESSAGE: Timer input is not valid!");
    return -1;
  }
  /*Now that we have to control word ready, we should send it to the
  timer control*/
  if (sys_outb(TIMER_CTRL, control_word) != 0) {
    printf("ERROR MESSAGE: Failure on sending control_word to TIMER_CTRL");
    return -1;
  }
  /*Now we should calculate the value initially loaded in the timer
  (denoted by div), div = clock(1193181Hz)/freq*/
  uint16_t div = (uint16_t)(TIMER_FREQ / freq);
  /*Since the time can only receive 8 bits, we have to convert 
  16 bits div into two parts, MSB and LSB*/

  uint8_t div_LSB;  /*variables with 8 bits*/
  uint8_t div_MSB;
  //Functions that make separate the 16 bit div into MSB and LSB
  if (util_get_LSB(div, &div_LSB) != 0){
    printf("ERROR MESSAGE: Failure on function util_get_LSB");
    return -1; 
  } 
  if (util_get_MSB(div, &div_MSB) != 0) {
    printf("ERROR MESSAGE: Failure on function util_get_MSB");
    return -1;
  }
  /*Functions that send the information of the div to the timer received
  in the input, with first the 8 LSB and then the 8 MSB */
  if (sys_outb(TIMER_0 + timer, div_LSB) !=0 )  {
    printf("ERROR MESSAGE: Failure on sending div_LSB to timer");
    return -1;
  }
  if (sys_outb(TIMER_0 + timer, div_MSB) !=0 )  {
    printf("ERROR MESSAGE: Failure on sending div_MSB to timer");
    return -1;
  }
  return 0;
}




int (timer_subscribe_int)(uint8_t *bit_no) 
{
  hook_id_timer = *bit_no;

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) !=0)
  {
    printf("ERROR MESSAGE: Failure on sys_irqsetpolicy");
    return -1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {

  if (sys_irqrmpolicy(&hook_id_timer) != 0)
  {
    printf("ERROR MESSAGE: Failure on sys_irqrmpolicy");
    return -1;
  }

  return 0;
}

void (timer_int_handler)() {
  counter_timer ++;
}






int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t ReadBack = TIMER_RB_SEL(timer) | BIT(5) | TIMER_RB_CMD ; // Timer 0 -> 11100010
  uint8_t status;
   if(sys_outb(TIMER_CTRL, ReadBack) != 0)  {
    printf("ERROR MESSAGE: Failure on sending Readback to TIMER_CTRL in function timer_get_conf");
    return -1;
  }
   
   if (util_sys_inb(TIMER_0 + timer, &status) !=0){    
    printf("ERROR MESSAGE: Failure on receiving status from TIMER_0 in function timer_get_conf");
    return -1;
  }
  
  *st = status;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  
  union timer_status_field_val conf;
  
  if( field == tsf_all)
  {
    conf.byte = st;
  }
  else if ( field == tsf_base)
  {
    conf.bcd = st & BIT(0);
  }
  else if ( field == tsf_mode)
  {
    conf.count_mode = (st & (BIT(1)|BIT(2)|BIT(3))) >> 1;
  }
  else if ( field == tsf_initial)
  {
    uint8_t inmode = (st & (BIT(4)|BIT(5))) >> 4;

    if ( inmode == 0)
    conf.in_mode = INVAL_val;

    else if ( inmode == BIT(0))
    conf.in_mode = LSB_only;

    else if ( inmode == BIT(1))
    conf.in_mode = MSB_only;

    else if ( inmode == (BIT(0)|BIT(1)))
    conf.in_mode = MSB_after_LSB;
  }

  if (timer_print_config(timer,field,conf) != 0) {
    printf("ERROR MESSAGE: Failure on printing configuration in function timer_display_conf");
    return -1;
  }

  return 0;
}
