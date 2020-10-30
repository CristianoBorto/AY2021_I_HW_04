/* ========================================
 *
 * Assignment 04
 * Author: Cristiano Bortolotti
 * File: MyISR.h
 * Date: 21/10/20 - 30/10/20
 *
 * ========================================
*/

#ifndef INTERRUPT_ROUTINES_H
    #define INTERRUPT_ROUTINES_H
    
    //header inclusion
    #include "project.h"
    #include "cytypes.h"
    
    //convention used for conditional controls useful to distinguish flags from variables
    #define TRUE 1 
    #define FALSE 0
    // identification of the 2 possible working mode
    #define LIGHT_SENSIBLE 0
    #define ALWAYS_ON 1
    // identification of stop and default mode
    #define STOP 0
    #define ERROR 1

    
    // interrupt statement
    CY_ISR_PROTO (Custom_ISR_Timer);
    CY_ISR_PROTO (Custom_ISR_UART);
    
    //global flag statement   
    volatile uint8 Dark_mode;
    volatile uint8 Start_sampling;
    volatile uint8 PacketReadyFlag;
    volatile uint8 Char_arrived;
    volatile uint8 Always_on;
    
    //global variable statement
    int32 mean_value;
    char Char_received;
    uint8 count;
#endif
/* [] END OF FILE */
