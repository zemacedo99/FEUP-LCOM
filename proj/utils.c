#include <lcom/lcf.h>
#include <stdint.h>

extern int count_sys_inb;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  uint16_t Mask_LSB = 0x00ff;
  *lsb = (uint8_t)(val & Mask_LSB);
  return 0; // perguntar o valor de retorno
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  uint16_t Mask_MSB = 0xff00;
  *msb = (uint8_t)((val & Mask_MSB)>>8);

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  
  uint32_t value32;
  if (sys_inb(port, &value32) != 0) return 1;

  *value = (uint8_t)value32;

  #ifdef LAB3
  count_sys_inb++;
  #endif

  return 0;
}
/*int (util_sys_outb)(int port, uint8_t *value){
  uint32_t value32;
  if (sys_outb(port, &value32) != 0) return 1;

  *value = (uint8_t)value32;

  return 0;
}*/


