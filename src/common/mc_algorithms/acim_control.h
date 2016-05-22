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
* @file         acim_control.h
*
* @brief        Control algorithms for ACIM header file
* 
*******************************************************************************/
#ifndef _ACIM_CONTROL_H_
#define _ACIM_CONTROL_H_

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "gflib_FP.h"
#include "gmclib_FP.h"
#include "gdflib_FP.h"
#include "amclib_FP.h"
#include "mlib_FP.h"
#include "mlib_types.h"
#include "arm_math.h"
#include <math.h>
#include "m1_acim_appconfig.h"

/******************************************************************************
| constants                         
|----------------------------------------------------------------------------*/
#define         F16MAXVAL       (32768.0F)

/******************************************************************************
| macros                        
|----------------------------------------------------------------------------*/

/******************************************************************************
| types                          
|----------------------------------------------------------------------------*/
/* RFO structure */
typedef struct
{
    acc32_t                             acc32RotFulxPos;  
    AMCLIB_ROT_FLUX_OBSRV_T_FLT         sCtrl;
    GMCLIB_2COOR_ALBE_T_FLT             sPsiRAlBe;   
    AMCLIB_ROT_FLUX_OBSRV_DEBUG_T_FLT   sCtrlDebug;
} MCS_ACIM_RFO_A2_T;

/* MRAS structure*/
typedef struct
{
    acc32_t                             acc32RotPos;
    AMCLIB_MRAS_T_FLT                   sCtrl;
    AMCLIB_MRAS_DEBUG_FLT               sCtrlDebug; 
} MCS_ACIM_MRAS_A2_T;

/* speed structure */
typedef struct
{
    GFLIB_CTRL_PI_P_AW_T_FLT            sSpeedPiParamsGFLIB;                    /* speed controller structure */
    GFLIB_RAMP_T_FLT                    sSpeedRampParams;                       /* speed ramp structure */
    float_t                             fltFilt;                                /* filtered mechanical speed value [rpm] */
    float_t                             fltRamp;                                /* ramped mechanical speed [rpm] */
    float_t                             fltCmd;                                 /* speed command [rpm] */
    float_t                             fltErrorSpeed;                          /* speed error [rpm] */
    float_t                             fltIdStartup;                           /* excitation current at speed FOC startup [A] */
    float_t                             fltIdStartupMinPct;                     /* d-axis current threshold to go spin state at speed FOC startup [%] */
    uint16_t                            ui16LoopCounter;                        /* loop counter */
    bool_t                              bSpeedPISatFlag;                        /* speed PI controller saturation flag */
} MCS_SPEED_A1_T;

/* Scalar structure */
typedef struct
{
    GFLIB_RAMP_T_FLT                    sScalarRampParams;                      /* scalar speed ramp structure */
    float_t                             fltSpeedScalarCmd;                      /* speed command [rpm] */
    float_t                             fltSpeedScalarCmdRamp;                  /* speed command ramped [rpm] */
    float_t                             fltPositionScalar;                      /* scalar position [rad]*/
    float_t                             fltScalarIntegGain;                     /* scalar integration constant */
    float_t                             fltScalarVrpmRatioGain;                 /* V/rpm constant [V/rpm] */
    float_t                             fltScalarUqMin;                         /* minimal voltage Uq [V] */
    uint16_t                            ui16LoopCounter;                        /* loop counter */
} MCS_SCALAR_A1_T;

/* ACIM FOC structure */
typedef struct
{
    MCS_ACIM_RFO_A2_T                   sRFO;                                   /* Rotor Flux Observer structure */
    MCS_ACIM_MRAS_A2_T                  sMRAS;                                  /* MRAS structure */
    GFLIB_CTRL_PI_P_AW_T_FLT            sIdPiParamsGFLIB;                       /* d-axis current controller struct */
    GFLIB_CTRL_PI_P_AW_T_FLT            sIqPiParamsGFLIB;                       /* q-axis current controller struct */
    GDFLIB_FILTER_IIR1_T_FLT            sUDcBusFilter;                          /* DC-bus voltage filter (FRAC16) */
    GMCLIB_3COOR_T_F16                  sIABCMeas;                              /* measured 3-phase current (FRAC16) */
    GMCLIB_3COOR_T_FLT                  sIABC;                                  /* measured 3-phase current [A] */
    GMCLIB_2COOR_ALBE_T_FLT             sIAlBe;                                 /* alpha/Beta current [A] */
    GMCLIB_2COOR_DQ_T_FLT               sIDQ;                                   /* DQ current in rotor flux coordinates [A] */    
    GMCLIB_2COOR_DQ_T_FLT               sIDQReq;                                /* required DQ current in rotor flux coordinates [A] */  
    GMCLIB_2COOR_DQ_T_FLT               sIDQError;                              /* DQ current error in rotor flux coordinates [A] */  
    GMCLIB_3COOR_T_F16                  sDutyABC;                               /* applied duty cycles ABC (FRAC16) */
    GMCLIB_2COOR_ALBE_T_FLT             sUAlBe;                                 /* Alpha/Beta stator voltage */
    GMCLIB_2COOR_ALBE_T_F16             sUAlBeComp_F16;                         /* Alpha/Beta stator voltage with DC-bus ripple elim (FRAC16)*/ 
    GMCLIB_2COOR_DQ_T_FLT               sUDQReq;                                /* required DQ voltage in rotor flux coordinates [V] */  
    GMCLIB_2COOR_SINCOS_T_FLT           sSCFOC;                                 /* sine & cosine of rotor flux angle [-] */
    float_t                             fltDutyCycleLimit;                      /* max. allowable duty cycle [-] */
    float_t                             fltUDcBus;                              /* DC-bus voltage [V] */
    float_t                             fltUDcBusFilt;                          /* filtered DC-bus voltage [V] */
    frac16_t                            f16UDcBusMeas;                          /* DC-bus voltage (FRAC16) */
    uint16_t                            ui16SectorSVM;                          /* SVM sector (sector) */     
    bool_t                              bIqPISatFlag;                           /* d-axis PI controller saturation flag */
    bool_t                              bIdPISatFlag;                           /* q-axis PI controller saturation flag */
    bool_t                              bSpeedStartup;                          /* motor startup flag */
} MCS_ACIM_FOC_A1_T;

/******************************************************************************
| global variables                        
|----------------------------------------------------------------------------*/

/******************************************************************************
| global functions                         
|----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern void MCS_ACIMOpenLoopScalarCtrlA1(MCS_ACIM_FOC_A1_T *psFocACIM, 
                                         MCS_SCALAR_A1_T *psScalarACIM);
extern void MCS_ACIMOpenLoopScalarCtrlA2(MCS_ACIM_FOC_A1_T *psFocACIM, 
                                         MCS_SCALAR_A1_T *psScalarACIM);
extern void MCS_ACIMFocCtrlVoltageA1(MCS_ACIM_FOC_A1_T *psFocACIM);
extern void MCS_ACIMFocCtrlVoltageA2(MCS_ACIM_FOC_A1_T *psFocACIM);
extern void MCS_ACIMFocCtrlCurrentA1(MCS_ACIM_FOC_A1_T *psFocACIM);
extern void MCS_ACIMSpeedCtrlA1(MCS_ACIM_FOC_A1_T *psFocACIM, 
                                MCS_SPEED_A1_T *psSpeed);

#ifdef __cplusplus
}
#endif


#endif /* _ACIM_CONTROL_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/