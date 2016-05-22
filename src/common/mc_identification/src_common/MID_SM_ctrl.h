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
* @file      MID_SM_ctrl.h
*
* @brief     Motor parameters identification State machine header file
*
*******************************************************************************/

#ifndef _MID_SM_CTRL_H_
#define _MID_SM_CTRL_H_

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "mlib_types.h"

/******************************************************************************
| constants
|----------------------------------------------------------------------------*/
/* State machine control command flags */
/* maximum 8 pair state _ACK/_DONE flags */
#define MID_SM_CTRL_NONE                    0x00000000
#define MID_SM_CTRL_START_ACK               0x00000002
#define MID_SM_CTRL_START_DONE              ((MID_SM_CTRL_START_ACK) * 0x10000)
#define MID_SM_CTRL_PWR_STG_CHARACT_ACK     0x00000004
#define MID_SM_CTRL_PWR_STG_CHARACT_DONE    ((MID_SM_CTRL_PWR_STG_CHARACT_ACK) * 0x10000)
#define MID_SM_CTRL_RS_ACK                  0x00000008
#define MID_SM_CTRL_RS_DONE                 ((MID_SM_CTRL_RS_ACK) * 0x10000)
#define MID_SM_CTRL_NOLOAD_ACK              0x00000010
#define MID_SM_CTRL_NOLOAD_DONE             ((MID_SM_CTRL_NOLOAD_ACK) * 0x10000)
#define MID_SM_CTRL_BLOCKED_ACK             0x00000020
#define MID_SM_CTRL_BLOCKED_DONE            ((MID_SM_CTRL_BLOCKED_ACK) * 0x10000)
#define MID_SM_CTRL_STOP_ACK                0x00000100
#define MID_SM_CTRL_STOP_DONE               ((MID_SM_CTRL_STOP_ACK) * 0x10000)


/******************************************************************************
| types
|----------------------------------------------------------------------------*/
/* Application state identification enum */
typedef enum {
    MID_START           = 0,
    MID_PWR_STG_CHARACT = 1,
    MID_RS              = 2,
    MID_NOLOAD          = 3,
    MID_BLOCKED         = 4,
    MID_STOP            = 5,
} MID_SM_APP_STATE_T;

typedef unsigned long MID_SM_APP_CTRL;
typedef unsigned long MID_SM_APP_FAULT;

typedef void (*MID_PFCN_VOID_VOID)(void); /* pointer to function */

/* User state machine functions structure */
typedef struct
{
    MID_PFCN_VOID_VOID  MID_Start;
    MID_PFCN_VOID_VOID  MID_PwrStgCharact;
    MID_PFCN_VOID_VOID  MID_Rs;
    MID_PFCN_VOID_VOID  MID_NoLoad;
    MID_PFCN_VOID_VOID  MID_Blocked;
    MID_PFCN_VOID_VOID  MID_Stop;
} MID_SM_APP_STATE_FCN_T;

/* User state-transition functions structure*/
typedef struct
{
    MID_PFCN_VOID_VOID  MID_Start2PwrStgCharact;
    MID_PFCN_VOID_VOID  MID_Start2Rs;
    MID_PFCN_VOID_VOID  MID_Rs2NoLoad;
    MID_PFCN_VOID_VOID  MID_NoLoad2Blocked;
    MID_PFCN_VOID_VOID  MID_Blocked2Stop;
    MID_PFCN_VOID_VOID  MID_PwrStgCharact2Stop;
} MID_SM_APP_TRANS_FCN_T;

/* State machine control structure */
typedef struct
{
    MID_SM_APP_STATE_FCN_T const*    psState;    /* State functions */
    MID_SM_APP_TRANS_FCN_T const*    psTrans;    /* Transition functions */
    MID_SM_APP_CTRL                  uiCtrl;     /* Control flags */
    MID_SM_APP_STATE_T               eState;     /* State */
} MID_SM_APP_CTRL_T;

/* pointer to function with a pointer to state machine control structure */
typedef void (*MID_PFCN_VOID_PSM)(MID_SM_APP_CTRL_T *sAppCtrl);


/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/
extern uint16_t ui16Calibration;
extern uint16_t ui16ElectricalParameters;

/* State machine function table (in pmem) */
extern const MID_PFCN_VOID_PSM gMID_SM_STATE_TABLE[6];


/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/
/* MID State machine function */
static inline void MID_SM_StateMachine(MID_SM_APP_CTRL_T *sAppCtrl)
{
    gMID_SM_STATE_TABLE[sAppCtrl -> eState](sAppCtrl);
}


#endif /* _MID_SM_CTRL_H_ */
