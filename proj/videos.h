 #pragma once
 #include "i8254.h"

/**
 * desenha pixel map
 * @param img pixel map a desenhar
 * @param x posicão no eixo dos x
 * @param y posicão no eixo dos y
*/
void (draw_pixel_map)(xpm_image_t img, uint16_t x, uint16_t y);

/**
 * desenha pixel map
 * @param mode modo de video
 * @return 0
*/
int (set_vbe_mode)(uint16_t mode);

/**
 * desenha pixel map
 * @param x posicão no eixo dos x
 * @param y posicão no eixo dos y
 * @param colour cor do bloco
*/
void (set_pixel)(uint16_t x, uint16_t y,uint32_t colour);


void (draw_line)(uint16_t x, uint16_t y,uint32_t colour, uint16_t width);
void (draw_rect)(uint16_t x, uint16_t y,uint32_t colour, uint16_t width, uint16_t height);
void (draw_rect_indexed)(uint32_t first, uint8_t no_rectangles, uint8_t step);
uint32_t (direct_colour)(uint32_t colour, uint16_t col, uint8_t step);
void (draw_rect_direct)(uint32_t first, uint8_t no_rectangles, uint8_t step);


unsigned get_hres();
unsigned get_vres();
unsigned get_bitspp();
unsigned get_bytespp();
uint8_t* get_vmem();
uint8_t* get_dbuf();
unsigned int get_vramsize();

