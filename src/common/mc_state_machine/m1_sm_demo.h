/*******************************************************************************
*
* Copyright 2015 Freescale Semiconductor, Inc.

*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details. 
*
****************************************************************************//*!
*
* @file     m1_sm_ref_sol.h
*
* @brief    Motor 1 state machine header file
*
******************************************************************************/
#ifndef _M1_STATEMACHINE_H_
#define _M1_STATEMACHINE_H_

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include <stdbool.h>

#include "state_machine.h"
#include "acim_control.h"

/* library headers */
#include "gmclib.h"
#include "gflib.h"
#include "gdflib.h"

/******************************************************************************
| constants
|----------------------------------------------------------------------------*/
#define MCRSP_TYPE      "acim"                                                  /* motor control package type */
#define MCRSP_VER       "1.0.0"                                                 /* motor control package version */

/******************************************************************************
| macros
|----------------------------------------------------------------------------*/
#define MC_FAULT_I_DCBUS_OVER      (0)                                          /* over-current fault flag */
#define MC_FAULT_U_DCBUS_UNDER     (1)                                          /* under-voltage fault flag */
#define MC_FAULT_U_DCBUS_OVER      (2)                                          /* over-voltage fault flag */
#define MC_FAULT_LOAD_OVER         (3)                                          /* over-load fault flag */
#define MC_FAULT_SPEED_OVER        (4)                                          /* over-speed fault flag */

/* sets the fault bit defined by faultid in the faults variable */
#define MC_FAULT_SET(faults, faultid) \
        (faults |= ((MCDEF_FAULT_T)1 << faultid))

/* clears the fault bit defined by faultid in the faults variable */
#define MC_FAULT_CLEAR(faults, faultid) \
        (faults &= ~((MCDEF_FAULT_T)1 << faultid))

/* check the fault bit defined by faultid in the faults variable, 
   returns 1 or 0 */
#define MC_FAULT_CHECK(faults, faultid) \
        ((faults & ((MCDEF_FAULT_T)1 << faultid)) >> faultid)

/* clears all fault bits in the faults variable */
#define MC_FAULT_CLEAR_ALL(faults) (faults = 0)

/* check if a fault bit is set in the faults variable, 0 = no fault */
#define MC_FAULT_ANY(faults) (faults > 0)

/* update a fault bit defined by faultid in the faults variable according to 
   the LSB of value */
#define MC_FAULT_UPDATE(faults, faultid, value) \
        {MC_FAULT_CLEAR(faults, faultid); \
         faults |= (((MC_FAULT_T)value & (MC_FAULT_T)1) << faultid);}

/******************************************************************************
| types
|----------------------------------------------------------------------------*/
/* run sub-states */
typedef enum {
    CALIB               = 0,
    READY               = 1,
    STARTUP             = 2,
    SPIN                = 3,
    FREEWHEEL           = 4,
} M1_RUN_SUBSTATE_T;    

/* software identification structure */
typedef struct 
{
    char    cBoardID[15];
    char    cMotorType[4];
    char    cAppVer[5];
    char    cAppType;
} APP_VER_T;

typedef uint16_t MCDEF_FAULT_T;

/* control modes of the motor */
typedef enum 
{
    CONTROL_MODE_SCALAR         = 0,
    CONTROL_MODE_VOLTAGE_FOC    = 1,
    CONTROL_MODE_CURRENT_FOC    = 2,
    CONTROL_MODE_SPEED_FOC      = 3,
} MCS_CONTROL_MODE_T;              

typedef struct
{
    float_t                     fltUDcBusOver;                                  /* DC-bus over voltage level */
    float_t                     fltUDcBusUnder;                                 /* DC-bus under voltage level */
    float_t                     fltSpeedOver;                                   /* over speed level */
    float_t                     fltSpeedOverLoad;                               /* over-load speed level */
    float_t                     fltSpeedMin;                                    /* minimum speed level */
    float_t                     fltSpeedNom;                                    /* nominal speed */  
} MCDEF_FAULT_THRESHOLDS_T;


/* ACIM FOC with RFO observer */
typedef struct
{
    MCS_ACIM_FOC_A1_T           sFocACIM;                                       /* field Oriented Cntrol structure */
    MCS_SPEED_A1_T              sSpeed;                                         /* speed control structure */
    MCS_SCALAR_A1_T             sScalarCtrl;                                    /* scalar control structure */
    MCDEF_FAULT_T               sFaultId;                                       /* application motor faults */
    MCDEF_FAULT_T               sFaultIdPending;                                /* fault pending structure */
    MCDEF_FAULT_THRESHOLDS_T    sFaultThresholds;                               /* fault tresholds */
    MCS_CONTROL_MODE_T          eControl;                                       /* MCAT control modes */
    int16_t                     ui16CounterState;                               /* main state counter */    
    int16_t                     ui16CounterSlowLoop;                            /* slow control loop counter */
    int16_t                     ui16TimeSlowLoop;                               /* slow control loop time count number */
    int16_t                     ui16TimeFullSpeedFreeWheel;                     /* freewheel time count number */
    int16_t                     ui16TimeCalibration;                            /* calibration time count number */
    int16_t                     ui16TimeFaultRelease;                           /* fault time count number */
    frac16_t                    f16AdcAuxSample;                                /* auxiliary ADC sample  */
    float_t                     fltIPMTemperature;                              /* IPM temperature in Celsius degrees */
    bool_t                      bFaultClearMan;                                 /* manual fault clear detection */
} MCDEF_ACIM_T;


/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/
extern bool_t           mbM1SwitchAppOnOff; 
extern MCDEF_ACIM_T     gsM1Drive;
extern SM_APP_CTRL_T    gsM1Ctrl;

/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
  
void M1_SetAppSwitch(bool_t bValue);
bool_t M1_GetAppSwitch();
void M1_SetSpeed(float_t fltSpeedCmd);
float_t M1_GetSpeed(void);
void M1_FaultDetection(void);  


#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/
#endif /* _M1_SM_REF_SOL_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
