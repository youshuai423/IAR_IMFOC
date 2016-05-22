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
* @brief  Multiply accumulate of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MAC4RND_F16_ASM_H_
#define _MLIB_MAC4RND_F16_ASM_H_

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
#define MLIB_Mac4Rnd_F16_Asmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)  MLIB_Mac4Rnd_F16_FAsmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)
#define MLIB_Mac4RndSat_F16_Asmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)  MLIB_Mac4RndSat_F16_FAsmi(f16Add1Mul1, f16Add1Mul2, f16Add2Mul1, f16Add2Mul2)
  
/***************************************************************************//*!
*
* f16Out = (f16Add1Mul1 * f16Add1Mul2) + (f16Add2Mul1 * f16Add2Mul2)
* Without saturation
*******************************************************************************/
static inline frac16_t MLIB_Mac4Rnd_F16_FAsmi(register frac16_t f16Add1Mul1, register frac16_t f16Add1Mul2, register frac16_t f16Add2Mul1, register frac16_t f16Add2Mul2)
{
    #if defined(__CC_ARM)                                                          /* For ARM Compiler */
        __asm volatile{ smulbb f16Add1Mul1, f16Add1Mul1, f16Add1Mul2               /* f16Add1Mul1 * f16Add1Mul2 */
                        smlabb f16Add1Mul1, f16Add2Mul1, f16Add2Mul2, f16Add1Mul1  /* f16Add1Mul1 * f16Add1Mul2 + f16Add2Mul1 * f16Add2Mul2 */
                        add f16Add1Mul1, f16Add1Mul1, #0x4000                      /* Rounding */
                        asr f16Add1Mul1, f16Add1Mul1, #15 };                       /* Result >> 15*/
    #else
        __asm volatile( "smulbb %0, %0, %2 \n"             /* f16Add1Mul1 * f16Add1Mul2 */
                        "smlabb %0, %1, %3, %0 \n"         /* f16Add1Mul1 * f16Add1Mul2 + f16Add2Mul1 * f16Add2Mul2 */
                        "add %0, %0, #0x4000 \n"           /* Rounding */
                        "asr %0, %0, #15 \n"               /* Result >> 15 */
                        : "+l"(f16Add1Mul1): "l"(f16Add2Mul1), "l"(f16Add1Mul2), "l"(f16Add2Mul2));
    #endif

    return f16Add1Mul1;
}

/***************************************************************************//*!
*
* f16Out = (f16Add1Mul1 * f16Add1Mul2) + (f16Add2Mul1 * f16Add2Mul2)
* With saturation
*******************************************************************************/
static inline frac16_t MLIB_Mac4RndSat_F16_FAsmi(register frac16_t f16Add1Mul1, register frac16_t f16Add1Mul2, register frac16_t f16Add2Mul1, register frac16_t f16Add2Mul2)
{
    #if defined(__CC_ARM)                                              /* For ARM Compiler */
        __asm volatile{ smulbb f16Add1Mul1, f16Add1Mul1, f16Add1Mul2   /* f16Add1Mul1 * f16Add1Mul2 */
                        smulbb f16Add2Mul1, f16Add2Mul1, f16Add2Mul2   /* f16Add2Mul1 * f16Add2Mul2 */
                        add f16Add1Mul1, f16Add1Mul1, #0x4000          /* Rounding */
                        qadd f16Add1Mul1, f16Add1Mul1, f16Add2Mul1     /* Addition with saturation */
                        ssat f16Add1Mul1, #16, f16Add1Mul1, asr #15 }; /* Result = Result >> 15 */
    #else
        __asm volatile( "smulbb %0, %0, %2 \n"              /* f16Add1Mul1 * f16Add1Mul2 */
                        "smulbb %1, %1, %3 \n"              /* f16Add2Mul1 * f16Add2Mul2 */
                        "add %0, %0, #0x4000 \n"            /* Rounding */
                        "qadd %0, %0, %1 \n"                /* Addition with saturation */
                        "ssat %0, #16, %0, asr #15 \n"      /* Result = Result >> 15 */
                        : "+l"(f16Add1Mul1), "+l"(f16Add2Mul1): "l"(f16Add1Mul2), "l"(f16Add2Mul2));
    #endif

    return f16Add1Mul1;
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4RND_F16_ASM_H_ */
