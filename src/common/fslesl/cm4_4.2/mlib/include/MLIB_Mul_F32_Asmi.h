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
* @brief  Multiply 
* 
*******************************************************************************/
#ifndef _MLIB_MUL_F32_ASM_H_
#define _MLIB_MUL_F32_ASM_H_

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
#define MLIB_MulSat_F32_Asmi(f32Mult1, f32Mult2) MLIB_MulSat_F32_FAsmi(f32Mult1, f32Mult2)
#define MLIB_MulSat_F32ss_Asmi(f16Mult1, f16Mult2) MLIB_MulSat_F32ss_FAsmi(f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* With saturation
*******************************************************************************/  
static inline frac32_t MLIB_MulSat_F32_FAsmi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult1, f32Mult2 /* f32Mult1 * f32Mult2 */
                        lsr f32Mult2, f32Mult2, #31                  /* f32Mult2 >> 31 */
                        qdadd f32Mult1, f32Mult2, f32Mult1 };        /* Converts result to frac32 and saturates */
    #else
        __asm volatile( "smull %1, %0, %0, %1 \n"                    /* f32Mult1 * f32Mult2 */
                        "lsr %1, %1, #31 \n"                         /* f32Mult2 >> 31 */
                        "qdadd %0, %1, %0 \n"                        /* Converts result to frac32 and saturates */
                        : "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Mult1;
}

/***************************************************************************//*!
*
* f32Out = f16Mult1 * f16Mult2
* With saturation
*******************************************************************************/
static inline frac32_t MLIB_MulSat_F32ss_FAsmi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    register frac32_t f32Out;
    #if defined(__CC_ARM)                                        /* For ARM Compiler */
        __asm volatile{ smulbb f16Mult1, f16Mult1, f16Mult2      /* f16Mult1 * f16Mult2 */
                        qadd f32Out, f16Mult1, f16Mult1 };       /* Converts result to fractional and saturates */
    #else
        __asm volatile( "smulbb %1, %1, %2 \n"                   /* f16Mult1 * f16Mult2 */
                        "qadd %0, %1, %1 \n"                     /* Converts result to fractional and saturates */
                        : "=l"(f32Out), "+l"(f16Mult1): "l"(f16Mult2));
    #endif

    return f32Out;
}


#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_F32_ASM_H_ */
