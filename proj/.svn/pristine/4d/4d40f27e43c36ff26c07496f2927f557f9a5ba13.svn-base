// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "tower.h"
#include "block.h"
#include "images.h"
#include "menus.h"

// kbd
// extern int keyboard_IRQ_num;
// extern int hook_id;
// extern uint8_t scan_code;

// timer
//extern int counter_timer;
//extern int hook_id_timer;

// mouse
// extern bool size2;
// extern uint8_t bytes[2];
// extern bool make_break;



int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
/*
static int print_usage() {
  printf("Usage: <mode - hex>\n");

  return 1;
}*/

int(proj_main_loop)(int argc, char *argv[]) 
{
  int score = 0;
  vg_init(SR5);


  xpm_image_t img;
  xpm_load(bg1, XPM_8_8_8_8, &img);
  if ((img.width > get_hres()) || (img.height > get_vres())) {
    vg_exit();
    return -1;
  }
  
  Bloco bloco = init_block(426,819,block_colour(),300,45);


  score = block_move(img,bloco);



  if(vg_exit() != OK)
  {
    printf("ERROR MESSAGE: Unable to set back to text mode!");
    return -1;
  }

  printf("SCORE: ");
  printf("%d\n",score);
  return 0;
}
