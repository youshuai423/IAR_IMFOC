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
* @brief  Multiply accumulate of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MAC4_F32_ASM_H_
#define _MLIB_MAC4_F32_ASM_H_

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
#define MLIB_Mac4Sat_F32ssss_Asmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)  MLIB_Mac4Sat_F32ssss_FAsmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)

/***************************************************************************//*!
*
* f32Out = (f16Add1Mul1 * f16Add1Mul2) + (f16Add2Mul1 * f16Add2Mul2)
* With saturation
*******************************************************************************/
static inline frac32_t MLIB_Mac4Sat_F32ssss_FAsmi(register frac16_t f16Add1Mul1, register frac16_t f16Add1Mul2, register frac16_t f16Add2Mul1, register frac16_t f16Add2Mul2)
{
    register frac32_t f32Out;
    #if defined(__CC_ARM)                                            /* For ARM Compiler */
        __asm volatile{ smulbb f16Add1Mul1, f16Add1Mul1, f16Add1Mul2 /* f16Add1Mul1 * f16Add1Mul2 */
                        smulbb f32Out, f16Add2Mul1, f16Add2Mul2      /* f16Add2Mul1 * f16Add2Mul2 */
                        qadd f32Out, f32Out, f16Add1Mul1             /* Addition with saturation */
                        qadd f32Out, f32Out, f32Out };               /* Addition with saturation */
    #else
        __asm volatile( "smulbb %1, %1, %2 \n"                       /* f16Add1Mul1 * f16Add1Mul2 */
                        "smulbb %2, %3, %4 \n"                       /* f16Add2Mul1 * f16Add2Mul2 */
                        "qadd %2, %2, %1 \n"                         /* Addition with saturation */
                        "qadd %0, %2, %2 \n"                         /* Addition with saturation */
                        : "=l"(f32Out), "+l"(f16Add1Mul1), "+l"(f16Add1Mul2): "l"(f16Add2Mul1), "l"(f16Add2Mul2));
    #endif

    return f32Out;
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4_F32_ASM_H_ */
