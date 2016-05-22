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
* @file     mcdrv_ftm_pwm3ph.h
*
* @brief    A header file of flexTimer 3-phase PWM motor control driver
*
* @periph   8-channel FTM 
*
******************************************************************************/
#ifndef _MCDRV_FTM_PWM3PH_H_
#define _MCDRV_FTM_PWM3PH_H_

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "mlib.h"
#include "mlib_types.h"
#include "fsl_device_registers.h"
#include "gmclib.h"

/******************************************************************************
| defines and macros
|----------------------------------------------------------------------------*/
#define MCDRV_FTM                           (0)

/******************************************************************************
| typedefs and structures
|----------------------------------------------------------------------------*/
typedef struct MCDRV_FTM_PWM3PH_T           MCDRV_FTM_PWM3PH_T;                 
typedef struct MCDRV_FTM_PWM3PH_INIT_T      MCDRV_FTM_PWM3PH_INIT_T;            

/******************************************************************************
| structures
|----------------------------------------------------------------------------*/
struct MCDRV_FTM_PWM3PH_T
{
    GMCLIB_3COOR_T_F16            * psUABC;                                     /* pointer to the 3-phase pwm duty cycles */
    FTM_Type                      * pui32PwmBase;                               /* pointer to phase A top value */  
    uint16_t                        ui16ChanBottomPhA;                          /* number of channel for phase A bottom */
    uint16_t                        ui16ChanTopPhA;                             /* number of channel for phase A top */
    uint16_t                        ui16ChanBottomPhB;                          /* number of channel for phase B bottom */
    uint16_t                        ui16ChanTopPhB;                             /* number of channel for phase B top */
    uint16_t                        ui16ChanBottomPhC;                          /* number of channel for phase C bottom */
    uint16_t                        ui16ChanTopPhC;                             /* number of channel for phase C top */
    uint16_t                        ui16FaultNum;                               /* FTM Fault number for over current fault detection */ 
    uint16_t                        ui16FaultFixNum;                            /* FTM fault number for fixed over-current fault detection */ 
    uint16_t                        ui16FaultAdjNum;                            /* FTM fault number for adjustable over-current fault detection */ 
};

struct MCDRV_FTM_PWM3PH_INIT_T
{
    FTM_Type                      * pui32PwmBase;                               /* pointer to phase A top value */  
    uint16_t                        ui16ChanPairNumPhA;                         /* number of pair channel for phase A */
    uint16_t                        ui16ChanPairNumPhB;                         /* number of pair channel for phase B */
    uint16_t                        ui16ChanPairNumPhC;                         /* number of pair channel for phase C */
    uint16_t                        ui16FaultFixNum;                            /* FTM fault number for fixed over-current fault detection */ 
    uint16_t                        ui16FaultAdjNum;                            /* FTM fault number for adjustable over-current fault detection */ 
};
/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| exported function prototypes
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

bool_t MCDRV_FtmPwm3PhInit(MCDRV_FTM_PWM3PH_T *this, 
                           MCDRV_FTM_PWM3PH_INIT_T *init);
bool_t MCDRV_FtmPwm3PhSet(MCDRV_FTM_PWM3PH_T *this);
bool_t MCDRV_FtmPwm3PhOutEn(MCDRV_FTM_PWM3PH_T *this);
bool_t MCDRV_FtmPwm3PhOutDis(MCDRV_FTM_PWM3PH_T *this);
bool_t MCDRV_FtmPwm3PhFltGet(MCDRV_FTM_PWM3PH_T *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_FTM_PWM3PH_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/