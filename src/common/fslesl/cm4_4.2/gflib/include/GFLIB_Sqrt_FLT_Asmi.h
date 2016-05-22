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
* @brief  Square root 
* 
*******************************************************************************/
#ifndef _MLIB_SQRT_FLT_ASM_H_
#define _MLIB_SQRT_FLT_ASM_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib_FP.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Sqrt_FLT_Asmi(fltVal) GFLIB_Sqrt_FLT_FAsmi(fltVal)
  

/***************************************************************************//*!
* @brief     This function returns the square root of input value.
*
* @param     in fltVal - The input value.
*
* @return    The function returns the square root of the input value. The
*            return value is float range. If input is <= 0, then the function 
*            returns 0. The function uses VSQRT instruction in Cortex M FPU.
*
* @remarks   The function uses VSQRT instruction in Cortex M FPU.
*
****************************************************************************/  
static inline float_t GFLIB_Sqrt_FLT_FAsmi(register float_t fltVal)
{
    #if defined(__CC_ARM)                        /* For ARM Compiler */
        __asm volatile{ vsqrt.f32 fltVal, fltVal };
    #else
        __asm volatile( "vsqrt.f32 %0, %0"
                       : "+t"(fltVal):);
    #endif
    
    return fltVal;
}

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_SQRT_FLT_ASM_H_*/
