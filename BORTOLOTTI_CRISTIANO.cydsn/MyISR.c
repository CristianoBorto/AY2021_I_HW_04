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
#include "MyFunction.h"
 
int32 value_digit = 0;

CY_ISR(Custom_ISR_UART)
{
    if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
    {
        Char_arrived = TRUE;
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
    }
}

/* [] END OF FILE */
