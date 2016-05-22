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
* @brief  Negation
* 
*******************************************************************************/
#ifndef _MLIB_NEG_F16_ASM_H_
#define _MLIB_NEG_F16_ASM_H_

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
#define MLIB_NegSat_F16_Asmi(f16Val) MLIB_NegSat_F16_FAsmi(f16Val)

/***************************************************************************//*!
*
* f16Out = -f16Val
* With saturation
*******************************************************************************/
static inline frac16_t MLIB_NegSat_F16_FAsmi(register frac16_t f16Val)
{
    register frac16_t f16Out = 0;
    #if defined(__CC_ARM)                                 /* For ARM Compiler */
        __asm volatile{ qsub16 f16Out, f16Out, f16Val };  /* f16Out = 0 - f16Val */
    #else
        __asm volatile( "qsub16 %0, %0, %1 \n"            /* f16Out = 0 - f16Val  */
                        : "+l"(f16Out): "l"(f16Val));
    #endif

    return f16Out;
}


#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_NEG_F16_ASM_H_ */
