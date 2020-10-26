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
#define Threshold 500 //reasonable threshold for dark mode

int main(void)
{
    Data_flag = 0;
    int32 value_digit = 0;
    int32 value_mv = 0;
    
    CyGlobalIntEnable; 
    isr_Timer_StartEx(Custom_ISR_Timer);
    isr_UART_StartEx(Custom_ISR_UART);
    
    PWM_Start();
    UART_Start();
    ADC_Start();
    AMux_FastSelect(PhotoR);
    
    while(1)
    {
        if (Data_flag==1)
        {
            value_digit = ADC_Read32();
            value_mv = ADC_CountsTo_mVolts(value_digit);
            if (value_mv <= Threshold)
            {
                PWM_WriteCompare(ON);
                sprintf(DataBuffer, "Sample: %ld mv \r\n", value_mv);
                UART_PutString(DataBuffer);
            }
            else
            {
                PWM_WriteCompare(OFF);
                sprintf(DataBuffer, "Sample: %ld mv \r\n", value_mv);
                UART_PutString(DataBuffer);
            }
            Data_flag = 0;
        }
        
    }
}

/* [] END OF FILE */
