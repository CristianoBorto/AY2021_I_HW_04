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
    
    #include "project.h"
    #include "cytypes.h"
    #include "stdio.h"
    #include "MyFunction.h"
    
    #define BYTE_TO_SEND 4
    #define BUFFER_SIZE 1+BYTE_TO_SEND+1
    #define ON 255
    #define OFF 0
    #define TRUE 1
    #define FALSE 0
    #define Potenz 0
    #define PhotoR 1
    #define a_Potenz 3.85e-3 //calibration lines (intercept = 0)
    
    // interrupt statement
    CY_ISR_PROTO (Custom_ISR_Timer);
    CY_ISR_PROTO (Custom_ISR_UART);
    
    uint8_t DataBuffer [BUFFER_SIZE];
    volatile uint8 Dark_mode;
    volatile uint8 Start_sampling;
    volatile uint8 PacketReadyFlag;
    int32 mean_value;
    uint8 count;
    
#endif
/* [] END OF FILE */
