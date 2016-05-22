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
* @brief  Arcus sine function
* 
*******************************************************************************/
#ifndef _GFLIB_ASIN_FLT_H_
#define _GFLIB_ASIN_FLT_H_

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
#define GFLIB_Asin_FLT_C(fltVal) GFLIB_Asin_FLT_FC(fltVal, &fltgflibAsinCoef)
 
/*******************************************************************************
* Types
*******************************************************************************/ 
typedef struct
{
    const float_t  fltA[8];
}   GFLIB_ASIN_T_FLT;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern const GFLIB_ASIN_T_FLT fltgflibAsinCoef;
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Asin_FLT_FC(float_t fltVal,const GFLIB_ASIN_T_FLT *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ASIN_FLT_H_ */
