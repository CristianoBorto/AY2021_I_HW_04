/* ========================================
 *
 * Assignment 04
 * Author: Cristiano Bortolotti
 * File: MyFunction.c
 * Date: 21/10/20 - 30/10/20
 *
 * ========================================
*/

#include "MyISR.h"
#include "MyFunction.h"

//packet parameters
#define BYTE_TO_SEND 4
#define BUFFER_SIZE 1+BYTE_TO_SEND+1
//identification of the two different channels of the AMux
#define Potenz 0
#define PhotoR 1
//calibration line (intercept = 0)
#define a_Potenz 3.90e-3 
//used to set LEDs ON/OFF
#define ON 255
#define OFF 0
        
uint8_t DataBuffer [BUFFER_SIZE]; //Packet statement
    
void Flags_settings()
{
    //reset of all flags
    Start_sampling = FALSE;
    Dark_mode = FALSE;
    PacketReadyFlag = FALSE;
    Always_on = FALSE;
    Char_arrived = FALSE;
    //packet header and tail setting
    DataBuffer [0] = 0xA0;
    DataBuffer [BUFFER_SIZE-1] = 0xC0;
    //reset of global variable
    count = 0;
    mean_value = 0;
    Char_received = 0;
}

void Start_interrupts(void)
{
    CyGlobalIntEnable; 
    isr_Timer_StartEx(Custom_ISR_Timer);
    isr_UART_StartEx(Custom_ISR_UART);
}

void Start_Peripherals(void)
{
    PWM_Start();
    UART_Start();
    ADC_Start();
}

void Reset_UART(void) //required reset of UART 
{
    Char_arrived = FALSE;
    UART_ClearRxBuffer();
}

void Starting(uint8 value)
{
    //reset of variables used to compute ADC output mean
    count = 0;
    mean_value = 0;
    //system setting to start sampling differing between the 2 possible working mode
    Start_sampling = TRUE;
    Confirm_LED_Write(ON);
    switch (value)
    {
        case LIGHT_SENSIBLE:
            Always_on = FALSE;
            Dark_mode = FALSE;
            AMux_FastSelect(PhotoR);
            break;
        case ALWAYS_ON:
            Always_on = TRUE;
            Dark_mode = TRUE;
            AMux_FastSelect(Potenz);
            break;
    }
    Timer_Start();
    Reset_UART();
}

void Stopping(uint8 value)
{
    //reset of system and stopping sampling
    Timer_Stop();
    count = 0;
    mean_value = 0;
    Start_sampling = FALSE;
    Confirm_LED_Write(OFF);
    PWM_WriteCompare(0);
    if (value == ERROR)
    {
        UART_PutString("ERROR! Please insert a valid value!\r\n");
    }
    Reset_UART();
}

int32 Bound_sample (int32 value) //it bounds signal in the range 0-65535
{
    if (value < 0) value = 0;
    if (value > 65535) value = 65535;
    return value;
}

void PhotoR_dark_response(void)
{ 
    //saving value in the packet
    DataBuffer[1] = mean_value >> 8;
    DataBuffer[2] = mean_value & 0xFF;
    //reset of variables used to compute ADC output mean
    count = 0;
    mean_value = 0;
    //changing the channel
    Dark_mode = TRUE;
    AMux_FastSelect(Potenz);
}

void PhotoR_light_response(void) //we still sample PhotoR
{
    PWM_WriteCompare(OFF);
    //saving and sending value in the packet
    DataBuffer[1]= mean_value >> 8;
    DataBuffer[2]= mean_value & 0xFF;
    DataBuffer[3]= 0x00;
    DataBuffer[4]= 0x00;
    PacketReadyFlag = TRUE;
    //reset of variables used to compute ADC output mean
    count = 0;
    mean_value = 0;   
}
    
void Potenz_response(void)
{
    PWM_WriteCompare(mean_value*a_Potenz); // LED modulation
    //saving and sending value in the packet
    DataBuffer[3]= mean_value >> 8;
    DataBuffer[4]= mean_value & 0xFF;
    PacketReadyFlag = TRUE;
    if (Always_on == TRUE) 
    {
        DataBuffer[1]= 0x00;
        DataBuffer[2]= 0x00;
    }
    else
    {
        Dark_mode = FALSE; //shifting to PhotoR channel
        AMux_FastSelect(PhotoR);
    }
    //reset of variables used to compute ADC output mean
    count = 0;
    mean_value = 0;  
}

void Send_packet(void)
{
    UART_PutArray(DataBuffer, BUFFER_SIZE);
    PacketReadyFlag = FALSE;
}

/* [] END OF FILE */
