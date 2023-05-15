#include "menus.h"

int menu_inicial(xpm_image_t img, unsigned x, unsigned y)
{

  draw_pixel_map(img, 0, 0);
  memcpy(get_vmem(),get_dbuf(),get_vramsize());

  if ((x > get_hres()) || (y > get_vres())) 
  {
    return -1;
  }

  draw_rect(x,y,0xFFFFFF,25,15); 
  draw_rect(x+25,y+15,0xFFFFFF,5,5); 
  draw_rect(x+30,y+20,0xFFFFFF,5,5); // desenha rato
  memcpy(get_vmem(),get_dbuf(),get_vramsize());
  return 0;
}
