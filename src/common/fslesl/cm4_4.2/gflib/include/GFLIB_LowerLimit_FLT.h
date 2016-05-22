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
* @brief  Lower limit functions with float output
* 
*******************************************************************************/
#ifndef _GFLIB_LOWERLIMIT_FLT_H_
#define _GFLIB_LOWERLIMIT_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_LowerLimit_FLT_Ci(fltVal, fltLLim)                               \
        GFLIB_LowerLimit_FLT_FCi(fltVal, fltLLim)
  
/***************************************************************************//*!
*
* @brief    Lower limit function
*   
* @param    in  float_t fltVal  - Argument in float_t
*               float_t fltLLim - LowerLimit in float_t
*                         
* @return   This function returns - float_t value 
*       
* @remarks  This function trims the argument above or equal to lower fltLLim 
*           limit. 
*
****************************************************************************/ 
static inline float_t GFLIB_LowerLimit_FLT_FCi(float_t fltVal, float_t fltLLim)
{
    if(fltVal < fltLLim)
    {
        fltVal = fltLLim;
    } 
    return(fltVal);
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LOWERLIMIT_FLT_H_ */


