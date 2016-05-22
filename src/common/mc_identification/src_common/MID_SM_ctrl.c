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
* @file     MID_SM_ctrl.c
*
* @brief    Motor parameters identification State machine control/switching
*
******************************************************************************/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "MID_SM_ctrl.h"

/******************************************************************************
| constants
|----------------------------------------------------------------------------*/

/******************************************************************************
| macros
|----------------------------------------------------------------------------*/

/******************************************************************************
| types
|----------------------------------------------------------------------------*/

/******************************************************************************
| global variables
|----------------------------------------------------------------------------*/
uint16_t ui16Calibration;            /* <true => calibration will be done after MID_SM_StateStart */
uint16_t ui16ElectricalParameters;   /* <true => measurement of electrical parameters will be done after MID_SM_StateStart */

/******************************************************************************
| local variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| local functions
|----------------------------------------------------------------------------*/

/*------------------------------------
 * Application state machine functions
 * ----------------------------------*/
static void MID_SM_StateStart(MID_SM_APP_CTRL_T *psAppCtrl);
static void MID_SM_StatePwrStgCharact(MID_SM_APP_CTRL_T *psAppCtrl);
static void MID_SM_StateRs(MID_SM_APP_CTRL_T *psAppCtrl);
static void MID_SM_StateNoLoad(MID_SM_APP_CTRL_T *psAppCtrl);
static void MID_SM_StateBlocked(MID_SM_APP_CTRL_T *psAppCtrl);
static void MID_SM_StateStop(MID_SM_APP_CTRL_T *psAppCtrl);

/******************************************************************************
| global functions
|----------------------------------------------------------------------------*/
/* State machine functions field */
const MID_PFCN_VOID_PSM gMID_SM_STATE_TABLE[6] = {MID_SM_StateStart, MID_SM_StatePwrStgCharact, MID_SM_StateRs, MID_SM_StateNoLoad, MID_SM_StateBlocked, MID_SM_StateStop};

/*****************************************************************************
@brief   Start state

@param   void

@return  none
******************************************************************************/
static void MID_SM_StateStart(MID_SM_APP_CTRL_T *psAppCtrl)
{
    /* call user Start function */
    psAppCtrl -> psState -> MID_Start();

    /* if START_DONE flag is set */
    if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_START_DONE) > 0)
    {
        /* clear state's _ACK & _DONE SM control flags */
        psAppCtrl -> uiCtrl &= ~(MID_SM_CTRL_START_DONE | MID_SM_CTRL_START_ACK);

        /* if ui16Calibration go to PWR_STG_CHARACT state */
        if(ui16Calibration != 0)
        {
            psAppCtrl -> psTrans -> MID_Start2PwrStgCharact();
            psAppCtrl -> eState = MID_PWR_STG_CHARACT;
        }

        /* if ui16ElectricalParameters go to RS state */
        if(ui16ElectricalParameters != 0)
        {
            psAppCtrl -> psTrans -> MID_Start2Rs();
            psAppCtrl -> eState = MID_RS;
        }
    }
}

/******************************************************************************
@brief   PwrStgChar state

@param   void

@return  none
******************************************************************************/
static void MID_SM_StatePwrStgCharact(MID_SM_APP_CTRL_T *psAppCtrl)
{
    /* call user PwrStgCharact function */
    psAppCtrl -> psState -> MID_PwrStgCharact();

    /* if PWR_STG_CHARACT_DONE flag is set */
    if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_PWR_STG_CHARACT_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl -> psTrans -> MID_PwrStgCharact2Stop();

        if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_PWR_STG_CHARACT_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl -> uiCtrl &= ~(MID_SM_CTRL_PWR_STG_CHARACT_DONE | MID_SM_CTRL_PWR_STG_CHARACT_ACK);

            /* next go to Rs state */
            psAppCtrl -> eState = MID_STOP;
        }
    }
}

/******************************************************************************
@brief   Rs state

@param   void

@return  none
******************************************************************************/
static void MID_SM_StateRs(MID_SM_APP_CTRL_T *psAppCtrl)
{
    /* call user Rs function */
    psAppCtrl -> psState -> MID_Rs();

    /* if RS_DONE flag is set */
    if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_RS_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl -> psTrans -> MID_Rs2NoLoad();

        if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_RS_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl -> uiCtrl &= ~(MID_SM_CTRL_RS_DONE | MID_SM_CTRL_RS_ACK);

            /* next go to Ls state */
            psAppCtrl -> eState = MID_NOLOAD;
        }
    }
}

/******************************************************************************
@brief   NoLoad state

@param   void

@return  none
******************************************************************************/
static void MID_SM_StateNoLoad(MID_SM_APP_CTRL_T *psAppCtrl)
{
    /* call user Rs function */
    psAppCtrl -> psState -> MID_NoLoad();

    /* if NOLOAD_DONE flag is set */
    if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_NOLOAD_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl -> psTrans -> MID_NoLoad2Blocked();

        if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_NOLOAD_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl -> uiCtrl &= ~(MID_SM_CTRL_NOLOAD_DONE | MID_SM_CTRL_NOLOAD_ACK);

            /* next go to NoLoad state */
            psAppCtrl -> eState = MID_BLOCKED;
        }
    }
}

/******************************************************************************
@brief   Blocked state

@param   void

@return  none
******************************************************************************/
static void MID_SM_StateBlocked(MID_SM_APP_CTRL_T *psAppCtrl)
{
    /* call user Blocked function */
    psAppCtrl -> psState -> MID_Blocked();

    /* if BLOCKED_DONE flag is set */
    if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_BLOCKED_DONE) > 0)
    {
        /* Run transition function */
        psAppCtrl -> psTrans -> MID_Blocked2Stop();

        if ((psAppCtrl -> uiCtrl & MID_SM_CTRL_BLOCKED_DONE) > 0)
        {
            /* clear state's _ACK & _DONE SM control flags */
            psAppCtrl -> uiCtrl &= ~(MID_SM_CTRL_BLOCKED_DONE | MID_SM_CTRL_BLOCKED_ACK);

            /* next go to Blocked state */
            psAppCtrl -> eState = MID_STOP;
        }
    }
}

/******************************************************************************
@brief   Stop state

@param   void

@return  none
******************************************************************************/
static void MID_SM_StateStop(MID_SM_APP_CTRL_T *psAppCtrl)
{
    /* call user Stop function */
    psAppCtrl -> psState -> MID_Stop();
}


/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
