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
* @brief  Rounding
* 
*******************************************************************************/
#ifndef _MLIB_RND_F32_H_
#define _MLIB_RND_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Add_F32.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Rnd_F16l_Ci(f32Val)    MLIB_Rnd_F16l_FCi(f32Val) 
  
/***************************************************************************//*!
*
* Converts 32-bit number into 16-bit with rounding
* Without saturation
*******************************************************************************/   
static inline frac16_t MLIB_Rnd_F16l_FCi(register frac32_t f32Val)
{
    return((frac16_t)(MLIB_Add_F32_Ci(f32Val, (frac32_t)0x00008000)>>16));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_RND_F32_H_ */
