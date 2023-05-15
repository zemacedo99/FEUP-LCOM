#include "block.h"

//timer
extern int counter_timer;
extern int hook_id_timer;
//kbc
extern int keyboard_IRQ_num;
extern int hook_id;
extern uint8_t scan_code;
// mouse
extern bool size2;
extern uint8_t bytes[2];
extern bool make_break;
extern int hook_id_mouse;
extern int counter;
extern uint8_t packet[3];
extern struct packet pp;
static uint8_t packet_byte;

Bloco (init_block)(int x, int y, uint32_t colour, uint16_t width, uint16_t height)
{
  Bloco b;
  b.x = x;
  b.y = y;
  b.colour = colour;
  b.width = width;
  b.height = height;
  return b;
}

uint32_t (block_colour)()
{
  static uint32_t scolour = 0x000099;
  scolour += 0x000033;

  if(scolour >= 0xCCCCFF)
  {
    scolour = 0x000033;
  }
  return scolour;
}

int draw_all(Bloco torre[], uint16_t numBlocos ,xpm_image_t img, Bloco bloco)
{
  draw_pixel_map(img, 0, 0);
  draw_tower(torre,numBlocos);
  draw_rect(bloco.x,bloco.y,bloco.colour,bloco.width,bloco.height); // bloco.y / 2 ??
  return 0;
}

int draw_tower(Bloco torre[], uint16_t numBlocos)
{
  if(numBlocos >= 5)
  {
    Bloco torre5[5]; 
    int index = 4;
    int i = numBlocos-1;
    while(index >= 0)
    {
      torre5[index]=torre[i];
      torre5[index].y = 819 - (index * torre5[index].height);
      index --; i--;
    }
    for(int i=0; i < 5; i++)
    {
      draw_rect(torre5[i].x,torre5[i].y,torre5[i].colour,torre5[i].width,torre5[i].height);
    }
  }
  else
  {
    for(int i=0; i < numBlocos; i++)
    {
      draw_rect(torre[i].x,torre[i].y,torre[i].colour,torre[i].width,torre[i].height);
    }
  }


  return 0;
}

int(block_move)(xpm_image_t img, Bloco bloco) 
{
  unsigned mouse_x=570;
  unsigned mouse_y=430;

  xpm_image_t img1;
  xpm_load(bg2, XPM_8_8_8_8, &img1);
  if ((img.width > get_hres()) || (img.height > get_vres())) 
  {
    vg_exit();
    return -1;
  }

  menu_inicial(img1,mouse_x,mouse_y);

  int16_t speed = 12;
  uint8_t fr_rate = 30; 
  uint16_t xf = get_hres()-bloco.width;
  //uint16_t yf = get_vres();

  bool right = true;
  bool spacePress = false;
  bool enterPress = false;
  int score = 0;

  Bloco torre[1000];
  uint16_t numBlocos = 0; 
  torre[numBlocos] = bloco;
  numBlocos += 1;
  bloco.y -= bloco.height; 
  bloco.colour = block_colour();

  // draw_all(torre,numBlocos,img,bloco);
  // memcpy(get_vmem(),get_dbuf(),get_vramsize());

  int ipc_status;
  uint8_t irq_set_timer = 0;
  uint8_t timer_bit = BIT(irq_set_timer);
  uint8_t irq_set_kbd;
  uint16_t irq_set_mouse;
  int r;

  if(write_mouse_command(ENABLE_DATA_REPORTING)!= 0)
  {
    return -1;
  }
  
  if(keyboard_subscribe_int(&irq_set_kbd) != 0) return -1; // subscrever interropções do kbd
  if(timer_subscribe_int(&irq_set_timer) != 0) return -1; // subscrever int do timer
  if(mouse_subscribe_int(&irq_set_mouse) != 0) return -1; // subscrever int do mouse

  message msg;

  while(scan_code != RELEASE_ESC_KEY) 
  { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) 
    { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) 
    { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) 
        {
            case HARDWARE: /* hardware interrupt notification */		
            	
                if (msg.m_notify.interrupts & irq_set_kbd) 
                { 
                  kbc_ih();
                  if(scan_code == PRESS_ENTER_KEY)
                  {
                    enterPress = true;
                    counter_timer = 0; // começa o jogo
                    break;
                  }

                  if(scan_code == PRESS_SPACE_KEY)
                  {
                    if(!spacePress) 
                    {
                      // perder o jogo
                      if(bloco.x >= torre[numBlocos-1].x + torre[numBlocos-1].width || bloco.x + bloco.width <= torre[numBlocos-1].x)
                      {
                        if(keyboard_unsubscribe_int() != 0) 
                        {
                          printf("ERROR MESSAGE: Unable to cancel subscription in function kbd_test_scan");
                          return -1;
                        }
                        if(timer_unsubscribe_int() != 0) 
                        {
                          printf("ERROR MESSAGE: Unable to cancel subscription in function kbd_test_scan");
                          return -1;
                        }
                        
                        if(mouse_unsubscribe_int() != 0) 
                        {
                          printf("ERROR MESSAGE: Unable to cancel subscription in function kbd_test_scan");
                          return -1;
                        }

                        if(write_mouse_command(DISABLE_DATA_REPORTING)!= 0)
                        {
                          return -1;
                        }

                        return score;
                      }

                      // corta o bloco
                      if(bloco.x <= torre[numBlocos-1].x)
                      {
                        bloco.width = torre[numBlocos-1].width-(torre[numBlocos-1].x-bloco.x);
                        bloco.x = torre[numBlocos-1].x;
                        xf = get_hres()-bloco.width;
                      }
                      else
                      {
                        bloco.width = torre[numBlocos-1].width + (torre[numBlocos-1].x-bloco.x);
                        xf = get_hres()-bloco.width;
                      }
                      
                      // adiciona bloco à torre
                      torre[numBlocos] = bloco;
                      numBlocos += 1;
                      score += 1;
                      speed ++;
                      if(numBlocos > 5)
                      {
                       bloco.colour = block_colour();
                      }
                      else
                      {
                        bloco.y -= bloco.height; 
                        bloco.colour = block_colour();    
                      }
                    }
                    spacePress = true;
                  }
                  if(scan_code == RELEASE_SPACE_KEY)
                  {
                    spacePress = false;
                  }
                }


                if (msg.m_notify.interrupts & irq_set_mouse) 
                { 
                  mouse_ih();
                  if(!enterPress)
                  {
                    packet_byte = get_packet_byte();

                    if(counter == 1)
                    {
                      if(packet_byte & BIT(3))
                      {
                        packet[0] = packet_byte;
                        counter ++;
                      }

                      continue;
                    }

                    if(counter == 2)
                    {
                      packet[1] = packet_byte;
                      counter ++;
                      continue;
                    }

                    if(counter == 3){
                      packet[2] = packet_byte;
                      packet_assembly();
                      mouse_x += pp.delta_x;
                      mouse_y -= pp.delta_y;

                      if(pp.lb)
                      {
                        enterPress = true;
                      }
                      
                      if((pp.delta_x != 0) || (pp.delta_y != 0))
                      {
                        menu_inicial(img1, mouse_x, mouse_y);
                      }

                      //mouse_print_packet(&pp);
                      counter = 1;
                    }
                  }
                }

                if (msg.m_notify.interrupts & timer_bit) 
                { 
                  if(!enterPress) break;
                  timer_int_handler();
                  if(counter_timer % (60/fr_rate) == 0)
                  {
                    speed += 0.9;
                    if(right)
                    {
                      bloco.x += speed;
                      if(bloco.x >= xf)
                      {
                        right = false;
                        bloco.x = xf;
                      }
                      draw_all(torre,numBlocos,img,bloco);
                      memcpy(get_vmem(),get_dbuf(),get_vramsize());
                    }

                    else
                    {
                      bloco.x -= speed;
                      if(bloco.x <= 0)
                      {
                        right = true;
                        bloco.x = 0;
                      }
                      draw_all(torre,numBlocos,img,bloco);
                      memcpy(get_vmem(),get_dbuf(),get_vramsize());
                    }
                  }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } 
    else 
    { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }

  }


  if(keyboard_unsubscribe_int() != 0) 
  {
    printf("ERROR MESSAGE: Unable to cancel subscription in function kbd_test_scan");
    return -1;
  }
  if(timer_unsubscribe_int() != 0) 
  {
    printf("ERROR MESSAGE: Unable to cancel subscription in function timer_test_scan");
    return -1;
  }

  if(mouse_unsubscribe_int() != 0) 
  {
    printf("ERROR MESSAGE: Unable to cancel subscription in function kbd_test_scan");
    return -1;
  }

  if(write_mouse_command(DISABLE_DATA_REPORTING)!= 0)
  {
    return -1;
  }


  return score;
}
