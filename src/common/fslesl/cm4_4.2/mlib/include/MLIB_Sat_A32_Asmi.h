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
* @brief  Saturation
* 
*******************************************************************************/
#ifndef _MLIB_SAT_A32_ASM_H_
#define _MLIB_SAT_A32_ASM_H_

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
#define MLIB_Sat_F16a_Asmi(a32Accum) MLIB_Sat_F16a_FAsmi(a32Accum)
  
/***************************************************************************//*!
*
* Takes the fractional part from number in accumulator format and saturate if the original number is greater than 1. 
* 
*******************************************************************************/ 
static inline frac16_t MLIB_Sat_F16a_FAsmi(register acc32_t a32Accum)
{
    #if defined(__CC_ARM)                               /* For ARM Compiler */
        __asm volatile{ ssat a32Accum, #16, a32Accum }; /* Saturation */

    #else
        __asm volatile( "ssat %0, #16, %0 \n"           /* Saturation */
                        : "+l"(a32Accum):);
    #endif

    return (frac16_t)a32Accum;
}


#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SAT_A32_ASM_H_ */
