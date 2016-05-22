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
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_F32_ASM_H_
#define _MLIB_MULRND_F32_ASM_H_

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
#define MLIB_MulRndSat_F32_Asmi(f32Mult1, f32Mult2) MLIB_MulRndSat_F32_FAsmi(f32Mult1, f32Mult2)
#define MLIB_MulRndSat_F32ls_Asmi(f32Mult1, f16Mult2) MLIB_MulRndSat_F32ls_FAsmi(f32Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* With saturation
*******************************************************************************/  
static inline frac32_t MLIB_MulRndSat_F32_FAsmi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smull f32Mult2, f32Mult1, f32Mult1, f32Mult2 /* f32Mult1 * f32Mult2 */
                        lsrs f32Mult2, f32Mult2, #31                 /* f32Mult2 >> 31 */
                        adc f32Mult2, f32Mult2, #0                   /* Rounding */
                        qdadd f32Mult1, f32Mult2, f32Mult1 };        /* 2 * f32Mult1 + f32Mult2 with saturation */
    #else
        __asm volatile( "smull %1, %0, %0, %1 \n"                    /* f32Mult1:f32Mult2 = f32Mult1 * f32Mult2 */
                        "lsrs %1, %1, #31 \n"                        /* f32Mult2 >> 31 */
                        "adc %1, %1, #0 \n"                          /* Rounding */
                        "qdadd %0, %1, %0 \n"                        /* 2 * f32Mult1 + f32Mult2 with saturation */
                        : "+l"(f32Mult1), "+l"(f32Mult2):);
    #endif

    return f32Mult1;
}

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f16Mult2
* With saturation
*******************************************************************************/
static inline frac32_t MLIB_MulRndSat_F32ls_FAsmi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ lsl f16Mult2, f16Mult2, #16                  /* f16Mult2 << 16 */
                        smull f16Mult2, f32Mult1, f32Mult1, f16Mult2 /* f32Mult1 * f16Mult2 */
                        lsrs f16Mult2, f16Mult2, #31                 /* f16Mult2 >> 31 */
                        adc f16Mult2, f16Mult2, #0                   /* Rounding */
                        qdadd f32Mult1, f16Mult2, f32Mult1 };        /* 2 * f32Mult1 + f16Mult2 with saturation */
    #else
        __asm volatile( "lsl %1, %1, #16 \n"                         /* f16Mult2 << 16 */
                        "smull %1, %0, %0, %1 \n"                    /* f32Mult1:f16Mult2 = f32Mult1 * f16Mult2 */
                        "lsrs %1, %1, #31 \n"                        /* f16Mult2 >> 31 */
                        "adc %1, %1, #0 \n"                          /* Rounding */
                        "qdadd %0, %1, %0 \n"                        /* 2 * f32Mult1 + f16Mult2 with saturation */
                        : "+l"(f32Mult1), "+l"(f16Mult2):);
    #endif

    return f32Mult1;
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_F32_ASM_H_ */
