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
* @file     state_machine.h
*
* @brief    Motor control state machine
*
******************************************************************************/
#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/

/******************************************************************************
| constants
|----------------------------------------------------------------------------*/
/* state machine control command flags */
#define SM_CTRL_NONE        (0x0)
#define SM_CTRL_FAULT       (0x1)
#define SM_CTRL_FAULT_CLEAR (0x2)
#define SM_CTRL_INIT_DONE   (0x4)
#define SM_CTRL_STOP        (0x8)
#define SM_CTRL_START       (0x10)
#define SM_CTRL_STOP_ACK    (0x20)
#define SM_CTRL_RUN_ACK     (0x40)

/******************************************************************************
| types
|----------------------------------------------------------------------------*/
/* aplication state identification enum */
typedef enum {
    FAULT           = 0,
    INIT            = 1,
    STOP            = 2,
    RUN             = 3,
} SM_APP_STATE_T;         

typedef unsigned short SM_APP_CTRL;
typedef unsigned long SM_APP_FAULT;

typedef void (*PFCN_VOID_VOID)(void); /* pointer to function */

/* user state machine functions structure */
typedef struct
{
    PFCN_VOID_VOID                  Fault;
    PFCN_VOID_VOID                  Init;
    PFCN_VOID_VOID                  Stop;
    PFCN_VOID_VOID                  Run;
} SM_APP_STATE_FCN_T;

/* user state-transition functions structure*/
typedef struct
{
    PFCN_VOID_VOID                  FaultInit;
    PFCN_VOID_VOID                  InitFault;
    PFCN_VOID_VOID                  InitStop;
    PFCN_VOID_VOID                  StopFault;
    PFCN_VOID_VOID                  StopRun;
    PFCN_VOID_VOID                  RunFault;
    PFCN_VOID_VOID                  RunStop;
} SM_APP_TRANS_FCN_T;

/* state machine control structure */
typedef struct
{
    SM_APP_STATE_FCN_T const*       psStateFast;                                /* state functions */
    SM_APP_STATE_FCN_T const*       psStateSlow;                                /* state functions slow*/
    SM_APP_TRANS_FCN_T const*       psTrans;                                    /* transition functions */
    SM_APP_CTRL                     uiCtrl;                                     /* control flags */
    SM_APP_STATE_T                  eState;                                     /* state */
} SM_APP_CTRL_T;

/* pointer to function with a pointer to state machine control structure */
typedef void (*PFCN_VOID_PSM)(SM_APP_CTRL_T *sAppCtrl); 


/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/* state machine function table */
extern  const PFCN_VOID_PSM gSM_STATE_TABLE_FAST[4];
extern  const PFCN_VOID_PSM gSM_STATE_TABLE_SLOW[4];

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/
/* state machine FAST function */
static inline void SM_StateMachineFast(SM_APP_CTRL_T *sAppCtrl)
{
    gSM_STATE_TABLE_FAST[sAppCtrl -> eState](sAppCtrl);
}

/* state machine SLOW function */
static inline void SM_StateMachineSlow(SM_APP_CTRL_T *sAppCtrl)
{
    gSM_STATE_TABLE_SLOW[sAppCtrl -> eState](sAppCtrl);
}

#ifdef __cplusplus
}
#endif

#endif /* _STATE_MACHINE_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/