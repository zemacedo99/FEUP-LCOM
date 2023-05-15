#include "keyboard.h"

int count_sys_inb = 0;
int keyboard_IRQ_num = 1;
int hook_id = 1;
uint8_t scan_code;
bool size2 = false; // array com 2 bytes
uint8_t bytes[2]; // array para guardar o scan_code
bool make_break; //make or break code


int (keyboard_subscribe_int)(uint8_t *bit_no) 
{

    *bit_no = BIT(1);
    if(sys_irqsetpolicy(keyboard_IRQ_num,IRQ_EXCLUSIVE | IRQ_REENABLE, &hook_id)!= OK) {
        printf("ERROR MESSAGE: function kdb_test_scan unable to subscribe KBC interrupts");
        return -1;
    }

    return 0;
}

int (keyboard_unsubscribe_int)() {

    if (sys_irqrmpolicy(&hook_id) != OK)
    {
        printf("ERROR MESSAGE: Failure on sys_irqrmpolicy");
        return -1;
    }

    return 0;
}
int check_OBF_status(){
    uint8_t status;
    util_sys_inb(STAT_REG,&status);
    count_sys_inb++;

    if((status & PARITY_ERROR)!=0){
        printf("ERROR MESSAGE: check_OBF_status with parity error");
        return -1;
    }
    if((status & TIMEOUT_ERROR)!=0){
        printf("ERROR MESSAGE: check_OBF_status with timeout error");
        return -1;
    }
    if((status & MOUSE_DATA)!= 0){
        printf("check_OBF_status with information of the mouse");
        return -1;
    }

    if((status & OUTPUT_BUFFER_FULL) != 0)
    {
        return 0;
    }
    return -1;
}
int get_scan_code(){
    util_sys_inb(OUT_BUF, &scan_code);
    count_sys_inb ++;
    //#ifdef LAB3
    //#endif

    if((scan_code & MSB)!=0)
    {   // check if it is a break code
        //if break code it enters the if
        make_break = false;  // false == break_code
    }             
    else
    {
        make_break = true; // true == make_code
    }

    if(scan_code == SCAN_CODE_SIZE2)
    { //Check if it is a 2 byte scan code
        bytes[0] = SCAN_CODE_SIZE2;
        size2 = true;
        return 1;
    }
    if (size2)
    {
        bytes[1] = scan_code;
        size2 = false;
        return 0;
    }

    bytes[0] = scan_code;
    return 0;
}
int enable_kbc_int(){
    uint8_t command_byte;

    sys_outb(STAT_REG, RCB); //Tells status register for read command byte
    util_sys_inb(OUT_BUF, &command_byte); //Gets command byte from input buffer
    count_sys_inb++;
    command_byte = command_byte | INT; //Using mask to enable keyboard interrupt
    sys_outb(STAT_REG, WCB); //Tells status register to receive write command byte
    sys_outb(IN_BUF, command_byte); //Sends to the intput buffer 
    return 0;
}
void (kbc_ih)(){
    //We will get the status on the status register which is on 0x64
    uint8_t status;
    //#define LAB3
    util_sys_inb(STAT_REG,&status);
    //#ifdef LAB3
    count_sys_inb++;
    //#endif
    //check if any error on the status
    if((status & PARITY_ERROR)!=0){
        printf("ERROR MESSAGE: kbc_ih status with parity error");
        return;
    }
    if((status & TIMEOUT_ERROR)!=0){
        printf("ERROR MESSAGE: kbc_ih status with timeout error");
        return;
    }
    if((status & MOUSE_DATA)!= 0){
        printf("kbc_ih with information of the mouse");
        return;
    }
    //check if output buffer is ready to send scan code
    if((status & OUTPUT_BUFFER_FULL) == 0){
        printf("ERROR MESSAGE: kbc_ih status with output buffer not full");
        return;
    }
    //After checking the status and everything is ok, we will read the scan code
    //in the output buffer
    //#define LAB3
    util_sys_inb(OUT_BUF, &scan_code);
    //#ifdef LAB3
    count_sys_inb = count_sys_inb +1;
    //#endif

    if((scan_code & MSB)!=0)
    {   // check if it is a break code
        //if break code it enters the if
        make_break = false;  // false == break_code
    }             
    else
    {
        make_break = true; // true == make_code
    }

    if(scan_code == SCAN_CODE_SIZE2)
    { //Check if it is a 2 byte scan code
        bytes[0] = SCAN_CODE_SIZE2;
        size2= true;
        return;
    }
    if (size2)
    {
        bytes[1] = scan_code;
        size2 = false;
        return;
    }

    bytes[0] = scan_code;
}
