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
* @brief  Upper limit functions with float output
* 
*******************************************************************************/
#ifndef _GFLIB_UPPERLIMIT_FLT_H_
#define _GFLIB_UPPERLIMIT_FLT_H_

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
#define GFLIB_UpperLimit_FLT_Ci(fltVal, fltULim)                               \
        GFLIB_UpperLimit_FLT_FCi(fltVal, fltULim)
  
/***************************************************************************//*!
*
* @brief    Upper limit function
*   
* @param    in  float_t fltVal - Argument in float_t
*               float_t fltULim - UpperLimit in float_t
*                         
* @return   This function returns - float_t value 
*       
* @remarks  This function trims the argument under or equal to upper fltULim 
*           limit. 
*
****************************************************************************/  
static inline float_t GFLIB_UpperLimit_FLT_FCi(float_t fltVal, float_t fltULim)
{
    if(fltVal > fltULim)
    {
        fltVal = fltULim;
    } 
    return(fltVal);
}
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_UPPERLIMIT_FLT_H_ */



