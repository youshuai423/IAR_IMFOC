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
* @brief  Inverse tangent 
* 
*******************************************************************************/
#ifndef _GFLIB_ATAN_F32_H_
#define _GFLIB_ATAN_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Atan_F16_Asm(f16Val) GFLIB_Atan_F16_FAsm(f16Val, &gsAtanCoef)
    
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t  f32A[3];
}GFLIB_ATAN_COEF_T_F32;

typedef struct
{
    GFLIB_ATAN_COEF_T_F32  GFLIB_ATAN_SECTOR_F32[8];
}GFLIB_ATAN_T_F32;

/*******************************************************************************
* Global variables
*******************************************************************************/
extern GFLIB_CONST GFLIB_ATAN_T_F32 gsAtanCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Atan_F16_FAsm(frac16_t f16Val, const GFLIB_ATAN_T_F32 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ATAN_F32_H_ */
