/*******************************************************************************
*
* Copyright 2015 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
*
*
****************************************************************************//*!
*
* @file     Calc_parameters.h
*
* @brief    Calulate ACIM parameters routine
*
***************************************************************************//*!*/
#ifndef CALC_PARAMETRS_H_
#define CALC_PARAMETRS_H_

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/
#include "MID_def.h"

/******************************************************************************
| constants
|----------------------------------------------------------------------------*/

/******************************************************************************
| macros
|----------------------------------------------------------------------------*/

/******************************************************************************
| types
|----------------------------------------------------------------------------*/    
typedef struct
{
    /* Inputs */
    struct
    {
        float_t fltUrms;
        float_t fltIrms;
        float_t fltP;
        float_t fltQ;
    }sNoLoad;

    struct
    {
        float_t fltUrms;
        float_t fltIrms;
        float_t fltP;
        float_t fltQ;
    }sBlocked;
        
    float_t  fltRs;
    float_t  fltFreq;
    int16_t  iCalcDone;
        
    /* Calculated parameters */
    struct
    {
        float_t fltRr;
        float_t fltLm;
        float_t fltLs;
        float_t fltLr;
    }sParameters;
}MID_CALC_PARAMETERS_A1_T;

/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

extern void MID_calculateParameters(MID_CALC_PARAMETERS_A1_T* sCalcParameters);

#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

#endif /* CALC_PARAMETRS_H_ */
