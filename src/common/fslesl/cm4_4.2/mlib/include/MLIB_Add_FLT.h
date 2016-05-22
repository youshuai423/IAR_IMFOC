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
* @brief  Addition
* 
*******************************************************************************/
#ifndef _MLIB_ADD_FLT_H_
#define _MLIB_ADD_FLT_H_

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
#define MLIB_Add_FLT_Ci(fltAdd1, fltAdd2) MLIB_Add_FLT_FCi(fltAdd1, fltAdd2)  

/***************************************************************************//*!
*  Addition        
*  fltOut = fltAdd1 + fltAdd2
*  The output saturation is not implemented.
*******************************************************************************/ 
static inline float_t MLIB_Add_FLT_FCi(register float_t fltAdd1, register float_t fltAdd2)
{
    return((float_t)(fltAdd1 + fltAdd2));
}

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_ADD_FLT_H_ */
