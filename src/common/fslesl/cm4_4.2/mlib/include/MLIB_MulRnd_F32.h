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
* @brief  Multiply with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MULRND_F32_H_
#define _MLIB_MULRND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Neg_F32.h"  

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_MulRnd_F32_Ci(f32Mult1, f32Mult2)      MLIB_MulRnd_F32_FCi(f32Mult1, f32Mult2)
#define MLIB_MulRnd_F32ls_Ci(f32Mult1, f16Mult2)    MLIB_MulRnd_F32ls_FCi(f32Mult1, f16Mult2)
#define MLIB_MulNegRnd_F32_Ci(f32Mult1, f32Mult2)   MLIB_MulNegRnd_F32_FCi(f32Mult1, f32Mult2)
#define MLIB_MulNegRnd_F32ls_Ci(f32Mult1, f16Mult2) MLIB_MulNegRnd_F32ls_FCi(f32Mult1, f16Mult2)

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f32Mult2
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_MulRnd_F32_FCi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    register frac32_t f32Temp;

    f32Temp = ((frac64_t)f32Mult1*f32Mult2 + (frac64_t)0x40000000) >> 31;    
    return (frac32_t)f32Temp;
}

/***************************************************************************//*!
*
* f32Out = f32Mult1 * f16Mult2
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_MulRnd_F32ls_FCi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    register frac32_t f32Temp;
    
    f32Temp = ((frac64_t)f32Mult1*(frac16_t)f16Mult2 + 0x4000)>>15;
    return(frac32_t)f32Temp;
}

/***************************************************************************//*!
*
* f32Out = f32Mult1 * -f32Mult2
* 
*******************************************************************************/ 
static inline frac32_t MLIB_MulNegRnd_F32_FCi(register frac32_t f32Mult1, register frac32_t f32Mult2)
{
    int64_t i64Mult1; 
    int64_t i64Mult2; 
    
    i64Mult1 = (int64_t)f32Mult1;
    i64Mult2 = (int64_t)f32Mult2;
    return((frac32_t) (((i64Mult1*(-i64Mult2))+0x40000000)>>31));
}

/***************************************************************************//*!
*
* f32Out = f32Mult1 * -f16Mult2
* 
*******************************************************************************/
static inline frac32_t MLIB_MulNegRnd_F32ls_FCi(register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    frac32_t f32Temp1;
    
    f32Temp1 = MLIB_Neg_F32_Ci((int32_t)(f16Mult2));
    return(((frac64_t)f32Mult1*(frac32_t)f32Temp1 + 0x4000)>>15);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MULRND_F32_H_ */
