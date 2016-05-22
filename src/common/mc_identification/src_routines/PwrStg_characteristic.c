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
* @file     PwrStg_characteristic.c
*
* @brief    Power Stage characterization routine
*
***************************************************************************//*!*/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/
#include "pwrstg_characteristic.h"

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
| local functions
|----------------------------------------------------------------------------*/

/******************************************************************************
| global functions
|----------------------------------------------------------------------------*/

/*******************************************************************************
 * Implementation variant: Kinetis and FSL ESL Kinetis libraries
 ******************************************************************************/
/******************************************************************************
*
* @brief   Transfer characteristic measurement function
*
* @param[in]  *sTransferCharFcn     input structure of type #MID_GET_CHAR_A1_T
*                                   for passing all necessary parameters.
*
* @return  none
*
******************************************************************************/
void MID_GetTransferCharacteristic(MID_GET_CHAR_A1_T* sTransferCharFcn)
{
    float_t fltRs_voltage_drop;      /* Auxiliary variable for Rs voltage drop calculation */

    /* Initialisation */
    if(sTransferCharFcn->ui16Active == 0)
    {
        sTransferCharFcn->ui16Active      = TRUE;
        sTransferCharFcn->ui16LoopCounter = 0;
        sTransferCharFcn->fltIdReqActual  = -sTransferCharFcn->fltIdCalib;
        *(sTransferCharFcn->pfltIdReq)    = sTransferCharFcn->fltIdReqActual;
        sTransferCharFcn->ui16LUTIndex    = 0;
    }

    /* LoopCounter for time keeping */
    sTransferCharFcn->ui16LoopCounter++;

    /* After 300ms settling of Id start calculation */
    if(sTransferCharFcn->ui16LoopCounter >= M1_TIME_300MS)
    {
        /* Faults */
        /* Check if Rs is low enough to reach 2A */
        if((MLIB_Abs_FLT(*(sTransferCharFcn->pfltIdfbck)) < (sTransferCharFcn->fltIdCalib - M1_K_I_50MA)) && (sTransferCharFcn->ui16LUTIndex == 0))
        {
            ui16FaultMID |= MID_FAULT_TOO_HIGH_RS;
            sTransferCharFcn->ui16Active   = FALSE;
            *(sTransferCharFcn->pfltIdReq) = 0.0F;
        }
        /* Check if motor is connected */
        if((MLIB_Abs_FLT(*(sTransferCharFcn->pfltIdfbck)) < M1_K_I_50MA) && (sTransferCharFcn->ui16LUTIndex == 0))
        {
            ui16FaultMID |= MID_FAULT_NO_MOTOR;
            sTransferCharFcn->ui16Active   = FALSE;
            *(sTransferCharFcn->pfltIdReq) = 0.0F;
        }

        /* Calculate voltage drop from Rs and Id */        
        fltRs_voltage_drop = sTransferCharFcn->fltIdReqActual*sTransferCharFcn->fltRs;

        /* Calculate Error voltage and store it to f16ErrorLookUp */
        sTransferCharFcn->fltUdErrorLookUp[sTransferCharFcn->ui16LUTIndex] = *(sTransferCharFcn->pfltUdReq) - fltRs_voltage_drop;

        /* Prepare for next point measurement */
        sTransferCharFcn->ui16LUTIndex++;
        sTransferCharFcn->fltIdReqActual += sTransferCharFcn->fltIdIncrement;
        *(sTransferCharFcn->pfltIdReq) = sTransferCharFcn->fltIdReqActual;
        sTransferCharFcn->ui16LoopCounter = 0;

        /* End after last current was measured */
        if(sTransferCharFcn->ui16LUTIndex >= M1_CHAR_CURRENT_POINT_NUMBERS)
        {
            sTransferCharFcn->ui16Active   = FALSE;
            *(sTransferCharFcn->pfltIdReq) = 0.0F;
        }
    }
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/