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
#ifndef _MLIB_SUB_F32_ASM_H_
#define _MLIB_SUB_F32_ASM_H_

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
#define MLIB_SubSat_F32_Asmi(f32Min, f32Sub) MLIB_SubSat_F32_FAsmi(f32Min, f32Sub)

/***************************************************************************//*!
*
* f32Out = f32Min - f32Sub
* With saturation
*******************************************************************************/ 
static inline frac32_t MLIB_SubSat_F32_FAsmi(register frac32_t f32Min, register frac32_t f32Sub)
{
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ qsub f32Min, f32Min, f32Sub};       /* Subtracts with saturation */
    #else
        __asm volatile( "qsub %0, %0, %1 \n"                /* Subtracts with saturation */
                        : "+l"(f32Min): "l"(f32Sub));
    #endif

    return f32Min;
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_F32_ASM_H_ */
