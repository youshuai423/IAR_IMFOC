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
* @brief  Subtraction
* 
*******************************************************************************/
#ifndef _MLIB_SUB_F32_H_
#define _MLIB_SUB_F32_H_

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
#define MLIB_Sub_F32_Ci(f32Min, f32Sub)    MLIB_Sub_F32_FCi(f32Min, f32Sub)
  
/***************************************************************************//*!
*
* f32Out = f32Min - f32Sub
* Without saturation
*******************************************************************************/ 
static inline frac32_t MLIB_Sub_F32_FCi(register frac32_t f32Min, register frac32_t f32Sub)
{
    return(f32Min - f32Sub);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_F32_H_ */
