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
#ifndef _MLIB_ABS_F32_H_
#define _MLIB_ABS_F32_H_

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
#define MLIB_Abs_F32_Ci(f32Val)    MLIB_Abs_F32_FCi(f32Val)
  
/***************************************************************************//*!
*  Absolute value      
*  32Out = |f32In|
*  not saturated
*******************************************************************************/
static inline frac32_t MLIB_Abs_F32_FCi(register frac32_t f32Val)
{
    return((f32Val < (frac32_t)0) ? (-f32Val) : (f32Val));
}

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ABS_F32_H_*/
