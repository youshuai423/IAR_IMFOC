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
#ifndef _MLIB_NEG_F32_H_
#define _MLIB_NEG_F32_H_

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
#define MLIB_Neg_F32_Ci(f32Val)    MLIB_Neg_F32_FCi(f32Val)  
  
/***************************************************************************//*!
*
* f32Out = -f32Val
* Without saturation
*******************************************************************************/ 
static inline frac32_t MLIB_Neg_F32_FCi(register frac32_t f32Val)
{
    return((frac32_t)(-f32Val));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_NEG_F32_H_ */
