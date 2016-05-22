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
* @file     NoLoad_test.c
*
* @brief    ACIM No Load test routine
*
***************************************************************************//*!*/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/
#include "NoLoad_test.h"
#include "Calc_power.h"

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
void MID_testNoLoad(MID_TEST_NOLOAD_A1_T* sNoLoadTest)
{
    GMCLIB_3COOR_T_FLT          sUABC;
  
    /* Initialization */
    if(sNoLoadTest->ui16Active == FALSE)
    {
        sNoLoadTest->ui16Active        = TRUE;
        sNoLoadTest->ui32LoopCounter   = 0;
        sNoLoadTest->fltFreqMeas       = 50.0F;
        sNoLoadTest->fltBulgarianConstantUrms   = 1.0F;
        sNoLoadTest->bCalcPower        = TRUE;
    }
    
    /* Pass speed to open loop scalar control */
    *(sNoLoadTest->pfltSpeedHz) = sNoLoadTest->fltFreqMeas;
    
    /* Pass Ud to open loop scalar control */
    *(sNoLoadTest->pfltUdReq) = sNoLoadTest->fltUdMeas;
    
    /* Convert U alpha-beta to ABC */
    GMCLIB_ClarkInv_FLT(sNoLoadTest->psUAlBe, &sUABC);
    
    /* Calculate power */
    sMIDCalcPower.sUABC.fltA = sNoLoadTest->fltBulgarianConstantUrms * sUABC.fltA;
    sMIDCalcPower.sUABC.fltB = sNoLoadTest->fltBulgarianConstantUrms * sUABC.fltB;
    sMIDCalcPower.sUABC.fltC = sNoLoadTest->fltBulgarianConstantUrms * sUABC.fltC;
    sMIDCalcPower.sIABC.fltA = sNoLoadTest->psIABC->fltA;
    sMIDCalcPower.sIABC.fltB = sNoLoadTest->psIABC->fltB;
    sMIDCalcPower.sIABC.fltC = sNoLoadTest->psIABC->fltC;
    
    /* Loop counter */
    sNoLoadTest->ui32LoopCounter++;
    
    /* Finnish No Load test after 10s */
    if(sNoLoadTest->ui32LoopCounter == 100000)
    {
        sNoLoadTest->fltUdMeas         = 0.0F;
        sNoLoadTest->bCalcPower        = FALSE;
    }
    
    /* Let the motor stop for 10s */
    if(sNoLoadTest->ui32LoopCounter == 200000)
    {
        sNoLoadTest->ui16Active        = FALSE;
        sNoLoadTest->ui32LoopCounter   = 0;
    
        MID_readPower(&sMIDCalcPower);
        sNoLoadTest->fltUrms = sMIDCalcPower.UrmsAvg;
        sNoLoadTest->fltIrms = sMIDCalcPower.IrmsAvg;
        sNoLoadTest->fltP    = sMIDCalcPower.PAvg;
        sNoLoadTest->fltQ    = sMIDCalcPower.QAvg;
        
        /* Check if 90% of Unom was reached */
        if(sNoLoadTest->fltUrms < (0.9F * sNoLoadTest->fltUdMeas * GFLIB_Sqrt_FLT(2.0F)))
          ui16FaultMID |= MID_FAULT_U_RMS_NOT_REACHED;
    }
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/