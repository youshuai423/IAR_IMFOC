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
* @brief  Multiply subtract of four inputs with rounding
* 
*******************************************************************************/
#ifndef _MLIB_MSU4RND_F16_H_
#define _MLIB_MSU4RND_F16_H_


#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Sub_F16.h"
#include "MLIB_MulRnd_F16.h"  
#include "MLIB_Sub_F32.h" 

/*******************************************************************************
* Macros
*******************************************************************************/  
#define MLIB_Msu4Rnd_F16_Ci(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)    \
        MLIB_Msu4Rnd_F16_FCi(f16MinMul1, f16MinMul2, f16SubMul1, f16SubMul2)
  
/***************************************************************************//*!
*
* f16Out = (f16MinMul1 * f16MinMul2) - (f16SubMul1 * f16SubMul2)
* Without saturation
*******************************************************************************/
static inline frac16_t MLIB_Msu4Rnd_F16_FCi(register frac16_t f16MinMul1, register frac16_t f16MinMul2, 
                                            register frac16_t f16SubMul1, register frac16_t f16SubMul2)
{
   return(frac16_t)(((MLIB_Sub_F32_Ci((int32_t)(f16MinMul1)*(int32_t)(f16MinMul2), 
                                      (int32_t)(f16SubMul1)*(int32_t)(f16SubMul2))) + 0x4000)>>15);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_MSU4RND_F16_H_ */
