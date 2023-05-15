#include <lcom/lcf.h>

/* STATUS REGISER */
#define STAT_REG            0x64
#define PARITY_ERROR        BIT(7)
#define TIMEOUT_ERROR       BIT(6)
#define MOUSE_DATA          BIT(5)

#define INH                 BIT(4) //Inhibit flag: 0 if keyboard is inhibited
#define A2                  BIT(3) // A2 input line: 0 data byte / 1 command byte
#define SYS                 BIT(2) //System flag: 0 if system in power-on reset, 1 if system already initialized

#define INPUT_BUFFER_FULL   BIT(1)
#define OUTPUT_BUFFER_FULL  BIT(0)


/*Address*/
#define OUT_BUF             0x60
#define IN_BUF              0x60


/* Mouse-Related KBC Commands */

#define RCB                 0x20    //Read Command Byte
#define WCB                 0x60    //Write Command Byte
#define DISABLE_MOUSE       0xA7
#define ENABLE_MOUSE        0xA8      
#define CHECK_MOUSE_INT     0xA9    //Return 0, if OK

#define WRITE_BYTE_MOUSE    0xD4    //Byte(A)


/* PS/2 MOUSE COMMANDS */
/* Commands passed as arguments of command 0xD4 */

#define RESET                   0xFF
#define RESEND                  0xFE
#define SET_DEFAULTS            0xF6
#define DISABLE_DATA_REPORTING  0xF5
#define ENABLE_DATA_REPORTING   0xF4
#define SET_SAMPLE_RATE         0xF3
#define SET_REMOTE_MODE         0xF0
#define READ_DATA               0xEB
#define SET_STREAM_MODE         0XEA
#define STATUS_REQUEST          0XE9
#define SET_RESOLUTION          0XE8
#define SET_SCALLING_21         0XE7
#define SET_SCALLING_11         0XE6


/*  the mouse controller sends an acknowledgment byte */
#define ACK                     0xFA  // every thing is ok
#define NACK                    0xFE  // if invalid byte 
#define ERROR                   0xFC // second consecutive invalid byte


/*  KBC COMMAND BYTE  */
#define DIS2                BIT(5)
#define DIS                 BIT(4)
#define INT2                BIT(1)
#define INT                 BIT(0)


#define MOUSE_IRQ_NUM           12


/*  MOUSE PACKET BYTE  information */
#define Y_OVfl              BIT(7)
#define X_OVfl              BIT(6)
#define MSB_Y_delta         BIT(5)
#define MSB_X_delta         BIT(4)
#define MB                  BIT(2)
#define RB                  BIT(1)
#define LB                  BIT(0)







int(mouse_subscribe_int)(uint16_t *bit_no);
int (mouse_unsubscribe_int)();
int (mouse_enable_data_reporting)();
void (mouse_ih)();
uint8_t get_packet_byte();
void packet_assembly();
int write_mouse_command(uint8_t command_argument);
int mouse_disable_data_reporting();
void statemachine(struct packet *pp, uint8_t x_len, uint8_t tolerance,bool * end);

struct mouse_ev* mouse_detect_eventt	(	struct packet * 	pp	);	

