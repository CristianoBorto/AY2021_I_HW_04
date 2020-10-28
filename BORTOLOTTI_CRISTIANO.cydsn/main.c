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
#include "MyFunction.h"

#define THRESHOLD 6553 //reasonable threshold for dark mode empirically derived --> 1/10 FS
#define SAMPLE_NUMBER 5

int main(void)
{
    // Initialization phase
    Flags_settings();
    Start_interrupts();
    Start_Peripherals();
    
    while(1)
    {
        if (Char_arrived == TRUE)
        {
            Char_received = UART_GetChar();
            switch(Char_received)
            {
                case 'B':
                case 'b':
                    Starting(LIGHT_SENSIBLE);
                    break;
                case 'O':
                case 'o':
                    Starting(ALWAYS_ON);
                    break;
                case 'S':
                case 's':
                    Stopping(STOP);
                    break;
                default:
                    Stopping(ERROR);
                    break;
            }
        }
           
        if (count == SAMPLE_NUMBER)
        {
            mean_value /= count;
            switch(Dark_mode)
            {
                case FALSE:
                    if (mean_value <= THRESHOLD)
                    {
                        PhotoR_dark_response();
                    }
                    else
                    {
                        PhotoR_light_response();
                    }
                    break;
                case TRUE:
                    Potenz_response();
                    break;
            }
            
        }

        if (PacketReadyFlag == TRUE)
            {
                Send_packet();
            } 
    
    } //end while(1)
} //end main

/* [] END OF FILE */
