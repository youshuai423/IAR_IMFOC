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
* @file     NoLoad_test.h
*
* @brief    ACIM No Load test routine
*
***************************************************************************//*!*/
#ifndef NOLOAD_TEST_H_
#define NOLOAD_TEST_H_

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
    GMCLIB_2COOR_ALBE_T_FLT*    psUAlBe;                ///<Pointer to required voltage AlBe
    GMCLIB_3COOR_T_FLT*         psIABC;                 ///<Pointer to measured 3-phase current
    uint16_t                    ui16Active;             ///<Inidicates whether No Load test is active (true) or not (false)
    uint32_t                    ui32LoopCounter;        ///<For time keeping
    float_t*                    pfltUdReq;              ///<Pointer to required voltage Ud (PI current controller's output)
    float_t*                    pfltIdfbck;             ///<Pointer to actual real measured current Id
    float_t*                    pfltSpeedHz;            ///<Pointer to actual real measured current Id
    float_t                     fltUdMeas;              ///<Measured Rs return
    float_t                     fltFreqMeas;            ///<Measured Rs return
    float_t                     fltUrms;                   ///<Measured Rs return
    float_t                     fltIrms;                   ///<Measured Rs return
    float_t                     fltP;                   ///<Measured Rs return
    float_t                     fltQ;                   ///<Measured Rs return
    float_t                     fltI;                   ///<Measured Rs return
    float_t                     fltBulgarianConstantUrms;   ///<Measured Rs return
    bool_t                      bCalcPower;             ///<Measured Rs return
}MID_TEST_NOLOAD_A1_T;                                  ///<Structure containing necessary variable for Rs measurement

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

extern void MID_testNoLoad(MID_TEST_NOLOAD_A1_T* sNoLoadTest);

#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

#endif /* NOLOAD_TEST_H_ */
