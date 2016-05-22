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
* @file     Calc_power.c
*
* @brief    Calulate 3-phase power routine
*
***************************************************************************//*!*/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "Calc_power.h"
#include "fraclib.h"
#include "meterliblp.h"
#include "meterlibcfg.h"

/******************************************************************************
| global variables
|----------------------------------------------------------------------------*/
MID_CALC_POWER_A1_T     sMIDCalcPower;

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
static tMETERLIBLP3PH_DATA    mlib = METERLIB_CFG;
static tENERGY_CNT            wh_cnt, varh_cnt;

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
void MID_calcPower(MID_CALC_POWER_A1_T* sCalcPower)
{
    
  
    /* Data processing */
    METERLIBLP3PH_ProcSamples(&mlib, FRAC24(sCalcPower->sUABC.fltA/U_MAX), FRAC24(sCalcPower->sIABC.fltA/I_MAX),\
                                     FRAC24(sCalcPower->sUABC.fltB/U_MAX), FRAC24(sCalcPower->sIABC.fltB/I_MAX),\
                                     FRAC24(sCalcPower->sUABC.fltC/U_MAX), FRAC24(sCalcPower->sIABC.fltC/I_MAX), NULL);
    
    
    
    /* Calculate watt hours */
    METERLIBLP3PH_CalcWattHours(&mlib,&wh_cnt,METERLIBLP_KWH_PR(IMP_PER_KWH));
    
    
    
    /* Calculate var hours */
    METERLIBLP3PH_CalcVarHours(&mlib,&varh_cnt,METERLIBLP_KVARH_PR(IMP_PER_KVARH));
    
    /* Calculate auxiliary */
    METERLIBLP3PH_CalcAuxiliary(&mlib);
}

/******************************************************************************
@brief        No load test function

@param[in]    *sRsMeasFcn   input structure of type #MID_GET_RS_A1_T for passing
              all necessary parameters.

@return       none
******************************************************************************/
void MID_readPower(MID_CALC_POWER_A1_T* sCalcPower)
{
    struct
    {
        double dblA;
        double dblB;
        double dblC;
    }sUrms;

    struct
    {
        double dblA;
        double dblB;
        double dblC;
    }sIrms;
      
    struct
    {
        double dblA;
        double dblB;
        double dblC;
    }sP;
        
    struct
    {
        double dblA;
        double dblB;
        double dblC;
    }sQ;

    struct
    {
        double dblA;
        double dblB;
        double dblC;
    }sS;  
  
    /* Read Urms, Irms, P, Q, S on phase A */
    METERLIBLP3PH_ReadResultsPh1(&mlib, &sUrms.dblA, &sIrms.dblA, \
                               &sP.dblA, &sQ.dblA, &sS.dblA);
    
    /* Read Urms, Irms, P, Q, S on phase B */
    METERLIBLP3PH_ReadResultsPh1(&mlib, &sUrms.dblB, &sIrms.dblB, \
                               &sP.dblB, &sQ.dblB, &sS.dblB);
    
    /* Read Urms, Irms, P, Q, S on phase C */
    METERLIBLP3PH_ReadResultsPh1(&mlib, &sUrms.dblC, &sIrms.dblC, \
                               &sP.dblC, &sQ.dblC, &sS.dblC);
    
    /* Calculate average values */
    sCalcPower->UrmsAvg = (float)((sUrms.dblA + sUrms.dblB + sUrms.dblC) / 3);
    sCalcPower->IrmsAvg = (float)((sIrms.dblA + sIrms.dblB + sIrms.dblC) / 3);
    sCalcPower->PAvg    = (float)((sP.dblA + sP.dblB + sP.dblC) / 3);
    sCalcPower->QAvg    = (float)((sQ.dblA + sQ.dblB + sQ.dblC) / 3);
    sCalcPower->SAvg    = (float)((sS.dblA + sS.dblB + sS.dblC) / 3);
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/