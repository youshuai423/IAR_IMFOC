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
* @file     mcdrv_pwma_pwm3ph.h
*
* @brief    A header file of eFlexPWM 3-phase PWM motor control driver
*
* @periph   Three sub-modules of eFlexPWM 
*
******************************************************************************/
#ifndef _MCDRV_PWMA_PWM3PH_H_
#define _MCDRV_PWMA_PWM3PH_H_

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
#define MCDRV_PWMA                           (1)

/******************************************************************************
| typedefs and structures
|----------------------------------------------------------------------------*/
typedef struct MCDRV_PWMA_PWM3PH_T           MCDRV_PWMA_PWM3PH_T;           
typedef struct MCDRV_PWMA_PWM3PH_INIT_T      MCDRV_PWMA_PWM3PH_INIT_T;            

/******************************************************************************
| structures
|----------------------------------------------------------------------------*/
struct MCDRV_PWMA_PWM3PH_T
{
    GMCLIB_3COOR_T_F16             *psUABC;                                     /* pointer to the 3-phase pwm duty cycles */
    PWM_Type                       *pui32PwmBaseAddress;                        /* PWMA base address */
    uint16_t                        ui16PhASubNum;                              /* PWMA phase A submodule number */
    uint16_t                        ui16PhBSubNum;                              /* PWMA phase B submodule number */
    uint16_t                        ui16PhCSubNum;                              /* PWMA phase C submodule number */
    uint16_t                        ui16FaultFixNum;                            /* PWMA fault number for fixed over-current fault detection */ 
    uint16_t                        ui16FaultAdjNum;                            /* PWMA fault number for adjustable over-current fault detection */ 
}; 

struct MCDRV_PWMA_PWM3PH_INIT_T
{
    PWM_Type                       *pui32PwmBaseAddress;                        /* PWMA base address */ 
    uint16_t                        ui16PhASubNum;                              /* PWMA phase A submodule number */
    uint16_t                        ui16PhBSubNum;                              /* PWMA phase B submodule number */
    uint16_t                        ui16PhCSubNum;                              /* PWMA phase C submodule number */
    uint16_t                        ui16FaultFixNum;                            /* PWMA fault number for fixed over-current fault detection */ 
    uint16_t                        ui16FaultAdjNum;                            /* PWMA fault number for adjustable over-current fault detection */ 
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

bool_t MCDRV_eFlexPwm3PhInit(MCDRV_PWMA_PWM3PH_T *this, 
                             MCDRV_PWMA_PWM3PH_INIT_T *init);
bool_t MCDRV_eFlexPwm3PhSet(MCDRV_PWMA_PWM3PH_T *this);
bool_t MCDRV_eFlexPwm3PhOutEn(MCDRV_PWMA_PWM3PH_T *this);
bool_t MCDRV_eFlexPwm3PhOutDis(MCDRV_PWMA_PWM3PH_T *this);
bool_t MCDRV_eFlexPwm3PhFltGet(MCDRV_PWMA_PWM3PH_T *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_PWMA_PWM3PH_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/