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
* @brief  Multiply accumulate of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MAC4RND_F32_H_
#define _MLIB_MAC4RND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Add_F32.h"
#include "MLIB_MulRnd_F32.h" 

/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Mac4Rnd_F32_Ci(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2)    \
        MLIB_Mac4Rnd_F32_FCi(f32Add1Mul1, f32Add1Mul2, f32Add2Mul1, f32Add2Mul2) 
  
/***************************************************************************//*!
*
* f32Out = (f32Add1Mul1 * f32Add1Mul2) + (f32Add2Mul1 * f32Add2Mul2)
* Without saturation
*******************************************************************************/  
static inline frac32_t MLIB_Mac4Rnd_F32_FCi(register frac32_t f32Add1Mul1, register frac32_t f32Add1Mul2, 
                                            register frac32_t f32Add2Mul1, register frac32_t f32Add2Mul2)
{
    return(frac32_t)((((int64_t)(f32Add1Mul1)*(int64_t)(f32Add1Mul2) + 
                       (int64_t)(f32Add2Mul1)*(int64_t)(f32Add2Mul2))+ 0x40000000)>>31);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MAC4RND_F32_H_ */
