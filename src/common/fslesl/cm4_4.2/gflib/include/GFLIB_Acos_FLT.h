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
* @brief  Arcus cosine function
* 
*******************************************************************************/
#ifndef _GFLIB_ACOS_FLT_H_
#define _GFLIB_ACOS_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"  
#include "gflib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Acos_FLT_C(fltVal) GFLIB_Acos_FLT_FC(fltVal, &fltgflibAcosCoef) 
  
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    const float_t  fltA[5];   /* Array of approximation coefficients */
}   GFLIB_ACOS_T_FLT;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern const GFLIB_ACOS_T_FLT fltgflibAcosCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Acos_FLT_FC(float_t fltVal,const GFLIB_ACOS_T_FLT *const psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ACOS_FLT_H_ */

