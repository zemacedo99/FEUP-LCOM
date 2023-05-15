#include <lcom/lcf.h>

/*Address*/
#define OUT_BUF             0x60
#define IN_BUF              0x60

#define RELEASE_ESC_KEY     0x81
#define PRESS_SPACE_KEY     0x39
#define RELEASE_SPACE_KEY   0xb9
#define PRESS_ENTER_KEY     0x1C
#define RELEASE_ENTER_KEY   0x9C

#define SCAN_CODE_SIZE2     0xE0
#define MSB                 BIT(7)

/* STATUS REGISER */
#define STAT_REG            0x64
#define PARITY_ERROR        BIT(7)
#define TIMEOUT_ERROR       BIT(6)
#define MOUSE_DATA          BIT(5)
#define INPUT_BUFFER_FULL   BIT(1)
#define OUTPUT_BUFFER_FULL  BIT(0)



#define RCB                 0x20    //Read Command Byte
#define WCB                 0x60    //Write Command Byte
#define CHECK_KBC           0xAA
#define CHECK_KBC_INT       0xAB    //Check Keyboard Interface  
#define DISABLE_KBD_INT     0xAD    //Disable KBD Interface
#define ENABLE_KBD_INT      0xAE    //Enable KBD Interface


/*  COMMAND BYTE    */
#define DIS2                BIT(5)
#define DIS                 BIT(4)
#define INT2                BIT(1)
#define INT                 BIT(0)



/* AH Meaning */

#define FCF           0x01 // Function call failed
#define FNS           0x02 // Function is not supported in current HW configuration
#define FNV           0x03 // Function is invalid in current video mode


/* Graphics Mode */

#define SR1           0x105
#define SR2           0x110
#define SR3           0x115
#define SR4           0x11A
#define SR5           0x14C



/* VBE Functions */

#define AH            0x4F 
#define SII           0x10  // INT 0x10 software interrupt instruction
#define AL_OK         0x4F  // VBE function called is supported
/* AL VBE Instructions */
#define AL_RCI        0x00 // Return VBE Controller Information  
#define AL_RMI        0x01 // Return VBE Mode Information  
#define AL_SVM        0x02 // Set VBE Mode
#define AL_STM        0x03 // Set Text Mode  



                //*********//
                //* TIMER *//
                //*********//


#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)
