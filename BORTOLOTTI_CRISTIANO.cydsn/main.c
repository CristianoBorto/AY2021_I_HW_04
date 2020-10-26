/* ========================================
 *
 * Assignment 04
 * Author: Cristiano Bortolotti
 * File: main.c
 * Date: 21/10/20 - 30/10/20
 *
 * ========================================
*/
#include "MyISR.h"

int main(void)
{
    // Initialization phase
    Flag_settings();
    Start_interrupt();
    Start_Peripherals();
    
    while(1)
    {
        while (Start_sampling == FALSE );
        
        if (Dark_mode == TRUE)
        {
            AMux_FastSelect(Potenz);
        }
        if (Dark_mode == FALSE)
        {
            AMux_FastSelect(PhotoR);
        }
        if (PacketReadyFlag == TRUE)
        {
            UART_PutArray(DataBuffer, BUFFER_SIZE);
            PacketReadyFlag = 0;
        }
        
    } //end while(1)
} //end main

/* [] END OF FILE */
