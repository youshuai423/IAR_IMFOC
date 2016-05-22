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
* @brief  Tangent
* 
*******************************************************************************/
#ifndef _GFLIB_TAN_F32_H_
#define _GFLIB_TAN_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/  
#include "mlib.h"
#include "gflib.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Tan_F16_C(f16Angle) GFLIB_Tan_F16_FC(f16Angle, &gsTanCoef)  
  
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    const frac32_t f32A[4];
}   GFLIB_TAN_COEF_T_F32;

typedef struct
{
    GFLIB_TAN_COEF_T_F32  GFLIB_TAN_SECTOR_F32[8];
}   GFLIB_TAN_T_F32;
  
/*******************************************************************************
* Global variables
*******************************************************************************/  
extern const GFLIB_TAN_T_F32 gsTanCoef;
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Tan_F16_FC(frac16_t f16Angle,
                                 const GFLIB_TAN_T_F32 *const pParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_TAN_F32_H_ */
