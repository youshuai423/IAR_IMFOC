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
* @brief  Digital IIR Filter, 2st order 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR2_F32_H_
#define _GDFLIB_FILTERIIR2_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gdflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterIIR2Init_F16_C(psParam)                                  \
        GDFLIB_FilterIIR2Init_F16_FC(psParam)                                                             
#define GDFLIB_FilterIIR2_F16_Asm(f16InX, psParam)                            \
        GDFLIB_FilterIIR2_F16_FAsm(f16InX, psParam)
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32B0; /* B0 coefficient of an IIR2 filter */
    frac32_t f32B1; /* B1 coefficient of an IIR2 filter */
    frac32_t f32B2; /* B2 coefficient of an IIR2 filter */
    frac32_t f32A1; /* A1 coefficient of an IIR2 filter */
    frac32_t f32A2; /* A2 coefficient of an IIR2 filter */
} GDFLIB_FILTER_IIR2_COEFF_T_F32;

typedef struct
{
    GDFLIB_FILTER_IIR2_COEFF_T_F32 sFltCoeff; /* Sub-structure containing filter coefficients. */
    frac32_t f32FltBfrY[2]; /* Internal accumulator buffer */    
    frac16_t f16FltBfrX[2]; /* Input buffer of an IIR2 filter */
} GDFLIB_FILTER_IIR2_T_F32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/ 
void GDFLIB_FilterIIR2Init_F16_FC(GDFLIB_FILTER_IIR2_T_F32 *psParam);

frac16_t GDFLIB_FilterIIR2_F16_FAsm(frac16_t f16InX,
                                  GDFLIB_FILTER_IIR2_T_F32 *psParam);
                                  
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR2_F32_H_ */
