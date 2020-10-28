/* ========================================
 *
 * Assignment 04
 * Author: Cristiano Bortolotti
 * File: MyFunction.h
 * Date: 21/10/20 - 30/10/20
 *
 * ========================================
*/

#include "project.h"

void Flags_settings(void);

void Start_interrupts(void);

void Start_Peripherals(void);

void Reset_UART(void);

void Starting(uint8 value);

void Stopping(uint8 value);

int32 Bound_sample (int32 value);

void PhotoR_dark_response(void);

void PhotoR_light_response(void);

void Potenz_response(void);

void Send_packet(void);

/* [] END OF FILE */
