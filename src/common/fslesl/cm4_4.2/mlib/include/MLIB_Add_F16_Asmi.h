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
#ifndef _MLIB_ADD_F16_ASM_H_
#define _MLIB_ADD_F16_ASM_H_

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
#define MLIB_AddSat_F16_Asmi(f16Add1, f16Add2) MLIB_AddSat_F16_FAsmi(f16Add1, f16Add2)
  
/***************************************************************************//*!
*  Saturated Addition
*     f16Out = f16Add1 + f16Add2
*******************************************************************************/
static inline frac16_t MLIB_AddSat_F16_FAsmi(register frac16_t f16Add1, register frac16_t f16Add2)
{
    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        __asm volatile{ qadd16 f16Add1, f16Add1, f16Add2};  /* Adds with saturation */
    #else
        __asm volatile( "qadd16 %0, %0, %1 \n"              /* Adds with saturation */
                        : "+l"(f16Add1): "l"(f16Add2));
    #endif

    return f16Add1;
}



#if defined(__cplusplus)
}
#endif

#endif  /*_MLIB_ADD_F16_ASM_H_*/
