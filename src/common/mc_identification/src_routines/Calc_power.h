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
* @file     Calc_power.h
*
* @brief    Calulate 3-phase power routine
*
***************************************************************************//*!*/
#ifndef CALC_POWER_H_
#define CALC_POWER_H_

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

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
    struct
    {
        float fltA;
        float fltB;
        float fltC;
    }sUABC;

    struct
    {
        float fltA;
        float fltB;
        float fltC;
    }sIABC;
        
    float UrmsAvg;
    float IrmsAvg;
    float PAvg;
    float QAvg;
    float SAvg;
    
}MID_CALC_POWER_A1_T;

/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/
extern MID_CALC_POWER_A1_T     sMIDCalcPower;

/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

extern void MID_calcPower(MID_CALC_POWER_A1_T* sCalcPower);
extern void MID_readPower(MID_CALC_POWER_A1_T* sCalcPower);

#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

#endif /* CALC_POWER_H_ */
