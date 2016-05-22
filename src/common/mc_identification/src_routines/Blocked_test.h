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
* @file     Blocked_test.h
*
* @brief    ACIM Locked Rotor test routine
*
***************************************************************************//*!*/
#ifndef BLOCKED_TEST_H_
#define BLOCKED_TEST_H_

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "MID_def.h"
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gmclib_FP.h"


/******************************************************************************
| constants
|----------------------------------------------------------------------------*/

/******************************************************************************
| macros
|----------------------------------------------------------------------------*/

/******************************************************************************
| types
|----------------------------------------------------------------------------*/
typedef struct
{
    GFLIB_CTRL_PI_P_AW_T_FLT    sIrmsPiParamsGFLIB;                         
    GMCLIB_2COOR_ALBE_T_FLT*    psUAlBe;                ///<Pointer to required voltage AlBe
    GMCLIB_3COOR_T_FLT*         psIABC;                 ///<Pointer to measured 3-phase current
    uint16_t                    ui16Active;             ///<Inidicates whether No Load test is active (true) or not (false)
    uint32_t                    ui32LoopCounter;        ///<For time keeping
    float_t*                    pfltUdReq;              ///<Pointer to required voltage Ud (PI current controller's output)
    float_t*                    pfltIdfbck;             ///<Pointer to actual real measured current Id
    float_t*                    pfltSpeedHz;            ///<Pointer to actual real measured current Id
    float_t                     fltIdMeas;              ///<Measured Rs return
    float_t                     fltUdMeas;              ///<Measured Rs return
    float_t                     fltFreqMeas;            ///<Measured Rs return
    float_t                     fltUrms;                   ///<Measured Rs return
    float_t                     fltIrms;                   ///<Measured Rs return
    float_t                     fltP;                   ///<Measured Rs return
    float_t                     fltQ;                   ///<Measured Rs return
    float_t                     fltI;                   ///<Measured Rs return
    float_t                     fltBulgarianConstantUrms;   ///<Measured Rs return
    bool_t                      bIrmsPISatFlag;         
    bool_t                      bCalcPower;             ///<Measured Rs return
    bool_t                      bRotorBlocked;
}MID_TEST_BLOCKED_A1_T;                               ///<Structure containing necessary variable for Rs measurement

/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/
extern uint16_t  ui16FaultMID;

/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

extern void MID_testBlocked(MID_TEST_BLOCKED_A1_T* sBlockedTest);

#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

#endif /* BLOCKED_TEST_H_ */
