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

#define Threshold 500*a_PhotoR //reasonable threshold for dark mode --> 500 mv, 1/10 FS

char Char_received = 0;
int32 value_digit = 0;
int32 value = 0;

CY_ISR(Custom_ISR_Timer)
{
    Timer_ReadStatusRegister(); //bring interrupt line low
    if(Start_sampling == 1)
    {
        value_digit = ADC_Read32();
        value=Bound_sample(value_digit);
        //value=ADC_CountsTo_mVolts(value); //ADC_CountsTo_mVolts(ADC_Read32());  ADC_Read32();
        switch(Dark_mode)
        {
            case 0:
                if (value <= Threshold)
                {
                    Dark_mode = 1;
                    DataBuffer[1]= value >> 8;
                    DataBuffer[2]= value & 0xFF;
                }
                else
                {
                    PWM_WriteCompare(OFF);
                    DataBuffer[1]= value >> 8;
                    DataBuffer[2]= value & 0xFF;
                    DataBuffer[3]= 0x00;
                    DataBuffer[4]= 0x00;
                    PacketReadyFlag = 1;
                    Dark_mode = 0;
                }
                break;
            case 1:
                PWM_WriteCompare(value_digit*a_Potenz);
                DataBuffer[3]= value >> 8;
                DataBuffer[4]= value & 0xFF;
                PacketReadyFlag = 1;
                Dark_mode = 0;
                break;
        }
    }
}

CY_ISR(Custom_ISR_UART)
{
    Char_received = UART_GetChar();
    switch(Char_received)
        {
            case 'B':
            case 'b':
                Start_sampling = 1;
                Confirm_LED_Write(ON);
                Timer_Start();
                break;
            case 'O':
            case 'o':
                PWM_WriteCompare(ON);
                Confirm_LED_Write(OFF);
                Start_sampling = 0;
                Timer_Stop();
                break;
            case 'S':
            case 's':
                Start_sampling = 0;
                Confirm_LED_Write(OFF);
                PWM_WriteCompare(OFF);
                Timer_Stop();
                break;
            default:
                break;
            
        }
}


/* [] END OF FILE */
