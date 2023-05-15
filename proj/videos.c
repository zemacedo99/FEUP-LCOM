// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include "videos.h"

static unsigned h_res;   /* Horizontal resolution in pixels */
static unsigned v_res;  	/* Vertical resolution in pixels */
static unsigned bits_per_pixel;   /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel;   /* Number of VRAM bytes per pixel */
static uint8_t *v_mem;      /* Process (virtual) address to which VRAM is mapped */
static uint8_t *d_buf;    
static void *video_mem; /*frame-buffer VM address static global variable*/
static unsigned int vram_size; /* VRAM's size, but you can use the frame-buffer size, instead */
static vbe_mode_info_t vbemodeinfo; 

unsigned get_hres()
{
  return h_res;
}

unsigned get_vres()
{
  return v_res;
}

unsigned get_bitspp()
{
  return bits_per_pixel;
}

unsigned get_bytespp()
{
  return bytes_per_pixel;
}

uint8_t* get_vmem()
{
  return v_mem;
}

uint8_t* get_dbuf()
{
  return d_buf;
}

unsigned int get_vramsize()
{
  return vram_size;
}


int (set_vbe_mode)(uint16_t mode)
{
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));	/* zero the structure */

  reg86.intno = SII; // INT 0x10 software interrupt instruction
  //reg86.ah = AH;
  //reg86.al = AL_SVM;
  reg86.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
  //reg86.bx = 1<<14|0x105; // set bit 14: linear framebuffer
  reg86.bx = 1 << 14 | mode;

  if (sys_int86(&reg86) != OK) {
    printf("vg_exit(): sys_int86() failed \n");
    return -1;
  }

  if(reg86.al != AL_OK)
  {
    return -1;
  }

  if(reg86.ah != OK)
  {
    return -1;
  }

  return 0;
}


void *(vg_init)(uint16_t mode) 
{
  
  vbe_get_mode_info(mode,&vbemodeinfo);

  h_res = vbemodeinfo.XResolution;
  v_res = vbemodeinfo.YResolution;

  bits_per_pixel = vbemodeinfo.BitsPerPixel;



  struct minix_mem_range mr;
  unsigned int vram_base; /* VRAM's physical addresss */

  int r;


  vram_base = vbemodeinfo.PhysBasePtr;
  bytes_per_pixel = ((bits_per_pixel+7)/8);
  vram_size = h_res * v_res * bytes_per_pixel;
  /* Use VBE function 0x01 to initialize vram_base and vram_size */

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");


  //printf("%d %d %d %d ",h_res,v_res,bits_per_pixel,bytes_per_pixel);
  set_vbe_mode(mode);
  
  v_mem = video_mem;
  d_buf = malloc(vram_size);
  return video_mem;
}


void (set_pixel)(uint16_t x, uint16_t y,uint32_t colour){
  unsigned index = (y*h_res+x)*bytes_per_pixel;
  for(unsigned i = 0; i < bytes_per_pixel; i++){
    v_mem[index+i] = (uint8_t) colour;
    colour = colour >> 8;
  }
}

void (set_pixel2)(unsigned index,uint32_t colour)
{ 
  for(unsigned i = 0; i < bytes_per_pixel; i++){
    d_buf[index+i] = (uint8_t) colour;
    colour = colour >> 8;
  }
  //v_mem[index] = colour;
  //colour = colour >> 8*bytes_per_pixel;
}

void (draw_line)(uint16_t x, uint16_t y,uint32_t colour, uint16_t width){
  for(int i = x ; i < width+x; i++){
    set_pixel(i,y,colour);
  }
}

void (draw_rect)(uint16_t x, uint16_t y,uint32_t colour, uint16_t width, uint16_t height)
{

  unsigned index = (y*h_res+x)*bytes_per_pixel;
  for(int i = y; i < height+y; i++)
  {
      for(int j = x; j < width+x; j++)
      {
        set_pixel2(index,colour);
        index += bytes_per_pixel;
      }

      index += (h_res-width)*bytes_per_pixel;
  }

  //  draw_line(x,i,colour,width);
}



void (draw_rect_indexed)(uint32_t first, uint8_t no_rectangles, uint8_t step){
  uint16_t row = 0;
  uint16_t col = 0;
  uint32_t colour;
  uint16_t width = (h_res/no_rectangles);
  uint16_t height = (v_res/no_rectangles);

  for(unsigned j = 0; j < no_rectangles; j++){
    for(unsigned i = 0; i < no_rectangles; i++){
      colour = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
      draw_rect(i*width,j*height,colour, width, height);
      col++;
    }
    col = 0;
    row ++;
  }
}

uint32_t (direct_colour)(uint32_t colour, uint16_t col, uint8_t step){
  //to calculate R(first), G(first), B(first)
  uint32_t R_first = colour >> vbemodeinfo.RedFieldPosition & (BIT(vbemodeinfo.RedMaskSize)-1);
  uint32_t G_first = colour >> vbemodeinfo.GreenFieldPosition & (BIT(vbemodeinfo.GreenMaskSize)-1);
  uint32_t B_first = colour >> vbemodeinfo.BlueFieldPosition & (BIT(vbemodeinfo.BlueMaskSize)-1);
  
  uint32_t R = (R_first + col * step) % (1 << vbemodeinfo.RedMaskSize);
  uint32_t G = (G_first + col * step) % (1 << vbemodeinfo.GreenMaskSize);
  uint32_t B = (B_first + col * step) % (1 << vbemodeinfo.BlueMaskSize);

  return (R << vbemodeinfo.RedFieldPosition | G << vbemodeinfo.GreenFieldPosition | B << vbemodeinfo.BlueFieldPosition);
}


void (draw_rect_direct)(uint32_t first, uint8_t no_rectangles, uint8_t step){
  uint16_t row = 0;
  uint16_t col = 0;
  uint32_t colour;
  uint16_t width = (h_res/no_rectangles);
  uint16_t height = (v_res/no_rectangles);

  for(unsigned j = 0; j < no_rectangles; j++)
  {
    for(unsigned i = 0; i < no_rectangles; i++){
      colour = direct_colour(first, col, step);
      draw_rect(i*width,j*height,colour, width, height);
      col++;
    }

    col = 0;
    row ++;
  }
}

void (draw_pixel_map)(xpm_image_t img, uint16_t x, uint16_t y){
  unsigned index = (y*h_res+x)*bytes_per_pixel;
  unsigned colour_ind = 0;

  for(int i = y; i < img.height+y; i++)
  {
      for(int j = x; j < img.width+x; j++) 
      {
        uint32_t colour = 0;
        for(unsigned int k = 0; k < bytes_per_pixel; k++)
        {
          colour |= img.bytes[k+colour_ind] << k*8;
        }
        set_pixel2(index,colour);
        index += bytes_per_pixel;
        colour_ind += bytes_per_pixel;
      }
      
      index += (h_res-img.width)*bytes_per_pixel;
  }
}

