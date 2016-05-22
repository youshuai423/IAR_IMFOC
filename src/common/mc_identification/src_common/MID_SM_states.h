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
* @file      MID_SM_states.h
*
* @brief     Motor parameters identification state machine header file
*
******************************************************************************/
#ifndef _MID_SM_STATES_H_
#define _MID_SM_STATES_H_

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "mid_sm_ctrl.h"
#include "pwrstg_characteristic.h"
#include "rs_measure.h"
#include "noload_test.h"
#include "blocked_test.h"
#include "calc_power.h"
#include "calc_parameters.h"

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
| exported variables
|----------------------------------------------------------------------------*/
/* The MID state machine structure declaration */
extern MID_SM_APP_CTRL_T gsMIDCtrl;

/* Measurement global variables */
extern MID_GET_RS_A1_T          sMIDRs;
extern MID_TEST_NOLOAD_A1_T     sMIDNoLoad;
extern MID_TEST_BLOCKED_A1_T    sMIDBlocked;
extern MID_GET_CHAR_A1_T        sMIDPwrStgChar;
extern uint16_t                 ui16FaultMID;
extern uint16_t                 ui16EnableMeasurement;

/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

#endif /* _MID_SM_STATES_ */
