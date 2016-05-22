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
* @brief  Multiply accumulate
* 
*******************************************************************************/
#ifndef _MLIB_MAC_A32_H_
#define _MLIB_MAC_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_Mac_A32ass_Ci(a32Accum, f16Mult1, f16Mult2)                     \
        MLIB_Mac_A32ass_FCi(a32Accum, f16Mult1, f16Mult2)  
  
/***************************************************************************//*!
*
* a32Out = a32Accum + ( f16Mult1 * f16Mult2 )
*
*******************************************************************************/
static inline acc32_t MLIB_Mac_A32ass_FCi(register acc32_t a32Accum, 
                                          register frac16_t f16Mult1,register frac16_t f16Mult2)
{   
    register acc32_t a32Temp;
    
    a32Temp = ((int32_t)f16Mult1 * (int32_t)f16Mult2);
    a32Temp = a32Temp >> 15;
    return (a32Accum + a32Temp);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC_A32_H_ */
