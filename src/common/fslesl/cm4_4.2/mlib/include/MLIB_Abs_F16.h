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
#ifndef _MLIB_ABS_F16_H_
#define _MLIB_ABS_F16_H_

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
#define MLIB_Abs_F16_Ci(f16Val)    MLIB_Abs_F16_FCi(f16Val)
  
/***************************************************************************//*!
*  Absolute value      
*  f16Out = |f16In|
*  not saturated
*******************************************************************************/
static inline frac16_t MLIB_Abs_F16_FCi(register frac16_t f16Val)
{
    return((f16Val < (frac16_t)0) ? (-f16Val) : (f16Val));
}

#if defined(__cplusplus)
}
#endif

#endif /*_MLIB_ABS_F16_H_*/
