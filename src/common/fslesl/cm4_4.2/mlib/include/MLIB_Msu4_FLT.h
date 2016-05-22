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
* @brief  Multiply subtract of four inputs
* 
*******************************************************************************/
#ifndef _MLIB_MSU4_FLT_H_
#define _MLIB_MSU4_FLT_H_

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
#define MLIB_Msu4_FLT_Ci(fltMinMul1, fltMinMul2, fltSubMul1, fltSubMul2)       \
        MLIB_Msu4_FLT_FCi(fltMinMul1, fltMinMul2, fltSubMul1, fltSubMul2) 

/***************************************************************************//*!
*
* fltOut = (fltMinMul1 * fltMinMul2) - (fltSubMul1 * fltSubMul2)
* The output saturation is not implemented, thus in case 
* the MSU4 of input values is outside the (-2^128, 2^128) 
* interval, the output value will overflow without any detection. 
*******************************************************************************/
static inline float_t MLIB_Msu4_FLT_FCi(register float_t fltMinMul1, register float_t fltMinMul2, 
                                        register float_t fltSubMul1, register float_t fltSubMul2)
{
    return (float_t)(((float_t)fltMinMul1 * (float_t)fltMinMul2) -
                     ((float_t)fltSubMul1 * (float_t)fltSubMul2));  
}

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MSU4_FLT_H_ */
