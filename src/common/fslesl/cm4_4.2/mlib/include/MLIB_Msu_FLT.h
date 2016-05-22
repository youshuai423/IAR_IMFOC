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
* @brief  Multiply subtract
* 
*******************************************************************************/
#ifndef _MLIB_MSU_FLT_H_
#define _MLIB_MSU_FLT_H_

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
#define MLIB_Msu_FLT_Ci(fltAccum, fltMult1, fltMult2)                          \
        MLIB_Msu_FLT_FCi(fltAccum, fltMult1, fltMult2)
  
/***************************************************************************//*!
*
* fltOut = fltAccum - ( fltMult1 * fltMult2)
* The output saturation is not implemented, thus in case 
* the MSU of input values is outside the (-2^128, 2^128) 
* interval, the output value will overflow without any detection.
*******************************************************************************/  
static inline float_t MLIB_Msu_FLT_FCi(register float_t fltAccum, 
                                       register float_t fltMult1, register float_t fltMult2)
{
    return((float_t) (fltAccum - (fltMult1 * fltMult2)));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSU_FLT_H_ */
