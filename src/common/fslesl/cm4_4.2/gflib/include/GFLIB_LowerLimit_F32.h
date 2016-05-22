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
* @brief  Lower limit functions with 32-bit fractional output
* 
*******************************************************************************/
#ifndef _GFLIB_LOWERLIMIT_F32_H_
#define _GFLIB_LOWERLIMIT_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gflib_types.h"
  
/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_LowerLimit_F32_Ci(f32Val, f32LLim)                              \
        GFLIB_LowerLimit_F32_FCi(f32Val, f32LLim)

/***************************************************************************//*!
*
* @brief    Lower limit function
*   
* @param    in   frac32_t f32Val  - Argument in <-1;1) in frac32_t
*                frac32_t f32LLim - LowerLimit in <-1;1) in frac32_t
*                         
* @return   This function returns - frac32_t value <-1;1)
*       
* @remarks  This function trims the argument above or equal to lower f32LLim 
*           limit. 
*
****************************************************************************/    
static inline frac32_t GFLIB_LowerLimit_F32_FCi(frac32_t f32Val, frac32_t f32LLim)
{
    if(f32Val < f32LLim)
    {
        f32Val = f32LLim;
    } 
    return(f32Val);
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LOWERLIMIT_F32_H_ */


