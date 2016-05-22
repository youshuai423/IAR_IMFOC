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
* @brief  Digital IIR Filter, 3st order
*
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR3_F32_H_
#define _GDFLIB_FILTERIIR3_F32_H_

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
#define GDFLIB_FilterIIR3Init_F16_C(psParam)                                  \
        GDFLIB_FilterIIR3Init_F16_FC(psParam)                                 
#define GDFLIB_FilterIIR3_F16_C(f16InX, psParam)                              \
        GDFLIB_FilterIIR3_F16_FC(f16InX, psParam)
        
/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac32_t f32B0; /* B0 coefficient of an IIR3 filter */
    frac32_t f32B1; /* B1 coefficient of an IIR3 filter */
    frac32_t f32B2; /* B2 coefficient of an IIR3 filter */
    frac32_t f32B3; /* B3 coefficient of an IIR3 filter */
    frac32_t f32A1; /* A1 coefficient of an IIR3 filter */
    frac32_t f32A2; /* A2 coefficient of an IIR3 filter */
    frac32_t f32A3; /* A3 coefficient of an IIR3 filter */
} GDFLIB_FILTER_IIR3_COEFF_T_F32;

typedef struct
{
    GDFLIB_FILTER_IIR3_COEFF_T_F32 sFltCoeff; /* Sub-structure containing filter coefficients. */
    frac32_t f32FltBfrY[3]; /* Internal accumulator buffer */
    frac16_t f16FltBfrX[3]; /* Input buffer of an IIR3 filter */
} GDFLIB_FILTER_IIR3_T_F32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/ 
void GDFLIB_FilterIIR3Init_F16_FC(GDFLIB_FILTER_IIR3_T_F32 *psParam);
frac16_t GDFLIB_FilterIIR3_F16_FC(frac16_t f16InX,
                                  GDFLIB_FILTER_IIR3_T_F32 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR3_F32_H_ */
