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
* @file     state_machine.c
*
* @brief    Motor control state machine
*
******************************************************************************/
/******************************************************************************
| includes
|----------------------------------------------------------------------------*/
#include "state_machine.h"

/******************************************************************************
| global variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| local variables
|----------------------------------------------------------------------------*/


/******************************************************************************
| local functions
|----------------------------------------------------------------------------*/

/*------------------------------------
 * application state machine functions
 * ----------------------------------*/
static void SM_StateFaultFast(SM_APP_CTRL_T *psAppCtrl);
static void SM_StateInitFast(SM_APP_CTRL_T *psAppCtrl);
static void SM_StateStopFast(SM_APP_CTRL_T *psAppCtrl);
static void SM_StateRunFast(SM_APP_CTRL_T *psAppCtrl);

/*------------------------------------
 * application state machine functions SLOW
 * ----------------------------------*/
static void SM_StateFaultSlow(SM_APP_CTRL_T *psAppCtrl);
static void SM_StateInitSlow(SM_APP_CTRL_T *psAppCtrl);
static void SM_StateStopSlow(SM_APP_CTRL_T *psAppCtrl);
static void SM_StateRunSlow(SM_APP_CTRL_T *psAppCtrl);

/******************************************************************************
| local functions
|----------------------------------------------------------------------------*/
/* state machine functions field */
const PFCN_VOID_PSM gSM_STATE_TABLE_FAST[4] = 
{
    SM_StateFaultFast, 
    SM_StateInitFast,
    SM_StateStopFast, 
    SM_StateRunFast
};

const PFCN_VOID_PSM gSM_STATE_TABLE_SLOW[4] = 
{
    SM_StateFaultSlow, 
    SM_StateInitSlow, 
    SM_StateStopSlow, 
    SM_StateRunSlow
};

/******************************************************************************
@brief   FAULT state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void SM_StateFaultFast(SM_APP_CTRL_T *psAppCtrl)
{
    /* user Fault function */
    psAppCtrl -> psStateFast -> Fault();
    
    /* if clear fault command flag */
    if ((psAppCtrl -> uiCtrl & SM_CTRL_FAULT_CLEAR) > 0)
    {
        /* clear INIT_DONE, FAULT, FAULT_CLEAR flags */
        psAppCtrl -> uiCtrl &= ~(SM_CTRL_INIT_DONE | 
                                 SM_CTRL_FAULT | 
                                 SM_CTRL_FAULT_CLEAR);

        /* user Fault to Init transition function */
        psAppCtrl -> psTrans -> FaultInit();

        /* init state */
        psAppCtrl -> eState = INIT;
    }
}

/******************************************************************************
@brief   Init state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void SM_StateInitFast(SM_APP_CTRL_T *psAppCtrl)
{
    /* user Init function */
    psAppCtrl -> psStateFast -> Init();

    /* if fault flag */
    if ((psAppCtrl -> uiCtrl & SM_CTRL_FAULT) > 0)
    {
        /* user Init to Fault transition function */
        psAppCtrl -> psTrans -> InitFault();
        
        /* fault state */
        psAppCtrl -> eState = FAULT;
    }
    /* if INIT_DONE flag */
    else if ((psAppCtrl -> uiCtrl & SM_CTRL_INIT_DONE) > 0)
    {
        /* clear INIT_DONE, START_STOP, OM_CHANGE, STOP_ACK, RUN_ACK flags */
        psAppCtrl -> uiCtrl &= ~(SM_CTRL_INIT_DONE | 
                                 SM_CTRL_STOP | 
                                 SM_CTRL_START | 
                                 SM_CTRL_STOP_ACK | 
                                 SM_CTRL_RUN_ACK);

         /* user Init to Stop transition function */
        psAppCtrl -> psTrans -> InitStop();

        /* stop state */
        psAppCtrl -> eState = STOP;
    }
}

/******************************************************************************
@brief   STOP state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void SM_StateStopFast(SM_APP_CTRL_T *psAppCtrl)
{
    /* user Stop function */
    psAppCtrl -> psStateFast -> Stop();

    /* if fault */
    if ((psAppCtrl -> uiCtrl & SM_CTRL_FAULT) > 0)
    {
        /* user Stop to Fault transition function */
        psAppCtrl -> psTrans -> StopFault();

        /* fault state */
        psAppCtrl -> eState = FAULT;    
    }
    else if ((psAppCtrl -> uiCtrl & SM_CTRL_START) > 0)
    {
        /* user Stop to Run transition function, user must set up the 
           SM_CTRL_RUN_ACK flag to allow the RUN state */
        psAppCtrl -> psTrans -> StopRun();
        
        /* clears the START command */
        if ((psAppCtrl -> uiCtrl & SM_CTRL_RUN_ACK) > 0)
        {
            /* clears the RUN_ACK flag */
            psAppCtrl -> uiCtrl &= ~(SM_CTRL_RUN_ACK | SM_CTRL_START);
            
            /* run state */
            psAppCtrl -> eState = RUN;    
        }
    }
}

/******************************************************************************
@brief   RUN state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void SM_StateRunFast(SM_APP_CTRL_T *psAppCtrl)
{
    /* user Run function */
    psAppCtrl -> psStateFast -> Run();

    if ((psAppCtrl -> uiCtrl & SM_CTRL_FAULT) > 0)
    {
        /* user Run to Fault transition function */
        psAppCtrl -> psTrans -> RunFault();

        /* fault state */
        psAppCtrl -> eState = FAULT;    
    }
    else if ((psAppCtrl -> uiCtrl & SM_CTRL_STOP) > 0)
    {
        /* user Run to Stop transition function, user must set up the 
           SM_CTRL_STOP_ACK flag to allow the STOP state */
        psAppCtrl -> psTrans -> RunStop();
        
        if ((psAppCtrl -> uiCtrl & SM_CTRL_STOP_ACK) > 0)
        {
            /* clears the STOP_ACK flag */
            psAppCtrl -> uiCtrl &= ~(SM_CTRL_STOP_ACK | SM_CTRL_STOP);
            
            /* run state */
            psAppCtrl -> eState = STOP;    
        }
    }
}

/******************************************************************************
@brief   Duplicate SM functions for SLOW loop

@param   N/A

@return  N/A
******************************************************************************/
static void SM_StateFaultSlow(SM_APP_CTRL_T *psAppCtrl)
{
    psAppCtrl -> psStateSlow -> Fault();
}

static void SM_StateInitSlow(SM_APP_CTRL_T *psAppCtrl)
{
    psAppCtrl -> psStateSlow -> Init();
}

static void SM_StateStopSlow(SM_APP_CTRL_T *psAppCtrl)
{
    psAppCtrl -> psStateSlow -> Stop();
}

static void SM_StateRunSlow(SM_APP_CTRL_T *psAppCtrl)
{
    psAppCtrl -> psStateSlow -> Run();
}

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/


