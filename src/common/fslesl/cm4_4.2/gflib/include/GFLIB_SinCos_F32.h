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
* @brief  Sine, Cosine
* 
*******************************************************************************/
#ifndef _GFLIB_SINCOS_A32_H_
#define _GFLIB_SINCOS_A32_H_

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
#define GFLIB_Sin_F16_Asm(f16Angle)  GFLIB_Sin_F16_FAsm(f16Angle, &gsSinCoef)
#define GFLIB_Cos_F16_Asmi(f16Angle) GFLIB_Cos_F16_FAsmi(f16Angle)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t  f32A[5];    /* Array of fractional coefficients */
} GFLIB_SIN_T_F32;

/*******************************************************************************
* Global variables
*******************************************************************************/  
extern GFLIB_CONST GFLIB_SIN_T_F32 gsSinCoef;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Sin_F16_FAsm(frac16_t f16Angle, const GFLIB_SIN_T_F32 *psParam);

/***************************************************************************//*!
* Cosine is calculated using the GFLIB_Sin_F16 function summed with FRAC(0.5)
*******************************************************************************/  
static inline frac16_t GFLIB_Cos_F16_FAsmi(register frac16_t f16Angle)
{
    return(GFLIB_Sin_F16_Asm((f16Angle +(frac16_t)16384)));
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SINCOS_A32_H_ */

