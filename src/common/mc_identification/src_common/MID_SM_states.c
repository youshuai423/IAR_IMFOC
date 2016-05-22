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
* @file      MID_SM_states.c
*
* @brief     Motor parameters identification state machine
*
*
******************************************************************************/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "mid_sm_states.h"
#include "mlib.h"

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
MID_GET_RS_A1_T                 sMIDRs;                 /* <Input structure for MID_getRs()*/
MID_TEST_NOLOAD_A1_T            sMIDNoLoad;             /* <Input structure for MID_testNoLoad()*/
MID_TEST_BLOCKED_A1_T           sMIDBlocked;            /* <Input structure for MID_testBlocked()*/
MID_CALC_PARAMETERS_A1_T        sMIDCalcParams;         /* <Input structure for MID_calculateParameters() */
MID_GET_CHAR_A1_T               sMIDPwrStgChar;         /* <Input structure for MID_getTransChar() */
uint16_t                        ui16FaultMID;           /* <MID fault number, 0x0000 = no fault */
uint16_t                        ui16EnableMeasurement;  /* <Enables measurement in superior machine */

/******************************************************************************
| local variables
|----------------------------------------------------------------------------*/
/*---------------------------------------------------------
 * (user) Motor parameters identification state machine functions
 * --------------------------------------------------------*/
static void MID_StateStart(void);
static void MID_StatePwrStgCharact(void);
static void MID_StateRs(void);
static void MID_StateNoLoad(void);
static void MID_StateBlocked(void);
static void MID_StateStop(void);

/*-------------------------------------------------------------------
 * (user) Motor parameters identification state-transition functions
 * ------------------------------------------------------------------*/
/* non-fault transition function prototypes */
static void MID_TransStart2PwrStgCharact(void);
static void MID_TransStart2Rs(void);
static void MID_TransPwrStgCharact2Stop(void);
static void MID_TransRs2NoLoad(void);
static void MID_TransNoLoad2Blocked(void);
static void MID_TransBlocked2Stop(void);

/* State machine functions field */
static const MID_SM_APP_STATE_FCN_T msSTATE =   {
                                                MID_StateStart,
                                                MID_StatePwrStgCharact,
                                                MID_StateRs,
                                                MID_StateNoLoad,
                                                MID_StateBlocked,
                                                MID_StateStop
                                                };

/* State-transition functions field */
/*
 * note: order of the transition function should be the same as in definition
 * of the MID_SM_APP_TRANS_FCN_T type
 */
static const MID_SM_APP_TRANS_FCN_T msTRANS =   {
                                                MID_TransStart2PwrStgCharact,
                                                MID_TransStart2Rs,
                                                MID_TransRs2NoLoad,
                                                MID_TransNoLoad2Blocked,
                                                MID_TransBlocked2Stop,
                                                MID_TransPwrStgCharact2Stop
                                                };

/* State machine structure definition and initialization */
MID_SM_APP_CTRL_T gsMIDCtrl =
{
    /* gsMIDCtrl.psState, User state functions  */
    &msSTATE,

    /* gsMIDCtrl.psTrans, User state-transition functions */
    &msTRANS,

    /* gsMIDCtrl.uiCtrl, Default no control command */
    MID_SM_CTRL_NONE,

    /* gsMIDCtrl.eState, Default state after reset */
    MID_START
};


/******************************************************************************
| local functions
|----------------------------------------------------------------------------*/

/******************************************************************************
@brief    MID START state

@param   void

@return  none
******************************************************************************/
static void MID_StateStart(void)
{
    /* Type the code to do when in the START state */

    /* set _DONE to switch to following transition state */
    gsMIDCtrl.uiCtrl |= MID_SM_CTRL_START_DONE;
}

/******************************************************************************
@brief    MID PWR_STG_CHARACT state

@param    void

@return   none
******************************************************************************/
static void MID_StatePwrStgCharact(void)
{
    /* Type the code to do when in the PWR_STG_CHARACT state */
    MID_GetTransferCharacteristic(&sMIDPwrStgChar);

    /* When charcterization ends, go to STOP state */
    if (sMIDPwrStgChar.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        gsMIDCtrl.uiCtrl |= MID_SM_CTRL_PWR_STG_CHARACT_DONE;
    }
}

/******************************************************************************
@brief    MID RS state

@param    void

@return   none
******************************************************************************/
static void MID_StateRs(void)
{
    /* Type the code to do when in the RS state */
    MID_getRs(&sMIDRs);

    /* When Rs measurement done, go to NOLOAD state */
    if (sMIDRs.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        gsMIDCtrl.uiCtrl |= MID_SM_CTRL_RS_DONE;
    }
}

/******************************************************************************
@brief    MID NOLOAD state

@param    void

@return   none
******************************************************************************/
static void MID_StateNoLoad(void)
{
    /* Type the code to do when in the RS state */
    MID_testNoLoad(&sMIDNoLoad);

    /* When No Load test done, go to NOLOAD state */
    if (sMIDNoLoad.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        gsMIDCtrl.uiCtrl |= MID_SM_CTRL_NOLOAD_DONE;
    }
}

/******************************************************************************
@brief    MID BLOCKED state

@param    void

@return   none
******************************************************************************/
static void MID_StateBlocked(void)
{
    /* Type the code to do when in the BLOCKED state */
    MID_testBlocked(&sMIDBlocked);

    /* When Locked Rotor test done, go to STOP state */
    if (sMIDBlocked.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        gsMIDCtrl.uiCtrl |= MID_SM_CTRL_BLOCKED_DONE;
    }
}

/******************************************************************************
@brief   MID STOP state

@param   void

@return  none
******************************************************************************/
static void MID_StateStop(void)
{
    /* Type the code to do when in the STOP state */
    /* Calculate parameters only if measurement was performed*/
    if(ui16ElectricalParameters != 0)
    {
        MID_calculateParameters(&sMIDCalcParams);
        
        /* When parameters calculation finished, set the MID_SM_CTRL_STOP_DONE flag */
        if (sMIDCalcParams.iCalcDone == TRUE)
            gsMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_DONE;
    }
    
    /* If not in measurement, set the MID_SM_CTRL_STOP_DONE flag immediately*/
    else
        gsMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_DONE;
    
    /*
    * motor parameters measurement is finished, main state machine
    * can proceed with next state
    */
}

/******************************************************************************
@brief   MID START to PWR_STG_CHARACT transition

@param   void

@return  none
******************************************************************************/
static void MID_TransStart2PwrStgCharact(void)
{
    /* Type the code to do when going from the START to the PWR_STG_CHARACT state */
    /* Enable start of characterisation */
    sMIDPwrStgChar.ui16Active = FALSE;

    /* Acknowledge that the state machine can proceed into PWR_STG_CHARACT state */
    gsMIDCtrl.uiCtrl |= MID_SM_CTRL_PWR_STG_CHARACT_ACK;
}

/******************************************************************************
@brief   MID START to RS transition

@param   void

@return  none
******************************************************************************/
static void MID_TransStart2Rs(void)
{
    /* Type the code to do when going from the START to the RS state */

    /* Enable start of Rs measurement */
    sMIDRs.ui16Active = FALSE;
    sMIDRs.pfltUdErrorLookUp = &(fltTransferCharError[0]);
    sMIDRs.sLUTparams.u16TableSize = M1_CHAR_CURRENT_POINT_NUMBERS;

    /* Acknowledge that the state machine can proceed into RS state */
    gsMIDCtrl.uiCtrl |= MID_SM_CTRL_RS_ACK;
}

/******************************************************************************
* @brief   MID PWR_STG_CHARACT to STOP transition
*
* @param   void
*
* @return  none
******************************************************************************/
static void MID_TransPwrStgCharact2Stop(void)
{
    /* Type the code to do when going from the PWR_STG_CHARACT to the STOP state */

    /* acknowledge that the state machine can proceed into RS state */
    gsMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}

/******************************************************************************
* @brief   MID RS to NOLOAD transition
*
* @param   void
*
* @return  none
******************************************************************************/
static void MID_TransRs2NoLoad(void)
{
    /* Type the code to do when going from the RS to the NOLOAD state */
    sMIDNoLoad.ui16Active = FALSE;
  
    /* Acknowledge that the state machine can proceed into NOLOAD state */
    gsMIDCtrl.uiCtrl |= MID_SM_CTRL_NOLOAD_ACK;
}

/******************************************************************************
* @brief   MID NOLOAD to BLOCKED transition
*
* @param   void
*
* @return  none
******************************************************************************/
static void MID_TransNoLoad2Blocked(void)
{
    /* Type the code to do when going from the NOLOAD to the BLOCKED state */

    /* Acknowledge that the state machine can proceed into BLOCKED state */
    gsMIDCtrl.uiCtrl |= MID_SM_CTRL_BLOCKED_ACK;
}

/******************************************************************************
* @brief   MID BLOCKED to STOP transition
*
* @param   void
*
* @return  none
******************************************************************************/
static void MID_TransBlocked2Stop(void)
{
    /* Type the code to do when going from the BLOCKED to the STOP state */
    sMIDCalcParams.fltFreq              = 50.0F;
    sMIDCalcParams.fltRs                = sMIDRs.fltRs;
    sMIDCalcParams.sNoLoad.fltIrms      = sMIDNoLoad.fltIrms;
    sMIDCalcParams.sNoLoad.fltUrms      = sMIDNoLoad.fltUrms;
    sMIDCalcParams.sNoLoad.fltP         = sMIDNoLoad.fltP;
    sMIDCalcParams.sNoLoad.fltQ         = sMIDNoLoad.fltQ;
    sMIDCalcParams.sBlocked.fltIrms     = sMIDBlocked.fltIrms;
    sMIDCalcParams.sBlocked.fltUrms     = sMIDBlocked.fltUrms;
    sMIDCalcParams.sBlocked.fltP        = sMIDBlocked.fltP;
    sMIDCalcParams.sBlocked.fltQ        = sMIDBlocked.fltQ;
    sMIDCalcParams.iCalcDone            = FALSE;
  
    /* Acknowledge that the state machine can proceed into STOP state */
    gsMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
