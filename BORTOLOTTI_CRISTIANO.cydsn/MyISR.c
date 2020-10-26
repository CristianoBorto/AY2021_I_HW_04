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

#define a_Potenz 3.85e-3 //calibration lines (intercept = 0)
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
        value_digit=Bound_sample(ADC_Read32());
        switch(Dark_mode)
        {
            case FALSE:
                if (value_digit <= Threshold)
                {
                    Dark_mode = TRUE;
                    DataBuffer[1]= value_digit >> 8;
                    DataBuffer[2]= value_digit & 0xFF;
                }
                else
                {
                    PWM_WriteCompare(OFF);
                    DataBuffer[1]= value_digit >> 8;
                    DataBuffer[2]= value_digit & 0xFF;
                    DataBuffer[3]= 0x00;
                    DataBuffer[4]= 0x00;
                    PacketReadyFlag = TRUE;
                }
                break;
            case TRUE:
                PWM_WriteCompare(value_digit*a_Potenz);
                DataBuffer[3]= value_digit >> 8;
                DataBuffer[4]= value_digit & 0xFF;
                PacketReadyFlag = TRUE;
                Dark_mode = FALSE;
                break;
        }
    }
}




/* [] END OF FILE */
