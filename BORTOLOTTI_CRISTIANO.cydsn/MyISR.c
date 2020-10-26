/* ========================================
 *
 * Assignment 04
 * Author: Cristiano Bortolotti
 * File: MyISR.c
 * Date: 21/10/20 - 30/10/20
 *
 * ========================================
*/

#include "MyISR.h"

#define Threshold 6553 //reasonable threshold for dark mode ~500 mv, 1/10 FS
 
char Char_received = 0;
int32 value_digit = 0;

CY_ISR(Custom_ISR_UART)
{
    Char_received = UART_GetChar();
    switch(Char_received)
        {
            case 'B':
            case 'b':
                Starting();
                break;
            case 'O':
            case 'o':
                Stopping(ON);
                break;
            case 'S':
            case 's':
                Stopping(OFF);
                break;
            default:
                Error();
                break;
        }
}

CY_ISR(Custom_ISR_Timer)
{
    Timer_ReadStatusRegister(); //bring interrupt line low
    if(Start_sampling == TRUE)
    {
        count++;
        value_digit=Bound_sample(ADC_Read32());
        mean_value += value_digit;
        if (count == 5)
        {
            mean_value /= count;
            switch(Dark_mode)
            {
                case FALSE:
                    if (mean_value <= Threshold)
                    {
                        PhotoR_light_response();
                    }
                    else
                    {
                        PhotoR_dark_response();
                    }
                    break;
                case TRUE:
                    Potenz_response();
                    break;
            }
        }
    }
}

/* [] END OF FILE */
