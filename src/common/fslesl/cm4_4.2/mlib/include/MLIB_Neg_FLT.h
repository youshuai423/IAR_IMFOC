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
* @brief  Negation
* 
*******************************************************************************/
#ifndef _MLIB_NEG_FLT_H_
#define _MLIB_NEG_FLT_H_

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
#define MLIB_Neg_FLT_Ci(fltVal) MLIB_Neg_FLT_FCi(fltVal) 
  
/***************************************************************************//*!
*
* fltOut = -fltVal
* The output saturation is not implemented, thus in case 
* the negation of input value is outside the (-2^128, 2^128) 
* interval, the output value will overflow without any detection.
*******************************************************************************/ 
static inline float_t MLIB_Neg_FLT_FCi(register float_t fltVal)
{
    return((float_t)(-fltVal));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_NEG_FLT_H_ */
