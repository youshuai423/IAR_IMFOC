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
* @brief  Conversion functions
* 
*******************************************************************************/
#ifndef _MLIB_CONV_F32_H_
#define _MLIB_CONV_F32_H_

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
#define MLIB_Conv_F32s_Ci(f16Val) MLIB_Conv_F32s_FCi(f16Val)
#define MLIB_Conv_F16l_Ci(f32Val) MLIB_Conv_F16l_FCi(f32Val)  

/***************************************************************************//*!
*
* @brief  16-bit fractional to 32-bit fractional convert function
*
* @param  in  frac16_t f16Val - Argument in <-1;1) in frac16_t
*
* @return This function returns - frac32_t value <-1;1)
*       
* @remarks  This function convert the lower 16-bits of the 32-bit input and
*           returns the upper 16-bit. 
*
*******************************************************************************/
static inline frac32_t MLIB_Conv_F32s_FCi(register frac16_t f16Val)
{
  return((frac32_t)((frac32_t)f16Val << ((uint16_t)16)));
}

/***************************************************************************//*!
*
* @brief  32-bit fractional to 16-bit fractional convert function
*
* @param  in  frac32_t f32Val - Argument in <-1;1) in frac32_t
*
* @return This function returns - frac16_t value <-1;1)
*       
* @remarks  This function convert the lower 16-bits of the 32-bit input and
*           returns the upper 16-bit. 
*
*******************************************************************************/
static inline frac16_t MLIB_Conv_F16l_FCi(register frac32_t f32Val)
{
  return((frac16_t)(f32Val >> ((uint16_t)16)));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_CONV_F32_H_ */
