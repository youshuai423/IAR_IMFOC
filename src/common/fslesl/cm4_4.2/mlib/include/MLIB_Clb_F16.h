/*******************************************************************************
*
* Copyright 2007-2015 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
* 
*
****************************************************************************//*!
*
* @brief  Count of leading bits
* 
*******************************************************************************/
#ifndef _MLIB_CLB_F16_H_
#define _MLIB_CLB_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#if defined(__IAR_SYSTEMS_ICC__)           /* IAR compiler   */
#define _clz(x) __CLZ(x)
#else
#define _clz(x) __builtin_clz(x)
#endif
/*******************************************************************************
* Macros 
*******************************************************************************/  
#define MLIB_Clb_U16s_Ci(f16Val) MLIB_Clb_U16s_FCi(f16Val)
  
/***************************************************************************//*!
* count of leading bits
*  - first, the absolute value of the input is calculated
*  - then the amount of zero bits before the first non-zero bits is counted (sign bit is not included)
****************************************************************************/
static inline uint16_t MLIB_Clb_U16s_FCi(register frac16_t f16Val)
{
    register frac16_t f16Temp;
    
    f16Temp = ((f16Val < 0) ? (~f16Val): f16Val);
    return (uint16_t)(_clz(f16Temp) - 0x11);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_CLB_F16_H_ */
