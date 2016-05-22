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
* @brief  Multiply 
* 
*******************************************************************************/
#ifndef _MLIB_MUL_F16_H_
#define _MLIB_MUL_F16_H_

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
#define MLIB_Mul_F16_Ci(f16Mult1, f16Mult2)    MLIB_Mul_F16_FCi(f16Mult1, f16Mult2)
#define MLIB_MulNeg_F16_Ci(f16Mult1, f16Mult2) MLIB_MulNeg_F16_FCi(f16Mult1, f16Mult2)
  
/***************************************************************************//*!
*
* f16Out = f16Mult1 * f16Mult2
* Without saturation
*******************************************************************************/ 
static inline frac16_t MLIB_Mul_F16_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return((frac16_t)(((int32_t)(f16Mult1)*(int32_t)(f16Mult2))>>15));
}

/***************************************************************************//*!
*
* f16Out = f16Mult1 * f16Mult2
* 
*******************************************************************************/
static inline frac16_t MLIB_MulNeg_F16_FCi(register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    return((frac16_t)(((int32_t)(f16Mult1)*(int32_t)(-f16Mult2))>>15));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MUL_F16_H_ */
