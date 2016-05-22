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
* @brief  Multiply accumulate with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MACRND_F32_H_
#define _MLIB_MACRND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Sub_F32.h"   

/*******************************************************************************
* Macros
*******************************************************************************/                       
#define MLIB_MacRnd_F32lls_Ci(f32Accum, f32Mult1, f16Mult2)                    \
        MLIB_MacRnd_F32lls_FCi(f32Accum, f32Mult1, f16Mult2)                   

/***************************************************************************//*!
*
* f32Out = f32Accum + ( f32Mult1 * f16Mult2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_MacRnd_F32lls_FCi(register frac32_t f32Accum, 
                                              register frac32_t f32Mult1, register frac16_t f16Mult2)
{
    return((frac32_t)(((frac64_t)f32Mult1*(frac16_t)f16Mult2 + 0x4000)>>15) + (frac32_t)f32Accum);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MACRND_F32_H_ */
