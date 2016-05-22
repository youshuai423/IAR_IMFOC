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
#ifndef _MLIB_NEG_F16_H_
#define _MLIB_NEG_F16_H_

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
#define MLIB_Neg_F16_Ci(f16Val)    MLIB_Neg_F16_FCi(f16Val) 
  
/***************************************************************************//*!
*
* f16Out = -f16Val
* Without saturation
*******************************************************************************/
static inline frac16_t MLIB_Neg_F16_FCi(register frac16_t f16Val)
{
    return((frac16_t)(-f16Val)); 
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_NEG_F16_H_ */
