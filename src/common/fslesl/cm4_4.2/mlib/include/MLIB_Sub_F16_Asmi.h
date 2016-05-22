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
* @brief  Subtraction
* 
*******************************************************************************/
#ifndef _MLIB_SUB_F16_ASM_H_
#define _MLIB_SUB_F16_ASM_H_

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
#define MLIB_SubSat_F16_Asmi(f16Min, f16Sub) MLIB_SubSat_F16_FAsmi(f16Min, f16Sub)

/***************************************************************************//*!
*
* f16Out = f16Min - f16Sub
* With saturation
*******************************************************************************/
static inline frac16_t MLIB_SubSat_F16_FAsmi(register frac16_t f16Min, register frac16_t f16Sub)
{
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ qsub16 f16Min, f16Min, f16Sub};     /* Subtracts with saturation */
    #else
        __asm volatile( "qsub16 %0, %0, %1 \n"              /* Subtracts with saturation */
                        : "+l"(f16Min): "l"(f16Sub));
    #endif

    return f16Min;
}


#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_F16_ASM_H_ */
