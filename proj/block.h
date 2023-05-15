#pragma once
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include "i8254.h"
#include "keyboard.h"
#include "mouse.h"
#include "videos.h"
#include "tower.h"
#include "images.h"
#include "menus.h"

// struct do bloco
typedef struct 
{
  int x; // posição do eixo dos x
  int y; // posição do eixo dos y
  uint32_t colour; // cor do bloco
  uint16_t width;  // comprimento do bloco
  uint16_t height; // altura do bloco
}
Bloco;

/**
 * incializa o bloco
 * @param x posição do eixo dos x
 * @param y posição do eixo dos y
 * @param colour cor do bloco
 * @param width comprimento do bloco
 * @param height altura do bloco
 * @return Bloco 
*/
Bloco (init_block)(int x, int y, uint32_t colour, uint16_t width, uint16_t height);

/**
 * muda a cor do bloco
 * @return cor do bloco
*/
uint32_t (block_colour)();

/**
 * trata de tudo relacionado com o bloco
 * @param xpm imagem de fundo
 * @param bloco bloco
 * @return 0
*/
int(block_move)(xpm_image_t img, Bloco bloco);

/**
 * desenha a torre
 * @param torre torre de blocos
 * @param numBlocos numero de blocos
 * @return 0
*/
int draw_tower(Bloco torre[], uint16_t numBlocos);

/**
 * desenha a torre
 * @param torre torre de blocos
 * @param numBlocos numero de blocos
 * @param img imagem de fundo
 * @param bloco bloco
 * @return 0
*/
int draw_all(Bloco torre[], uint16_t numBlocos ,xpm_image_t img, Bloco bloco);
