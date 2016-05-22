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
* @brief  Digital Float IIR Filter, 3st order
*
*******************************************************************************/
#ifndef _GDFLIB_FILTERIIR3_FLT_H_
#define _GDFLIB_FILTERIIR3_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gdflib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define GDFLIB_FilterIIR3Init_FLT_C(psParam)                                   \
        GDFLIB_FilterIIR3Init_FLT_FC(psParam)                                  
#define GDFLIB_FilterIIR3_FLT_C(fltInX, psParam)                               \
        GDFLIB_FilterIIR3_FLT_FC(fltInX, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t fltB0; /* B0 coefficient of an IIR3 filter */
    float_t fltB1; /* B1 coefficient of an IIR3 filter */
    float_t fltB2; /* B2 coefficient of an IIR3 filter */
    float_t fltB3; /* B3 coefficient of an IIR3 filter */
    float_t fltA1; /* A1 coefficient of an IIR3 filter */
    float_t fltA2; /* A2 coefficient of an IIR3 filter */
    float_t fltA3; /* A3 coefficient of an IIR3 filter */
} GDFLIB_FILTER_IIR3_COEFF_T_FLT;

typedef struct
{
    GDFLIB_FILTER_IIR3_COEFF_T_FLT sFltCoeff; /* Sub-structure containing filter coefficients. */
    float_t fltFltBfrY[3]; /* Internal accumulator buffer */
    float_t fltFltBfrX[3]; /* Input buffer of an IIR3 filter */
} GDFLIB_FILTER_IIR3_T_FLT;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/ 
void GDFLIB_FilterIIR3Init_FLT_FC(GDFLIB_FILTER_IIR3_T_FLT *psParam);
float_t GDFLIB_FilterIIR3_FLT_FC(float_t fltInX,
                                  GDFLIB_FILTER_IIR3_T_FLT *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERIIR3_FLT_H_ */
