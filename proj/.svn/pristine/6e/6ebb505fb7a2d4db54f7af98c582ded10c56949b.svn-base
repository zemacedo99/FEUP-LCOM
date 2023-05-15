// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include "mouse.h"


int hook_id_mouse = MOUSE_IRQ_NUM;
static uint8_t packet_byte;
uint8_t status;
uint8_t packet[3];
int counter = 1;
struct packet pp;
//int qn = 1;
//int x;
//int y;
//uint8_t x_leng;
//uint8_t toleranceg;

uint8_t get_packet_byte()
{
    return packet_byte;
}

/*
struct packet{
    uint8_t bytes[3];
    bool rb;
    bool mb;
    bool lb;
    int16_t delta_x;
    int16_t delta_y;
    bool x_ov;          
    bool y_ov;
};*/


int(mouse_subscribe_int)(uint16_t *bit_no)
{
 
    *bit_no = BIT(hook_id_mouse);
    
    if(sys_irqsetpolicy(MOUSE_IRQ_NUM,IRQ_EXCLUSIVE | IRQ_REENABLE , &hook_id_mouse)!= OK) {
        printf("ERROR MESSAGE: Function mouse_subscribe_int, unable to set IRQ policy");
        return -1;
    }

    return 0;
}
int (mouse_unsubscribe_int)() {

    if (sys_irqrmpolicy(&hook_id_mouse) != OK)
    {
        printf("ERROR MESSAGE: Function mouse_unsubscribe_int, unable to remove IRQ policy");
        return -1;
    }

    return 0;
}

/*
int (mouse_enable_data_reporting)(){
    //Subscribe the mouse interrupts in a exclusive mode
    if(mouse_subscribe_int !=0){
        return -1;
    }
    //Enable data reporting by the mouse
    //not constructed yet
}
*/


void (mouse_ih)(){
    
    util_sys_inb(STAT_REG,&status);
    if((status & PARITY_ERROR)!=0){
        printf("ERROR MESSAGE: Funtion mouse_ih, check_OBF_status with parity error");
        return ;
    }
    if((status & TIMEOUT_ERROR)!=0){
        printf("ERROR MESSAGE: Funtion mouse_ih, check_OBF_status with timeout error");
        return ;
    }
    if((status & OUTPUT_BUFFER_FULL) == 0){
        printf("ERROR MESSAGE: Function mouse_ih status with output buffer not full");
        return;
    }

    util_sys_inb(OUT_BUF, &packet_byte);

}  

int write_mouse_command(uint8_t command_argument){

    bool nack = false;
    
    do
    {
    if((sys_outb(STAT_REG,WRITE_BYTE_MOUSE)!=OK)) // manda o commando de escrita do rato (0xD4) para o Status Register 
    {
        return -1;
    }
    
    if(util_sys_inb(STAT_REG,&status)!=OK)  // Recebe o status do KBC
    {
        return -1;
    }


    if((status & INPUT_BUFFER_FULL)!=0) // verifica o IBF
    {
        return -1;
    }

    if(sys_outb(IN_BUF,command_argument)!=OK) // manda o comando de disable data report do mouse (0xF5) para o out but 
    {
        return -1;
    }

    
    if(util_sys_inb(OUT_BUF,&status)!=OK) return -1;

    if(status == ACK) return 0;
    nack = false;

    if(status == NACK)
    {
        nack = true;
    }
    
    
    if(status == ERROR)
    {
        return -1;
    }
    

    } while (nack);
    return 0;
}



void packet_assembly()
{
    pp.bytes[0] = packet[0];

    pp.rb = packet[0] & RB;
    pp.mb = packet[0] & MB;
    pp.lb = packet[0] & LB;
    
    if((packet[0] & MSB_X_delta) == 0) pp.delta_x = 0;
    else pp.delta_x = ~0xFF;
    if((packet[0] & MSB_Y_delta) == 0) pp.delta_y = 0;
    else pp.delta_y = ~0xFF;

    pp.x_ov = packet[0] & X_OVfl;          
    pp.y_ov = packet[0] & Y_OVfl;

    pp.bytes[1] = packet[1];

    pp.delta_x = pp.delta_x | packet[1];

    pp.bytes[2] = packet[2];

    pp.delta_y = pp.delta_y | packet[2];
}


void statemachine(struct packet *pp, uint8_t x_len, uint8_t tolerance, bool *end)
{
struct mouse_ev mev;
mev = *mouse_detect_event(pp);

static int x;
static int y;
static int qn = 1;

switch (qn)
{

case 1:
    x = 0;
    y = 0;

    if(mev.type == LB_PRESSED)
    {
        qn = 2;
    }
    break;



case 2:

    if(mev.type == LB_RELEASED)
    {
        if(y/x >= 1)
        {
            //printf("%d\t%d\n",x,x_len);
            if(x >= x_len)
            {
                qn = 3;
            }
            else
            {
                qn = 1;
            }
            
        }
    }

    else if ( mev.type == MOUSE_MOV)
    {
        if( ((mev.delta_x < 0) && (abs(mev.delta_x) > tolerance)) || ((mev.delta_y < 0) && (abs(mev.delta_y) > tolerance)))
        {
            qn = 1;
        }
        else
        {
            x += mev.delta_x;
            y += mev.delta_y;
        }
        
    }

    else
    {
        qn = 1;
    }
    break;



case 3:

    x = 0;
    y = 0;

    if(mev.type == LB_PRESSED)
    {
        qn = 2;
    }

    else if(mev.type == RB_PRESSED)
    {
        qn = 4;
    }

    else if ( mev.type == MOUSE_MOV)
    {
        if( (abs(mev.delta_x) > tolerance) || (abs(mev.delta_y) > tolerance)  )
        {
            qn = 1;
        }
    }

    else
    {
        qn = 1;
    }
    break;



case 4:

    if(mev.type == RB_RELEASED)
    {
        if( y/x <= -1)
        {
            if(x >= x_len)
            {
                qn = 5;
                *end = true;
            }
            else
            {
                qn = 1;
            }
            
        }
    }

    else if ( mev.type == MOUSE_MOV)
    {
        if(((mev.delta_x < 0) && (abs(mev.delta_x) > tolerance)) || ((mev.delta_y > 0) && (abs(mev.delta_y) > tolerance)))
        {
            qn = 1;
        }
        else
        {
            x += mev.delta_x;
            y += mev.delta_y;
        }
        
    }

    else
    {
        qn = 1;
    }
    break;


default:
    break;
}
//printf("%d\n",qn);
}


/*
struct mouse_ev* mouse_detect_eventt	(struct packet * pp)
{
    struct mouse_ev* mev;

    if(pp.rb)
    {
        mev->type = RB_PRESSED;
    }
    if(!pp.rb)
    {
        mev->type = RB_REALESE;
    }
    

    mev->type =
    pp.bytes[0];

}
*/	

