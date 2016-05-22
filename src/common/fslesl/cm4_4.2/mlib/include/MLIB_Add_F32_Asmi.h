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
* @brief  Addition
* 
*******************************************************************************/
#ifndef _MLIB_ADD_F32_ASM_H_
#define _MLIB_ADD_F32_ASM_H_

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
#define MLIB_AddSat_F32_Asmi(f32Add1, f32Add2) MLIB_AddSat_F32_FAsmi(f32Add1, f32Add2)

/***************************************************************************//*!
*  Saturated Addition
*     f32Out = f32Add1 + f32Add2
*******************************************************************************/
static inline frac32_t MLIB_AddSat_F32_FAsmi(register frac32_t f32Add1, register frac32_t f32Add2)
{
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ qadd f32Add1, f32Add1, f32Add2};    /* Adds with saturation */
    #else
        __asm volatile( "qadd %0, %0, %1 \n"                /* Adds with saturation */
                        : "+l"(f32Add1): "l"(f32Add2));
    #endif

    return f32Add1;
}


#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD_F32_ASM_H_ */
