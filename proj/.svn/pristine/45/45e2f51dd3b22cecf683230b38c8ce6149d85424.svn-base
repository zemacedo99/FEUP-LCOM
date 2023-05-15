#include "tower.h"

int (init_bg)(xpm_map_t xpm)
{
  xpm_image_t img;
  xpm_load(xpm, XPM_8_8_8_8, &img);

  if ((img.width > get_hres()) || (img.height > get_vres())) {
    vg_exit();
    return -1;
  }

  draw_pixel_map(img, 0, 0);
  return 0;
}





//Criar struck com x e comprimento do bloco
//criar um array de blocos onde guardamos os blocos q formam a torre


