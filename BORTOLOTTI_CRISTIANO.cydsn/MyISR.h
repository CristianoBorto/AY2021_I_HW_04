/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
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
    //calibration lines (intercept = 0)
    #define a_PhotoR 13.107
    #define a_Potenz 3.85e-3 //5.1e-2
    
    
    // interrupt statement
    CY_ISR_PROTO (Custom_ISR_Timer);
    CY_ISR_PROTO (Custom_ISR_UART);
    
    uint8_t DataBuffer [BUFFER_SIZE];
    volatile uint8 Dark_mode;
    volatile uint8 Start_sampling;
    volatile uint8 PacketReadyFlag;
    
#endif
/* [] END OF FILE */
