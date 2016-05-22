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
* @brief  Absolute Value
* 
*******************************************************************************/
#ifndef _MLIB_ABS_FLT_ASMI_H_
#define _MLIB_ABS_FLT_ASMI_H_

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
#define MLIB_Abs_FLT_Asmi(fltVal) MLIB_Abs_FLT_FAsmi(fltVal)
  
/***************************************************************************//*!
*  Absolute value      
*  fltOut = |fltIn|
*  The output saturation is not implemented.
*******************************************************************************/
static inline float_t MLIB_Abs_FLT_FAsmi(register float_t fltVal)
{
    #if defined(__CC_ARM)                                 /* For ARM Compiler */
        __asm volatile{ vabs.f32 fltVal, fltVal };        /* Absolute value */
    #else
        __asm volatile( "vabs.f32 %0, %0 "
                       : "+t"(fltVal):);
    #endif

    return fltVal;
}

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ABS_FLT_ASMI_H_*/
