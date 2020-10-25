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

char Char_received=0;

CY_ISR(Custom_ISR_Timer)
{
    Timer_ReadStatusRegister(); //bring interrupt line low
    if(Char_flag==1)
    {
    Data_flag=1;
    }
}

CY_ISR(Custom_ISR_UART)
{
    Char_received = UART_GetChar();
    switch(Char_received)
    {
        case 'B':
        case 'b':
            Char_flag = 1;
            Confirm_LED_Write(1);
            Timer_Start();
            break;
        case 'O':
        case 'o':
            PWM_WriteCompare(0);
            break;
        case 'S':
        case 's':
            Char_flag = 0;
            Confirm_LED_Write(0);
            Timer_Stop();
            break;
        default:
            break;
    }
    
}


/* [] END OF FILE */
