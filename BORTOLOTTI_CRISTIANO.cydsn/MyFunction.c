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

#include "MyFunction.h"

int32 Bound_sample (int32 value)
{
    if (value < 0) value = 0;
    if (value > 65535) value = 65535;
    return value;
}

/* [] END OF FILE */
