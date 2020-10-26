/* ========================================
 *
 * Assignment 04
 * Author: Cristiano Bortolotti
 * File: MyFunction.c
 * Date: 21/10/20 - 30/10/20
 *
 * ========================================
*/

#include "MyFunction.h"
#include "MyISR.h"

void Flag_settings()
{
    Start_sampling = FALSE;
    Dark_mode = FALSE;
    PacketReadyFlag = FALSE;
    DataBuffer [0] = 0xA0;
    DataBuffer [BUFFER_SIZE-1] = 0xC0;
    count = 0;
    mean_value = 0;
}

void Start_interrupt(void)
{
    CyGlobalIntEnable; 
    isr_Timer_StartEx(Custom_ISR_Timer);
    isr_UART_StartEx(Custom_ISR_UART);
}

void Start_Peripherals(void)
{
    PWM_Start();
    UART_Start();
    AMux_FastSelect(PhotoR);
    ADC_Start();
}

void Starting(void)
{
    Start_sampling = TRUE;
    Confirm_LED_Write(ON);
    Timer_Start();
    UART_ClearRxBuffer();
}

void Stopping(uint8 value)
{
    Timer_Stop();
    Start_sampling = FALSE;
    PWM_WriteCompare(value);
    Confirm_LED_Write(OFF);
    UART_ClearRxBuffer();
    count = 0;
    mean_value = 0;
    Dark_mode = FALSE;
}

void Error(void)
{
    UART_PutString("ERROR! Please insert a valid value!\r\n");
    UART_ClearRxBuffer();
}

int32 Bound_sample (int32 value)
{
    if (value < 0) value = 0;
    if (value > 65535) value = 65535;
    return value;
}

void PhotoR_light_response(void)
{
    Dark_mode = TRUE;
    DataBuffer[1] = mean_value >> 8;
    DataBuffer[2] = mean_value & 0xFF;
    count = 0;
    mean_value = 0;
}

void PhotoR_dark_response(void)
{
    PWM_WriteCompare(OFF);
    DataBuffer[1]= mean_value >> 8;
    DataBuffer[2]= mean_value & 0xFF;
    DataBuffer[3]= 0x00;
    DataBuffer[4]= 0x00;
    PacketReadyFlag = TRUE;
    count = 0;
    mean_value = 0;   
}
    
void Potenz_response(void)
{
    PWM_WriteCompare(mean_value*a_Potenz);
    DataBuffer[3]= mean_value >> 8;
    DataBuffer[4]= mean_value & 0xFF;
    PacketReadyFlag = TRUE;
    Dark_mode = FALSE;
    count = 0;
    mean_value = 0;  
}

/* [] END OF FILE */
