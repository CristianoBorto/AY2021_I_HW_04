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
#include "MyISR.h"

#define Potenz 0
#define PhotoR 1

int main(void)
{
    
    Start_sampling = 0;
    Dark_mode = 0;
    
    DataBuffer [0] = 0xA0;
    DataBuffer [BUFFER_SIZE-1] = 0xC0;
    PacketReadyFlag = 0;
    
    CyGlobalIntEnable; 
    isr_Timer_StartEx(Custom_ISR_Timer);
    isr_UART_StartEx(Custom_ISR_UART);
    
    PWM_Start();
    UART_Start();
    ADC_Start();
    AMux_FastSelect(PhotoR);
    
    while(1)
    {
        if (Dark_mode == 1)
        {
            AMux_FastSelect(Potenz);
        }
        if (Dark_mode == 0)
        {
            AMux_FastSelect(PhotoR);
        }
        if (PacketReadyFlag == 1)
        {
            UART_PutArray(DataBuffer, BUFFER_SIZE);
            PacketReadyFlag = 0;
        }
        
    } //end while(1)
} //end main

/* [] END OF FILE */
