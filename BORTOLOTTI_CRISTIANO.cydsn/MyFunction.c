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

/* [] END OF FILE */
