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
* @brief  Multiply subtract of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MSU4_F32_ASM_H_
#define _MLIB_MSU4_F32_ASM_H_

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
#define MLIB_Msu4Sat_F32ssss_Asmi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)  MLIB_Msu4Sat_F32ssss_FAsmi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)

/***************************************************************************//*!
*
* f32Out = (f16MinMul1 * f16MinMul2) - (f16SubMul1 * f16SubMul2)
* With saturation
*******************************************************************************/
static inline frac32_t MLIB_Msu4Sat_F32ssss_FAsmi(register frac16_t f16MinMul1,register frac16_t f16MinMul2,register frac16_t f16SubMul1,register frac16_t f16SubMul2)
{
    register frac32_t f32Out;
    #if defined(__CC_ARM)                                         /* For ARM Compiler */
        __asm volatile{ smulbb f32Out, f16MinMul1, f16MinMul2     /* f16MinMul1 * f16MinMul2 */
                        smulbb f16MinMul1, f16SubMul1, f16SubMul2 /* f16SubMul1 * f16SubMul2 */
                        qsub f32Out, f32Out, f16MinMul1           /* Subtraction with saturation */
                        qadd f32Out, f32Out, f32Out };            /* Addition with saturation */
    #else
        __asm volatile( "smulbb %1, %1, %2 \n"                    /* f16MinMul1 * f16MinMul2 */
                        "smulbb %2, %3, %4 \n"                    /* f16SubMul1 * f16SubMul2 */
                        "qsub %2, %1, %2 \n"                      /* Subtraction with saturation */
                        "qadd %0, %2, %2 \n"                      /* Addition with saturation */
                        : "=l"(f32Out), "+l"(f16MinMul1), "+l"(f16MinMul2): "l"(f16SubMul1), "l"(f16SubMul2));
    #endif

    return f32Out;
}


#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MSU4_F32_ASM_H_ */
