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
* @file     Blocked_test.c
*
* @brief    ACIM Locked Rotor test routine
*
***************************************************************************//*!*/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "Blocked_test.h"
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
void MID_testBlocked(MID_TEST_BLOCKED_A1_T* sBlockedTest)
{
    GMCLIB_3COOR_T_FLT          sUABC;
  
    /* Initialization */
    if(sBlockedTest->ui16Active == FALSE)
    {
        sBlockedTest->ui16Active        = TRUE;
        sBlockedTest->ui32LoopCounter   = 0;
        sBlockedTest->fltFreqMeas       = 50.0F;
        sBlockedTest->fltUdMeas         = 0.0F;
        sBlockedTest->fltBulgarianConstantUrms   = 1.0F;
        sBlockedTest->bRotorBlocked     = FALSE;
    }
    
    /* Wait until rotor is locked */
    if(sBlockedTest->bRotorBlocked)
    {
        /* Calculate power in slow loop */
        sBlockedTest->bCalcPower        = TRUE;
        
        /* Pass speed to open loop scalar control */
        *(sBlockedTest->pfltSpeedHz) = sBlockedTest->fltFreqMeas;
        
        /* Pass Ud to open loop scalar control */
        *(sBlockedTest->pfltUdReq) = sBlockedTest->fltUdMeas;
        
        /* Convert U alpha-beta to ABC */
        GMCLIB_ClarkInv_FLT(sBlockedTest->psUAlBe, &sUABC);
        
        /* Calculate power */
        sMIDCalcPower.sUABC.fltA = sBlockedTest->fltBulgarianConstantUrms * sUABC.fltA;
        sMIDCalcPower.sUABC.fltB = sBlockedTest->fltBulgarianConstantUrms * sUABC.fltB;
        sMIDCalcPower.sUABC.fltC = sBlockedTest->fltBulgarianConstantUrms * sUABC.fltC;
        sMIDCalcPower.sIABC.fltA = sBlockedTest->psIABC->fltA;
        sMIDCalcPower.sIABC.fltB = sBlockedTest->psIABC->fltB;
        sMIDCalcPower.sIABC.fltC = sBlockedTest->psIABC->fltC;
        
        /* Loop counter */
        sBlockedTest->ui32LoopCounter++;
        
        /* Finnish Blocked test after 5s */
        if(sBlockedTest->ui32LoopCounter == 50000)
        {
            sBlockedTest->fltUdMeas         = 0.0F;
            sBlockedTest->fltIdMeas         = 0.0F;
            sBlockedTest->bCalcPower        = FALSE;
            sBlockedTest->ui16Active        = FALSE;
            sBlockedTest->ui32LoopCounter   = 0;
            
            MID_readPower(&sMIDCalcPower);
            sBlockedTest->fltUrms           = sMIDCalcPower.UrmsAvg;
            sBlockedTest->fltIrms           = sMIDCalcPower.IrmsAvg;
            sBlockedTest->fltP              = sMIDCalcPower.PAvg;
            sBlockedTest->fltQ              = sMIDCalcPower.QAvg;
            
            /* Check if 90% of Inom was reached */
            if(sBlockedTest->fltIrms < (0.9F * sBlockedTest->fltIdMeas))
              ui16FaultMID |= MID_FAULT_I_RMS_NOT_REACHED;
        }
    }
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/