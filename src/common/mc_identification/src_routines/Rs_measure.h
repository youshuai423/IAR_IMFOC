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
* @file     Rs_measure.h
*
* @brief    Stator resistance measurement routine
*
***************************************************************************//*!*/
#ifndef RS_MEASURE_H_
#define RS_MEASURE_H_

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "MID_def.h"
#include "mlib_FP.h"
#include "gflib_FP.h"

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
    GFLIB_LUT1D_T_FLT   sLUTparams;             ///<LUT parameters
    uint16_t            ui16Active;             ///<Inidicates whether Rs is being measured (true) or not (false)
    uint16_t            ui16LoopCounter;        ///<Serves for timing to determine e.g. 600ms
    float_t*            pfltIdReq;              ///<Pointer to required current Id (PI current controller's input)
    float_t             fltIdMeas;              ///<User defined measuring DC current
    float_t*            pfltUdReq;              ///<Pointer to required voltage Ud (PI current controller's output)
    float_t*            pfltIdfbck;             ///<Pointer to actual real measured current Id
    float_t             fltRs;                  ///<Measured Rs return
    float_t*            pfltUdErrorLookUp;      ///<Pointer to Look Up table with the transfer characteristic coefficients
    float_t             fltRescaleIdLUTGain;    ///<Gain when re-scaling current Id into range -2A to 2A for LUT
}MID_GET_RS_A1_T;                               ///<Structure containing necessary variable for Rs measurement

/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/
extern float_t  fltTransferCharError[];
extern uint16_t  ui16FaultMID;

/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

extern void MID_getRs(MID_GET_RS_A1_T* sRsMeasFcn);

#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

#endif /* RS_MEASURE_H_ */
