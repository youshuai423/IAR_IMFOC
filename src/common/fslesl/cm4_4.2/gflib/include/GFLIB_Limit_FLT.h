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
* @brief  Limit functions with float output
* 
*******************************************************************************/
#ifndef _GFLIB_LIMIT_FLT_H_
#define _GFLIB_LIMIT_FLT_H_

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
#define GFLIB_Limit_FLT_Ci(fltVal, fltLLim, fltULim)                           \
        GFLIB_Limit_FLT_FCi(fltVal, fltLLim, fltULim)

/***************************************************************************//*!
*
* @brief    Limit function float version
* 
* @param    in   float_t fltVal  - Argument in float_t
*                float_t fltLLim - Lower limit in float_t
*                float_t fltULim - Upper limit in float_t
*                         
* @return   This function returns - float_t value 
*       
* @remarks  This function trims the argument according to the upper fltULim and 
*           lower fltLLim limits. The upper limit must >= lower limit.
*
****************************************************************************/  
static inline float_t GFLIB_Limit_FLT_FCi(float_t fltVal, 
                                          float_t fltLLim, float_t fltULim)
{
    if(fltVal > fltULim) 
    {
        return(fltULim);
    }
    if(fltVal < fltLLim) 
    {
        return(fltLLim);
    }
   
    return(fltVal);
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LIMIT_FLT_H_ */

