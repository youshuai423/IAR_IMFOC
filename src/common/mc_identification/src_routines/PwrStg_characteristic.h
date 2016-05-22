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
* @file     PwrStg_characteristic.h
*
* @brief    Power Stage characterization routine
*
***************************************************************************//*!*/

#ifndef PWRSTG_CHARASTERISTIC_H_
#define PWRSTG_CHARASTERISTIC_H_

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "mid_def.h"
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
    uint16_t    ui16Active;                /* < Inidicates whether Transfer characteristic is being measured (true) or not (false) */
    uint16_t    ui16LoopCounter;           /* < Serves for timing to determine e.g. 600ms */
    float_t     fltRs;                     /* < Stator resistance of connected motor */
    float_t*    pfltIdReq;                 /* < Pointer to required current Id (PI current controller's input) */
    float_t*    pfltUdReq;                 /* < Pointer to required voltage Ud (PI current controller's output) */
    float_t*    pfltIdfbck;                /* < Pointer to actual real measured current Id */
    float_t     fltIdCalib;                /*< Range of the characterisation is from -f16IdCharMax to +f16IdCharMax */
    float_t     fltIdReqActual;            /* < Actual current at which is the characteristic being measured at the moment */
    float_t     fltUdErrorLookUp[M1_CHAR_CURRENT_POINT_NUMBERS];  /* < Ud error voltages */
    uint16_t    ui16LUTIndex;              /* < Lookup table index, counts up to 65 until a whole characteristic is measured */
    float_t     fltIdIncrement;            /* < Id increment after each 300ms */
}MID_GET_CHAR_A1_T;

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

extern void MID_GetTransferCharacteristic(MID_GET_CHAR_A1_T* sTransferCharFcn);

#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

#endif /* PWRSTG_CHARASTERISTIC_H_ */
