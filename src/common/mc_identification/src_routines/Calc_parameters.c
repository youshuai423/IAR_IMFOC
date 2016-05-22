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
* @file     Calc_parameters.c
*
* @brief    Calulate ACIM parameters routine
*
***************************************************************************//*!*/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/
#include "Calc_parameters.h"

#include "app_init.h"

/******************************************************************************
| global variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| constant and macros
|----------------------------------------------------------------------------*/

/******************************************************************************
| local types
|----------------------------------------------------------------------------*/

/******************************************************************************
| local function prototypes
|----------------------------------------------------------------------------*/

/******************************************************************************
| local variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| global functions
|----------------------------------------------------------------------------*/

/*******************************************************************************
 * Implementation variant: Kinetis and FSL ESL Kinetis libraries
 ******************************************************************************/

/******************************************************************************
@brief        No load test function

@param[in]    *sRsMeasFcn   input structure of type #MID_GET_RS_A1_T for passing
              all necessary parameters.

@return       none
******************************************************************************/
void MID_calculateParameters(MID_CALC_PARAMETERS_A1_T* sCalcParameters)
{
    float_t     fltR, fltXBlocked, fltXNoLoad, fltX1, fltX2, fltLl1, fltLl2, fltXm;
    
    /* Auxiliary calculations */
    fltR        = (sCalcParameters->sBlocked.fltP) / (sCalcParameters->sBlocked.fltIrms * sCalcParameters->sBlocked.fltIrms);
    fltXBlocked = (sCalcParameters->sBlocked.fltQ) / (sCalcParameters->sBlocked.fltIrms * sCalcParameters->sBlocked.fltIrms);
    fltXNoLoad  = (sCalcParameters->sNoLoad.fltQ) / (sCalcParameters->sNoLoad.fltIrms * sCalcParameters->sNoLoad.fltIrms);
    fltX1       = fltXBlocked * 0.5F;
    fltX2       = fltXBlocked - fltX1;
    fltLl1      = fltX1/(2.0F*M1_PI_CNST*sCalcParameters->fltFreq);
    fltLl2      = fltX2/(2.0F*M1_PI_CNST*sCalcParameters->fltFreq);
    fltXm       = fltXNoLoad - fltX1;
    
    /* Rotor resistance */
    sCalcParameters->sParameters.fltRr = (fltR - sCalcParameters->fltRs) * ((fltX2 + fltXm)/fltXm) * ((fltX2 + fltXm)/fltXm);
    
    /* Mutual inductance */
    sCalcParameters->sParameters.fltLm = fltXm/(2.0F*M1_PI_CNST*sCalcParameters->fltFreq);
    
    /* Stator inductance */
    sCalcParameters->sParameters.fltLs = sCalcParameters->sParameters.fltLm + fltLl1;
    
    /* Rotor inductance */
    sCalcParameters->sParameters.fltLr = sCalcParameters->sParameters.fltLm + fltLl2;
    
    sCalcParameters->iCalcDone = TRUE;
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/