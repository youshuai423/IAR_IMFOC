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
#ifndef _MLIB_MSU4_F32_H_
#define _MLIB_MSU4_F32_H_

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
#define MLIB_Msu4_F32ssss_Ci(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)    \
        MLIB_Msu4_F32ssss_FCi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2) 

/***************************************************************************//*!
*
* f32Out = (f16MinMul1 * f16MinMul2) - (f16SubMul1 * f16SubMul2)
* Without saturation
*******************************************************************************/
static inline frac32_t MLIB_Msu4_F32ssss_FCi(register frac16_t f16MinMul1, register frac16_t f16MinMul2,
                                             register frac16_t f16SubMul1, register frac16_t f16SubMul2)
{
    return (((frac32_t)(((int32_t)(f16MinMul1)*(int32_t)(f16MinMul2))<<1)) - 
            ((frac32_t)(((int32_t)(f16SubMul1)*(int32_t)(f16SubMul2))<<1))); 
}

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MSU4_F32_H_ */
