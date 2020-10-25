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
    
    #define BUFFER_SIZE 16
    
    // interrupt statement
    CY_ISR_PROTO (Custom_ISR_Timer);
    CY_ISR_PROTO (Custom_ISR_UART);
    
    char DataBuffer [BUFFER_SIZE];
    volatile uint8 Data_flag;
    volatile uint8 Char_flag;
    
#endif
/* [] END OF FILE */
