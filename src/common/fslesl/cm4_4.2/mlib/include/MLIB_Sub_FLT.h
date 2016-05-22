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
#ifndef _MLIB_SUB_FLT_H_
#define _MLIB_SUB_FLT_H_

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
#define MLIB_Sub_FLT_Ci(fltMin, fltSub) MLIB_Sub_FLT_FCi(fltMin, fltSub)
  
/***************************************************************************//*!
*
*  fltOut = fltMin - fltSub
*  The output saturation is not implemented, thus in case 
*  the substraction of input value is outside the (-2^128, 2^128) 
*  interval, the output value will overflow without any detection.
*******************************************************************************/ 
static inline float_t MLIB_Sub_FLT_FCi(register float_t fltMin, register float_t fltSub)
{
    return(fltMin - fltSub);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_FLT_H_ */
