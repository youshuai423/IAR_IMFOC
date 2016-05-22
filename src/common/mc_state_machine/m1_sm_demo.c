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
* @file     m1_sm_ref_sol.c
*
* @brief    Motor 1 reference solution state machine 
*
******************************************************************************/
/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "m1_sm_demo.h"
#include "app_init.h"
#include "main.h"

/******************************************************************************
| constants                          
|----------------------------------------------------------------------------*/
#define M1_SVM_SECTOR_DEFAULT          (2)                                      /* default SVM sector */

/******************************************************************************
| macros                          
|----------------------------------------------------------------------------*/
/* max FRAC16 value */
#define F16_TO_FLT(f16In)              (f16In * (1.0F/32768.0F))
#define FLT_TO_F16(fltIn)              (fltIn * 32768.0F)

/******************************************************************************
| global variables                          
|----------------------------------------------------------------------------*/
/* main control structure */
MCDEF_ACIM_T                            gsM1Drive;
                                        
/* main application switch */           
bool_t                                  mbM1SwitchAppOnOff;
                                        
/* motor M1 structure */
M1_RUN_SUBSTATE_T                       meM1StateRun;    

/* 3-phase PWM actuator */              
extern M1_MCDRV_PWM3PH_T                msM1Pwm3ph;
                                        
/* current/voltage sensing sensor */
extern M1_MCDRV_ADC_T                   msM1AdcSensor;

/******************************************************************************
| local variables     
|----------------------------------------------------------------------------*/
/* FreeMASTER scales */
static volatile float                   fltM1voltageScale; 
static volatile float                   fltM1DCBvoltageScale;
static volatile float                   fltM1currentScale;
static volatile float                   fltM1speedScale;

/******************************************************************************
| local functions                          
|----------------------------------------------------------------------------*/
/* user state machine functions */
static void M1_StateFaultFast(void);
static void M1_StateInitFast(void);
static void M1_StateStopFast(void);
static void M1_StateRunFast(void);

static void M1_StateFaultSlow(void);
static void M1_StateInitSlow(void);
static void M1_StateStopSlow(void);
static void M1_StateRunSlow(void);

/* user state-transition functions */
static void M1_TransFaultInit(void);
static void M1_TransInitFault(void);
static void M1_TransInitStop(void);
static void M1_TransStopFault(void);
static void M1_TransStopRun(void);
static void M1_TransRunFault(void);
static void M1_TransRunStop(void);

/* state machine functions field */
/* as global variable used in main.c */
const SM_APP_STATE_FCN_T msSTATE_FAST = 
{
    M1_StateFaultFast, 
    M1_StateInitFast, 
    M1_StateStopFast, 
    M1_StateRunFast
};

const SM_APP_STATE_FCN_T msSTATE_SLOW = 
{
    M1_StateFaultSlow, 
    M1_StateInitSlow, 
    M1_StateStopSlow, 
    M1_StateRunSlow
};

/* state-transition functions field */
static const SM_APP_TRANS_FCN_T msTRANS = 
{
    M1_TransFaultInit, 
    M1_TransInitFault, 
    M1_TransInitStop, 
    M1_TransStopFault, 
    M1_TransStopRun, 
    M1_TransRunFault, 
    M1_TransRunStop
};

/* state machine structure declaration and initialization */
SM_APP_CTRL_T gsM1Ctrl = 
{
    /* gsM1Ctrl.psState, user state functions  */
    &msSTATE_FAST,
    
    /* gsM1Ctrl.psState, user state functions  */
    &msSTATE_SLOW,
     
    /* gsM1Ctrl.psTrans, user state-transition functions */
    &msTRANS,
 
    /* gsM1Ctrl.uiCtrl, deafult no control command */
    SM_CTRL_NONE,
      
    /* gsM1Ctrl.eState, default state after reset */
    INIT     
};

/* user sub-state machine functions */
static void M1_StateRunCalibFast(void);
static void M1_StateRunReadyFast(void);
static void M1_StateRunStartupFast(void);
static void M1_StateRunSpinFast(void);
static void M1_StateRunFreewheelFast(void);

static void M1_StateRunCalibSlow(void);
static void M1_StateRunReadySlow(void);
static void M1_StateRunStartupSlow(void);
static void M1_StateRunSpinSlow(void);
static void M1_StateRunFreewheelSlow(void);

/* user sub-state-transition functions */
static void M1_TransRunCalibReady(void);
static void M1_TransRunReadyStartup(void);
static void M1_TransRunStartupSpin(void);
static void M1_TransRunStartupFreewheel(void);
static void M1_TransRunSpinFreewheel(void);
static void M1_TransRunFreewheelReady(void);

/* sub-state machine functions field */
static const PFCN_VOID_VOID mM1_STATE_RUN_TABLE_FAST[6] = 
{
    M1_StateRunCalibFast, 
    M1_StateRunReadyFast, 
    M1_StateRunStartupFast, 
    M1_StateRunSpinFast, 
    M1_StateRunFreewheelFast, 
};

static const PFCN_VOID_VOID mM1_STATE_RUN_TABLE_SLOW[6] = 
{
    M1_StateRunCalibSlow, 
    M1_StateRunReadySlow, 
    M1_StateRunStartupSlow, 
    M1_StateRunSpinSlow, 
    M1_StateRunFreewheelSlow, 
};

/* additional functions */
static void M1_ClearFOCVariables(void);


/***************************************************************************//*!
@brief   FAULT state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateFaultFast(void)
{
    /* read 3-phase motor currents, otherwise the interrupt flag will not be 
       cleared */
    M1_MCDRV_CURR_3PH_GET(&msM1AdcSensor);
    
    /* read DC-bus voltage value */
    M1_MCDRV_DCBUS_VOLTAGE_GET(&msM1AdcSensor);
        
    /* get raw IPM temperature */
    M1_MCDRV_AUX_GET(&msM1AdcSensor);
    
    /* convert voltages from frac measured values to float */
    gsM1Drive.sFocACIM.fltUDcBus = (float)((F16_TO_FLT(fltM1DCBvoltageScale) * 
                                            gsM1Drive.sFocACIM.f16UDcBusMeas));
    
    /* filter DC-bus voltage */
    gsM1Drive.sFocACIM.fltUDcBusFilt = GDFLIB_FilterIIR1_FLT(
        gsM1Drive.sFocACIM.fltUDcBus, &gsM1Drive.sFocACIM.sUDcBusFilter);    
    
    /* disable user application switch */
    mbM1SwitchAppOnOff = FALSE;
     
    /* after fault condition ends wait defined time to clear fault state */
    if (!MC_FAULT_ANY(gsM1Drive.sFaultIdPending))
    {
        /* slow loop counter */
        if (--gsM1Drive.ui16CounterSlowLoop == 0)
        {
            /* initialize speed loop counter */
            gsM1Drive.ui16CounterSlowLoop = gsM1Drive.ui16TimeSlowLoop;
            
            if (--gsM1Drive.ui16CounterState == 0)
            {
                /* clear fault command */
                gsM1Ctrl.uiCtrl |= SM_CTRL_FAULT_CLEAR;
            }
        }
    }
    else
    {
        gsM1Drive.ui16CounterState = gsM1Drive.ui16TimeFaultRelease;
    }
    
    /* clear fault state manually from FreeMASTER */
    if(gsM1Drive.bFaultClearMan)
    {
        /* clear fault state */
        gsM1Ctrl.uiCtrl |= SM_CTRL_FAULT_CLEAR;
        gsM1Drive.bFaultClearMan = FALSE;
    }   
    
    /* detects faults */
    M1_FaultDetection();
}

/***************************************************************************//*!
@brief   State Init function FAST

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateInitFast(void)
{  
    /* type the code to do when in the INIT state */
    
    /* init scalar control */
    gsM1Drive.sScalarCtrl.fltScalarIntegGain            = M1_SCALAR_INTEG_GAIN;
    gsM1Drive.sScalarCtrl.fltScalarVrpmRatioGain        = M1_SCALAR_VHZ_FACTOR_GAIN;
    gsM1Drive.sScalarCtrl.fltScalarUqMin                = M1_SCALAR_UQ_MIN;
    gsM1Drive.sScalarCtrl.sScalarRampParams.fltRampUp   = M1_SCALAR_RAMP_UP;
    gsM1Drive.sScalarCtrl.sScalarRampParams.fltRampDown = M1_SCALAR_RAMP_DOWN;
    
    /* init RFO */
    gsM1Drive.sFocACIM.sRFO.sCtrl.fltKrInvGain          = M1_RFO_KR_INV_GAIN;
    gsM1Drive.sFocACIM.sRFO.sCtrl.fltKrLsTotLeakGain    = M1_RFO_LS_TOT_LEAK_GAIN;
    gsM1Drive.sFocACIM.sRFO.sCtrl.fltPsiRA1Gain         = M1_RFO_PSI_RA1_GAIN;
    gsM1Drive.sFocACIM.sRFO.sCtrl.fltPsiRB1Gain         = M1_RFO_PSI_RB1_GAIN;
    gsM1Drive.sFocACIM.sRFO.sCtrl.fltPsiSA1Gain         = M1_RFO_PSI_SA1_GAIN;
    gsM1Drive.sFocACIM.sRFO.sCtrl.fltPsiSA2Gain         = M1_RFO_PSI_SA2_GAIN;  
    gsM1Drive.sFocACIM.sRFO.sCtrl.fltRsEst              = M1_RFO_RS_EST;
    gsM1Drive.sFocACIM.sRFO.sCtrl.sCtrl.fltPGain        = M1_RFO_COMP_KP_GAIN;
    gsM1Drive.sFocACIM.sRFO.sCtrl.sCtrl.fltIGain        = M1_RFO_COMP_KI_GAIN;
    AMCLIB_RotFluxObsrvInit_FLT_FCi(&(gsM1Drive.sFocACIM.sRFO.sCtrl));
    
    /* init MRAS */
    gsM1Drive.sFocACIM.sMRAS.sCtrl.fltPsiRA1Gain        = M1_RFO_PSI_RA1_GAIN;
    gsM1Drive.sFocACIM.sMRAS.sCtrl.fltPsiRB1Gain        = M1_RFO_PSI_RB1_GAIN;
    gsM1Drive.sFocACIM.sMRAS.sCtrl.fltTs                = M1_SAMPLE_TIME;
    gsM1Drive.sFocACIM.sMRAS.sCtrl.sCtrl.fltIGain       = M1_MRAS_KI_GAIN;
    gsM1Drive.sFocACIM.sMRAS.sCtrl.sCtrl.fltPGain       = M1_MRAS_KP_GAIN; 
    gsM1Drive.sFocACIM.sMRAS.sCtrl.sCtrl.fltSpeedIIR1Param.sFltCoeff.fltB0 = M1_SPEED_IIR_B0;
    gsM1Drive.sFocACIM.sMRAS.sCtrl.sCtrl.fltSpeedIIR1Param.sFltCoeff.fltB1 = M1_SPEED_IIR_B1;
    gsM1Drive.sFocACIM.sMRAS.sCtrl.sCtrl.fltSpeedIIR1Param.sFltCoeff.fltA1 = M1_SPEED_IIR_A1;
    AMCLIB_MRASInit_A32ffa_FCi(&(gsM1Drive.sFocACIM.sMRAS.sCtrl));

    /* default MCAT control mode after reset */
    gsM1Drive.eControl                                  = CONTROL_MODE_SPEED_FOC;
    
    /* timing control and general variables */
    gsM1Drive.ui16CounterSlowLoop                       = 0;
    gsM1Drive.ui16TimeSlowLoop                          = M1_SPEED_LOOP_CNTR;
    gsM1Drive.ui16CounterState                          = 0;
    gsM1Drive.ui16TimeFullSpeedFreeWheel                = M1_FREEWHEEL_DURATION;
    gsM1Drive.ui16TimeCalibration                       = M1_CALIB_DURATION;
    gsM1Drive.ui16TimeFaultRelease                      = M1_FAULT_DURATION;
    mbM1SwitchAppOnOff                                  = FALSE;
    
    /* fault set to init states */
    MC_FAULT_CLEAR_ALL(gsM1Drive.sFaultId);
    MC_FAULT_CLEAR_ALL(gsM1Drive.sFaultIdPending);
    
    /* fault thresholds */
    gsM1Drive.sFaultThresholds.fltUDcBusOver            = M1_U_DCB_OVERVOLTAGE;
    gsM1Drive.sFaultThresholds.fltUDcBusUnder           = M1_U_DCB_UNDERVOLTAGE;
    gsM1Drive.sFaultThresholds.fltSpeedOver             = M1_N_OVERSPEED;
    gsM1Drive.sFaultThresholds.fltSpeedOverLoad         = M1_N_OVERLOAD;
    gsM1Drive.sFaultThresholds.fltSpeedMin              = M1_N_MIN;
    gsM1Drive.sFaultThresholds.fltSpeedNom              = M1_N_NOM;
        
    /* defined scaling for FreeMASTER */
    fltM1voltageScale                                   = M1_U_MAX;
    fltM1currentScale                                   = M1_I_MAX;
    fltM1DCBvoltageScale                                = M1_U_DCB_MAX;
    fltM1speedScale                                     = M1_N_MAX;
            
    /* ACIM FOC initialisation */
    gsM1Drive.sFocACIM.ui16SectorSVM                    = M1_SVM_SECTOR_DEFAULT;
    gsM1Drive.sFocACIM.f16UDcBusMeas                    = FRAC16(0.0);
    gsM1Drive.sFocACIM.fltUDcBusFilt                    = FRAC16(0.0);
    gsM1Drive.sFocACIM.sUDcBusFilter.sFltCoeff.fltB0    = M1_UDCB_IIR_B0;
    gsM1Drive.sFocACIM.sUDcBusFilter.sFltCoeff.fltB1    = M1_UDCB_IIR_B1;
    gsM1Drive.sFocACIM.sUDcBusFilter.sFltCoeff.fltA1    = M1_UDCB_IIR_A1;
    
     /* filter init not to enter to fault */
    gsM1Drive.sFocACIM.sUDcBusFilter.fltFltBfrX[0]      = M1_U_DCB_UNDERVOLTAGE/2.0 + M1_U_DCB_OVERVOLTAGE/2.0; 
    gsM1Drive.sFocACIM.sUDcBusFilter.fltFltBfrY[0]      = gsM1Drive.sFocACIM.sUDcBusFilter.fltFltBfrX[0];
    
    gsM1Drive.sFocACIM.sDutyABC.f16A                    = FRAC16(0.5);
    gsM1Drive.sFocACIM.sDutyABC.f16B                    = FRAC16(0.5);
    gsM1Drive.sFocACIM.sDutyABC.f16C                    = FRAC16(0.5);
    
    /* speed control */
    gsM1Drive.sSpeed.sSpeedRampParams.fltRampUp         = M1_SPEED_RAMP_UP;
    gsM1Drive.sSpeed.sSpeedRampParams.fltRampDown       = M1_SPEED_RAMP_DOWN;
    gsM1Drive.sSpeed.fltIdStartup                       = M1_ID_REQ;
    gsM1Drive.sSpeed.fltIdStartupMinPct                 = M1_SPEED_ID_MIN_STARTUP;
    
    gsM1Drive.sSpeed.sSpeedPiParamsGFLIB.fltPGain       = M1_SPEED_PI_PROP_GAIN;
    gsM1Drive.sSpeed.sSpeedPiParamsGFLIB.fltIGain       = M1_SPEED_PI_INTEG_GAIN;
    gsM1Drive.sSpeed.sSpeedPiParamsGFLIB.fltUpperLim    = M1_SPEED_LOOP_HIGH_LIMIT;
    gsM1Drive.sSpeed.sSpeedPiParamsGFLIB.fltLowerLim    = M1_SPEED_LOOP_LOW_LIMIT;
    GFLIB_CtrlPIpAWInit_FLT(0.0F,&gsM1Drive.sSpeed.sSpeedPiParamsGFLIB);

    /* field Oriented Control */
    gsM1Drive.sFocACIM.sIdPiParamsGFLIB.fltPGain        = M1_D_KP_GAIN;
    gsM1Drive.sFocACIM.sIdPiParamsGFLIB.fltIGain        = M1_D_KI_GAIN;
    GFLIB_CtrlPIpAWInit_FLT(0.0F,&gsM1Drive.sFocACIM.sIdPiParamsGFLIB);
    
    gsM1Drive.sFocACIM.sIqPiParamsGFLIB.fltPGain        = M1_Q_KP_GAIN;
    gsM1Drive.sFocACIM.sIqPiParamsGFLIB.fltIGain        = M1_Q_KI_GAIN;    
    GFLIB_CtrlPIpAWInit_FLT(0.0F,&gsM1Drive.sFocACIM.sIqPiParamsGFLIB);
       
    gsM1Drive.sFocACIM.fltUDcBus                        = 0.0F;
    gsM1Drive.sFocACIM.fltDutyCycleLimit                = M1_CLOOP_LIMIT;

    /* reset rest of FOC variables */
    M1_ClearFOCVariables();
       
    /* INIT_DONE command */
    gsM1Ctrl.uiCtrl |= SM_CTRL_INIT_DONE;
}

/***************************************************************************//*!
@brief   STOP state

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateStopFast(void)
{
    /* type the code to do when in the STOP state */
  
    /* read 3-phase motor currents, otherwise the interrupt flag will not 
       be cleared */
    M1_MCDRV_CURR_3PH_GET(&msM1AdcSensor);
    
    /* read DC-bus voltage value */
    M1_MCDRV_DCBUS_VOLTAGE_GET(&msM1AdcSensor);
        
    /* get raw IPM temperature */
    M1_MCDRV_AUX_GET(&msM1AdcSensor);

    /* convert voltages from fractional measured values to float */
    gsM1Drive.sFocACIM.fltUDcBus = (float)(fltM1DCBvoltageScale * 
        F16_TO_FLT(gsM1Drive.sFocACIM.f16UDcBusMeas));
    
    /* filter DC-bus voltage */
    gsM1Drive.sFocACIM.fltUDcBusFilt = GDFLIB_FilterIIR1_FLT(
        gsM1Drive.sFocACIM.fltUDcBus,&gsM1Drive.sFocACIM.sUDcBusFilter);
   
    /* if the user switches on or set non-zero speed */
    if ((mbM1SwitchAppOnOff != 0) || (gsM1Drive.sSpeed.fltCmd != 0.0F))
    {
        /* set the switch on */
        mbM1SwitchAppOnOff = 1;  
        
        /* start command */
        gsM1Ctrl.uiCtrl |= SM_CTRL_START;
    }
    
    /* detect fault */
    M1_FaultDetection();
  
    /* if a fault occurred */
    if (gsM1Drive.sFaultIdPending)
    {
        /* switches to the FAULT state */
        gsM1Ctrl.uiCtrl |= SM_CTRL_FAULT;    
    }
}

/***************************************************************************//*!
@brief   RUN state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunFast(void)
{
    /* type the code to do when in the RUN state */
  
    /* if the user switches off */
    if (!mbM1SwitchAppOnOff)
    {
        /* stop command */
        gsM1Ctrl.uiCtrl |= SM_CTRL_STOP;    
    }
        
    /* detect fault */
    M1_FaultDetection();
    
    /* if a fault occurred */
    if ( gsM1Drive.sFaultIdPending != 0)
    {
        /* switches to the FAULT state */
        gsM1Ctrl.uiCtrl |= SM_CTRL_FAULT;    
    }
  
    /* type the code to do when in the RUN state */
    M1_MCDRV_CURR_3PH_GET(&msM1AdcSensor);
    
    /* convert phase currents from fractional measured values to float */
    gsM1Drive.sFocACIM.sIABC.fltA = (float)(fltM1currentScale * 
        F16_TO_FLT(gsM1Drive.sFocACIM.sIABCMeas.f16A));
    gsM1Drive.sFocACIM.sIABC.fltB = (float)(fltM1currentScale * 
        F16_TO_FLT(gsM1Drive.sFocACIM.sIABCMeas.f16B));
    gsM1Drive.sFocACIM.sIABC.fltC = (float)(fltM1currentScale * 
        F16_TO_FLT(gsM1Drive.sFocACIM.sIABCMeas.f16C));

    /* read DC-bus voltage value */
    M1_MCDRV_DCBUS_VOLTAGE_GET(&msM1AdcSensor);

    /* convert voltages from fractional measured values to float */
    gsM1Drive.sFocACIM.fltUDcBus = (float)(fltM1DCBvoltageScale * 
        F16_TO_FLT(gsM1Drive.sFocACIM.f16UDcBusMeas));
    
    /* filter DC-bus voltage */
    gsM1Drive.sFocACIM.fltUDcBusFilt = GDFLIB_FilterIIR1_FLT(
        gsM1Drive.sFocACIM.fltUDcBus,&gsM1Drive.sFocACIM.sUDcBusFilter);
    
    /* read speed */
    gsM1Drive.sSpeed.fltFilt = (60.0F/(2*FLOAT_PI*M1_MOTOR_PP)) * 
        gsM1Drive.sFocACIM.sMRAS.sCtrlDebug.fltSpeedIIR1;
    
    /* get raw IPM temperature */
    M1_MCDRV_AUX_GET(&msM1AdcSensor);
    
    /* run sub-state function */
    mM1_STATE_RUN_TABLE_FAST[meM1StateRun]();       
  
    /* PWM peripheral update */
    M1_MCDRV_PWM3PH_SET(&msM1Pwm3ph);
    
    /* set current sensor for  sampling */
    M1_MCDRV_CURR_3PH_CHAN_ASSIGN(&msM1AdcSensor);
    
}
/***************************************************************************//*!
@brief   FAULT state SLOW


@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateFaultSlow(void)
{
    /* type the code to do when in the FAULT state */ 
  
    /* get IPM temperature in Celsius degrees */
    gsM1Drive.fltIPMTemperature = M1_GetIPMTemperature(&msM1AdcSensor);
}

/***************************************************************************//*!
@brief   INIT state SLOW

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateInitSlow(void)
{
    /* type the code to do when in the INIT state */
  
}

/***************************************************************************//*!
@brief   STOP state SLOW

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateStopSlow(void)
{
    /* type the code to do when in the FAULT state */
  
    /* get IPM temperature in Celsius degrees */
    gsM1Drive.fltIPMTemperature = M1_GetIPMTemperature(&msM1AdcSensor);
}

/***************************************************************************//*!
@brief   RUN state SLOW

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunSlow(void)
{
    /* type the code to do when in the RUN state */
  
    /* run sub-state function */
    mM1_STATE_RUN_TABLE_SLOW[meM1StateRun]();
    
    /* get IPM temperature in Celsius degrees */
    gsM1Drive.fltIPMTemperature = M1_GetIPMTemperature(&msM1AdcSensor);
}

/***************************************************************************//*!
@brief   FAULT to INIT transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransFaultInit(void)
{
    /* type the code to do when going from the FAULT to the INIT state */
}

/***************************************************************************//*!
@brief   INIT to FAULT transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransInitFault(void)
{  
    /* type the code to do when going from the INIT to the FAULT state */
  
    /* disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&msM1Pwm3ph);
    
    /* clear FOC variables */
    M1_ClearFOCVariables();
    
    /* init slow loop counter */
    gsM1Drive.ui16CounterSlowLoop = 1;
}

/***************************************************************************//*!
@brief   INIT to STOP transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransInitStop(void)
{
    /* type the code to do when going from the INIT to the STOP state */

    /* disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&msM1Pwm3ph);
}

/***************************************************************************//*!
@brief   STOP to FAULT transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransStopFault(void)
{
    /* type the code to do when going from the STOP to the FAULT state */

    /* disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&msM1Pwm3ph);
    
    gsM1Drive.sFaultId = gsM1Drive.sFaultIdPending;
    gsM1Drive.ui16CounterState = gsM1Drive.ui16TimeFaultRelease;   
    
    /* init slow loop counter */
    gsM1Drive.ui16CounterSlowLoop = 1;
}

/***************************************************************************//*!
@brief   STOP to RUN transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransStopRun(void)
{        
    /* type the code to do when going from the STOP to the RUN state */
  
    /* 50% duty cycle */  
    gsM1Drive.sFocACIM.sDutyABC.f16A = FRAC16(0.5);
    gsM1Drive.sFocACIM.sDutyABC.f16B = FRAC16(0.5);
    gsM1Drive.sFocACIM.sDutyABC.f16C = FRAC16(0.5);
    
    /* PWM duty cycles calculation and update */
    M1_MCDRV_PWM3PH_SET(&msM1Pwm3ph);    
    
    /* clear offset filters */
    M1_MCDRV_CURR_3PH_CALIB_INIT(&msM1AdcSensor);
    
    /* enable PWM output */
    M1_MCDRV_PWM3PH_EN(&msM1Pwm3ph);
      
    /* pass calibration routine duration to state counter*/
    gsM1Drive.ui16CounterState = gsM1Drive.ui16TimeCalibration;

    /* init slow loop counter  */
    gsM1Drive.ui16CounterSlowLoop = 1;
            
    /* calibration sub-state when transition to RUN */
    meM1StateRun = CALIB;
                
    /* acknoledge that the system can proceed into the RUN state */
    gsM1Ctrl.uiCtrl |= SM_CTRL_RUN_ACK;
}

/***************************************************************************//*!
@brief   RUN to FAULT transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransRunFault(void)
{
    /* type the code to do when going from the RUN to the FAULT state */
  
    /* disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&msM1Pwm3ph);

    /* clear FOC variables */
    M1_ClearFOCVariables();
    
    /* init slow loop counter */
    gsM1Drive.ui16CounterSlowLoop = 1;
}

/***************************************************************************//*!
@brief   RUN to STOP transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransRunStop(void)
{  
    /* type the code to do when going from the RUN to the STOP state */
  
    /* disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&msM1Pwm3ph);

    /* clear FOC variables */
    M1_ClearFOCVariables();

    /* acknowledge that the system can proceed into the STOP state */
    gsM1Ctrl.uiCtrl |= SM_CTRL_STOP_ACK;
}

/***************************************************************************//*!
@brief   RUN CALIB sub-state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunCalibFast(void)
{     
    /* type the code to do when in the RUN CALIB sub-state */
  
    /* call offset measurement */
    M1_MCDRV_CURR_3PH_CALIB(&msM1AdcSensor);

    /* change SVM sector in range <1;6> to measure all AD channel mapping 
       combinations */
    if(++gsM1Drive.sFocACIM.ui16SectorSVM > 6)
            gsM1Drive.sFocACIM.ui16SectorSVM = 1;
}


/***************************************************************************//*!
@brief   RUN READY sub-state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunReadyFast(void)
{
    /* type the code to do when in the RUN READY sub-state */
        
    /* MCAT control structure switch */
    switch (gsM1Drive.eControl)
    {
    case CONTROL_MODE_SCALAR: 
        /* scalar control */
        if (gsM1Drive.sScalarCtrl.fltSpeedScalarCmd != 0.0F)
        {
            gsM1Drive.sFocACIM.sUDQReq.fltD = 0.0F; 
            gsM1Drive.sFocACIM.sUDQReq.fltQ = 0.0F; 
            
            /* transition to the RUN STARTUP sub-state */
            M1_TransRunReadyStartup();
        }
        break;
        
    case CONTROL_MODE_VOLTAGE_FOC:
        /* voltage FOC */
        if ((gsM1Drive.sFocACIM.sUDQReq.fltD != 0) && 
            (gsM1Drive.sFocACIM.sUDQReq.fltQ != 0))
        {
            if(gsM1Drive.sFocACIM.sUDQReq.fltQ > 0)
            {
                gsM1Drive.sSpeed.fltCmd = 
                    2*gsM1Drive.sFaultThresholds.fltSpeedMin;
            }
            else
            {
                gsM1Drive.sSpeed.fltCmd = 
                    -2*gsM1Drive.sFaultThresholds.fltSpeedMin;
            }
           
            /* transition to the RUN STARTUP sub-state */
            M1_TransRunReadyStartup();
        }
        break;                      
        
    case CONTROL_MODE_CURRENT_FOC: 
        /* current FOC */
        if ((gsM1Drive.sFocACIM.sIDQReq.fltD != 0) && 
            (gsM1Drive.sFocACIM.sIDQReq.fltQ != 0))
        {
            if(gsM1Drive.sFocACIM.sIDQReq.fltQ > 0)
            {
                gsM1Drive.sSpeed.fltCmd = 
                    2*gsM1Drive.sFaultThresholds.fltSpeedMin;
            }
            else
            {
                gsM1Drive.sSpeed.fltCmd = 
                    -2*gsM1Drive.sFaultThresholds.fltSpeedMin;
            }
            
            /* transition to the RUN STARTUP sub-state */
            M1_TransRunReadyStartup();
        }
        break;  
        
    case CONTROL_MODE_SPEED_FOC:
    default:  
        /* full (speed FOC) */
        if((MLIB_Abs_FLT(gsM1Drive.sSpeed.fltCmd) > 
            gsM1Drive.sFaultThresholds.fltSpeedMin)&&\
           (MLIB_Abs_FLT(gsM1Drive.sSpeed.fltCmd) <= 
            gsM1Drive.sFaultThresholds.fltSpeedNom))
        {
            /* transition to the RUN STARTUP sub-state */
            M1_TransRunReadyStartup();
        }
        else
        {
            gsM1Drive.sSpeed.fltCmd = 0.0F;
        }
    }
}

/***************************************************************************//*!
@brief   RUN STARTUP sub-state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunStartupFast(void)
{
    /* type the code to do when in the RUN STARTUP sub-state */
  
    /* MCAT control structure switch */
    switch (gsM1Drive.eControl)
    {
    case CONTROL_MODE_SCALAR: 
    case CONTROL_MODE_VOLTAGE_FOC:   
    case CONTROL_MODE_CURRENT_FOC:
        /* switch directly to SPIN state */
        M1_TransRunStartupSpin();
        break;                 
    
    case CONTROL_MODE_SPEED_FOC:
    default:
        /* if user changes speed out of limits */
        if((MLIB_Abs_FLT(gsM1Drive.sSpeed.fltCmd) < 
            gsM1Drive.sFaultThresholds.fltSpeedMin)||\
           (MLIB_Abs_FLT(gsM1Drive.sSpeed.fltCmd) >= 
            gsM1Drive.sFaultThresholds.fltSpeedNom))
        {
            /* switch to FREEWHEEL state */
            M1_TransRunStartupFreewheel();
            return;
        }
        
        /* reset RFO and MRAS */
        AMCLIB_RotFluxObsrvInit_FLT_FCi(&(gsM1Drive.sFocACIM.sRFO.sCtrl));
        AMCLIB_MRASInit_A32ffa_FCi(&(gsM1Drive.sFocACIM.sMRAS.sCtrl));
        
        /* generate no torque and magnetize rotor*/
        gsM1Drive.sFocACIM.sIDQReq.fltQ = 0.0F;
        gsM1Drive.sFocACIM.sIDQReq.fltD = gsM1Drive.sSpeed.fltIdStartup;
          
        /* current control */
        MCS_ACIMFocCtrlCurrentA1(&gsM1Drive.sFocACIM);
        
        /* if sufficient time elapsed go to spin */
        /* if sufficient time elapsed go to spin */
        if(gsM1Drive.sFocACIM.sIDQ.fltD > (gsM1Drive.sSpeed.fltIdStartupMinPct * 
            gsM1Drive.sSpeed.fltIdStartup))
        {
            M1_TransRunStartupSpin();
        }
        break;
    }
}

/***************************************************************************//*!
@brief   RUN SPIN sub-state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunSpinFast(void)
{
    /* type the code to do when in the RUN SPIN sub-state */
  
    /* MCAT control structure switch */
    switch (gsM1Drive.eControl)
    {
    case CONTROL_MODE_SCALAR: 
        /* scalar control */
        MCS_ACIMOpenLoopScalarCtrlA1(&gsM1Drive.sFocACIM, 
                                     &gsM1Drive.sScalarCtrl);
        
        /* check whether the speed is below minimum */
        if((gsM1Drive.sScalarCtrl.fltSpeedScalarCmd == 0.0F) && \
            MLIB_Abs_FLT(gsM1Drive.sSpeed.sSpeedRampParams.fltState) < 
            gsM1Drive.sFaultThresholds.fltSpeedMin)
        {
            /* sub-state RUN FREEWHEEL */
            M1_TransRunSpinFreewheel();
        }
        break;

    case CONTROL_MODE_VOLTAGE_FOC:
        /* voltage FOC */
        MCS_ACIMFocCtrlVoltageA2(&gsM1Drive.sFocACIM);
        
        /* check whether the required voltages are non-zero */
        if((gsM1Drive.sFocACIM.sUDQReq.fltD == 0.0F) || 
           (gsM1Drive.sFocACIM.sUDQReq.fltQ == 0.0F))
        {
            /* sub-state RUN FREEWHEEL */
            M1_TransRunSpinFreewheel();
        }
        break;               

    case CONTROL_MODE_CURRENT_FOC: 
        /* current FOC */
        MCS_ACIMFocCtrlCurrentA1(&gsM1Drive.sFocACIM);
        
        /* check whether the required currents are non-zero */
        if((gsM1Drive.sFocACIM.sIDQReq.fltD == 0.0F) || 
           (gsM1Drive.sFocACIM.sIDQReq.fltQ == 0.0F))
        {
            /* sub-state RUN FREEWHEEL */
            M1_TransRunSpinFreewheel();
        }
        break;
          
    case CONTROL_MODE_SPEED_FOC:
    default: 
        /* full (speed) FOC */
        MCS_ACIMFocCtrlCurrentA1(&gsM1Drive.sFocACIM);
        break; 
    }
}

/***************************************************************************//*!
@brief   RUN FREEWHEEL sub-state FAST

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunFreewheelFast(void)
{        
    /* type the code to do when in the RUN FREEWHEEL sub-state */

    /* clear actual speed values */
    gsM1Drive.sSpeed.fltRamp        = 0.0F;

    /* control variables */
    gsM1Drive.sFocACIM.sIDQReq.fltD = 0.0F;
    gsM1Drive.sFocACIM.sIDQReq.fltQ = 0.0F;
    gsM1Drive.sFocACIM.sUDQReq.fltD = 0.0F;
    gsM1Drive.sFocACIM.sUDQReq.fltQ = 0.0F;

    /* slow loop */
    if (--gsM1Drive.ui16CounterSlowLoop == 0)
    {
        /* initialize speed loop counter */
        gsM1Drive.ui16CounterSlowLoop = gsM1Drive.ui16TimeSlowLoop;

        if (--gsM1Drive.ui16CounterState == 0)
        {
            /* sub-state RUN READY */
            M1_TransRunFreewheelReady();
        }        
    }
}

/***************************************************************************//*!
@brief   RUN CALIB sub-state SLOW

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunCalibSlow(void)
{    
    /* type the code to do when in the RUN CALIB sub-state */
  
    if(--gsM1Drive.ui16CounterState == 0)
    {
        /* write calibrated offset values */
        M1_MCDRV_CURR_3PH_CALIB_SET(&msM1AdcSensor);            
    
        /* to switch to the RUN READY sub-state */
        M1_TransRunCalibReady();
    }
}

/***************************************************************************//*!
@brief   RUN READY sub-state SLOW

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunReadySlow(void)
{
    /* type the code to do when in the RUN READY sub-state */
}

/***************************************************************************//*!
@brief   RUN STARTUP sub-state SLOW

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunStartupSlow(void)
{
    /* type the code to do when in the RUN STARTUP sub-state */
}

/***************************************************************************//*!
@brief   RUN SPIN sub-state SLOW

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunSpinSlow(void)
{
    /* type the code to do when in the RUN SPIN sub-state */
  
    if (gsM1Drive.eControl != CONTROL_MODE_SCALAR &&
        gsM1Drive.eControl != CONTROL_MODE_VOLTAGE_FOC &&
        gsM1Drive.eControl != CONTROL_MODE_CURRENT_FOC)
    {
        /* speed reverse */
        if((gsM1Drive.sSpeed.fltCmd > gsM1Drive.sFaultThresholds.fltSpeedMin)||\
           (gsM1Drive.sSpeed.fltCmd < -gsM1Drive.sFaultThresholds.fltSpeedMin))
        {    
            /* pass required speed values lower than nominal speed */
            if((MLIB_Abs_FLT(gsM1Drive.sSpeed.fltCmd) > 
                gsM1Drive.sFaultThresholds.fltSpeedNom))
            {    
                /* set required speed to nominal speed if over speed command > 
                   speed nominal */
                if(gsM1Drive.sSpeed.fltCmd > 0.0F)
                {
                    gsM1Drive.sSpeed.fltCmd = 
                        gsM1Drive.sFaultThresholds.fltSpeedNom;
                }
                else
                {
                    gsM1Drive.sSpeed.fltCmd = 
                        -gsM1Drive.sFaultThresholds.fltSpeedNom;
                }
            }
        }
        else
        {
            gsM1Drive.sSpeed.fltRamp = 0.0F;
            if(MLIB_Abs_FLT(gsM1Drive.sSpeed.sSpeedRampParams.fltState) < 
               gsM1Drive.sFaultThresholds.fltSpeedMin)
            {
                /* sub-state RUN FREEWHEEL */
                M1_TransRunSpinFreewheel();
            }
        }
        
        /* call speed control */
        MCS_ACIMSpeedCtrlA1(&gsM1Drive.sFocACIM, &gsM1Drive.sSpeed);
    }
}

/***************************************************************************//*!
@brief   RUN FREEWHEEL sub-state SLOW

@param   N/A

@return  N/A
******************************************************************************/
static void M1_StateRunFreewheelSlow(void)
{
    /* type the code to do when in the RUN FREEWHEEL sub-state */
}

/***************************************************************************//*!
@brief   RUN CALIB to READY transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransRunCalibReady(void)
{
    /* type the code to do when going from the RUN CALIB to the RUN READY 
       sub-state */

    /* set 50% PWM duty cycle */
    gsM1Drive.sFocACIM.sDutyABC.f16A = FRAC16(0.5);
    gsM1Drive.sFocACIM.sDutyABC.f16B = FRAC16(0.5);
    gsM1Drive.sFocACIM.sDutyABC.f16C = FRAC16(0.5);
    
    /* init slow loop counter  */
    gsM1Drive.ui16CounterSlowLoop = 1;        

    /* switch to sub state READY */
    meM1StateRun = READY;
}

/***************************************************************************//*!
@brief   RUN READY to ALIGN transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransRunReadyStartup(void)
{
    /* type the code to do when going from the RUN READY to the RUN STARTUP 
       sub-state */
    
    /* set the startup flag */
    gsM1Drive.sFocACIM.bSpeedStartup = TRUE;
  
    /* switch to sub state STARTUP */
    meM1StateRun = STARTUP;
}


/***************************************************************************//*!
@brief   RUN STARTUP to SPIN transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransRunStartupSpin(void)
{
    /* type the code to do when going from the RUN STARTUP to the RUN SPIN 
       sub-state */
  
    /* clear the startup flag */
    gsM1Drive.sFocACIM.bSpeedStartup = FALSE;
  
    /* to switch to the RUN SPIN sub-state */
    meM1StateRun = SPIN;
}


/******************************************************************************
 @brief   RUN STARTUP to FREEWHEEL transition

 @param   N/A

 @return  N/A
******************************************************************************/
static void M1_TransRunStartupFreewheel(void)
{
    /* type the code to do when going from the RUN STARTUP to the RUN FREEWHEEL 
       sub-state */
  
    /* turn off all transistors */
    M1_MCDRV_PWM3PH_DIS(&msM1Pwm3ph);
    
    /* clear application parameters */
    M1_ClearFOCVariables();
          
    /* freewheel duration set-up */
    gsM1Drive.ui16CounterState = gsM1Drive.ui16TimeFullSpeedFreeWheel;
    
    /* sub-state RUN FREEWHEEL */
    meM1StateRun = FREEWHEEL;
}

/******************************************************************************
@brief   RUN SPIN to FREEWHEEL transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransRunSpinFreewheel(void)
{
    /* type the code to do when going from the RUN SPIN to the RUN FREEWHEEL 
       sub-state */
  
    /* turn off all transistors */
    M1_MCDRV_PWM3PH_DIS(&msM1Pwm3ph);
    
    /* clear application parameters */
    M1_ClearFOCVariables();

    /* init slow loop counter  */
    gsM1Drive.ui16CounterSlowLoop = 1;
                        
    /* cenerates a time gap before the alignment to assure the rotor is not 
       rotating */
    gsM1Drive.ui16CounterState = gsM1Drive.ui16TimeFullSpeedFreeWheel;                 

    /* sub-state RUN FREEWHEEL */
    meM1StateRun = FREEWHEEL;    
}


/***************************************************************************//*!
@brief   RUN FREEWHEEL to READY transition

@param   N/A

@return  N/A
******************************************************************************/
static void M1_TransRunFreewheelReady(void)
{
    /* type the code to do when going from the RUN FREEWHEEL to the RUN READY 
       sub-state */
  
    /* enable PWM output */
    M1_MCDRV_PWM3PH_EN(&msM1Pwm3ph);
        
    /* init slow loop counter  */
    gsM1Drive.ui16CounterSlowLoop = 1;       
    
    /* sub-state RUN READY */
    meM1StateRun = READY;
}

/******************************************************************************
Global functions
******************************************************************************/

/***************************************************************************//*!                                         
@brief   Clear FOC variables

@param   N/A

@return  N/A
******************************************************************************/
static void M1_ClearFOCVariables(void)
{
    AMCLIB_RotFluxObsrvInit_FLT_FCi(&(gsM1Drive.sFocACIM.sRFO.sCtrl));
    AMCLIB_MRASInit_A32ffa_FCi(&(gsM1Drive.sFocACIM.sMRAS.sCtrl));

    /* ACIM FOC */
    gsM1Drive.sFocACIM.sUDQReq.fltD                   = 0.0F;
    gsM1Drive.sFocACIM.sUDQReq.fltQ                   = 0.0F;
    gsM1Drive.sFocACIM.sUAlBe.fltAlpha                = 0.0F;
    gsM1Drive.sFocACIM.sUAlBe.fltBeta                 = 0.0F;
    gsM1Drive.sFocACIM.sDutyABC.f16A                  = FRAC16(0.5);
    gsM1Drive.sFocACIM.sDutyABC.f16B                  = FRAC16(0.5);
    gsM1Drive.sFocACIM.sDutyABC.f16C                  = FRAC16(0.5);
                                                      
    gsM1Drive.sFocACIM.sIAlBe.fltAlpha                = 0.0F;
    gsM1Drive.sFocACIM.sIAlBe.fltBeta                 = 0.0F;
    gsM1Drive.sFocACIM.sIDQ.fltD                      = 0.0F;
    gsM1Drive.sFocACIM.sIDQ.fltQ                      = 0.0F;
    gsM1Drive.sFocACIM.sIDQReq.fltD                   = 0.0F;
    gsM1Drive.sFocACIM.sIDQReq.fltQ                   = 0.0F;
    gsM1Drive.sFocACIM.sIABC.fltA                     = 0.0F;
    gsM1Drive.sFocACIM.sIABC.fltB                     = 0.0F;
    gsM1Drive.sFocACIM.sIABC.fltC                     = 0.0F;
                                                      
    gsM1Drive.sFocACIM.sIdPiParamsGFLIB.fltIAccK_1    = 0.0F;
    gsM1Drive.sFocACIM.sIdPiParamsGFLIB.fltInErrK_1   = 0.0F;
    gsM1Drive.sFocACIM.sIdPiParamsGFLIB.bLimFlag      = FALSE;
    gsM1Drive.sFocACIM.sIDQError.fltD                 = 0.0F;
    gsM1Drive.sFocACIM.bIdPISatFlag                   = FALSE;
                                                     
    gsM1Drive.sFocACIM.sIqPiParamsGFLIB.fltIAccK_1    = 0.0F;
    gsM1Drive.sFocACIM.sIqPiParamsGFLIB.fltInErrK_1   = 0.0F;
    gsM1Drive.sFocACIM.sIqPiParamsGFLIB.bLimFlag      = FALSE;
    gsM1Drive.sFocACIM.sIDQError.fltQ                 = 0.0F;
    gsM1Drive.sFocACIM.bIqPISatFlag                   = FALSE;
                                                      
    gsM1Drive.sFocACIM.sSCFOC.fltSin                  = 0.0F;
    gsM1Drive.sFocACIM.sSCFOC.fltCos                  = 1.0F;
    
    /* speed cotnrol */
    gsM1Drive.sSpeed.fltCmd                           = 0.0F;
    gsM1Drive.sSpeed.fltRamp                          = 0.0F;
    gsM1Drive.sSpeed.fltFilt                          = 0.0F;
    gsM1Drive.sSpeed.sSpeedRampParams.fltState        = 0.0F;
    
    gsM1Drive.sSpeed.sSpeedPiParamsGFLIB.fltIAccK_1   = 0.0F;
    gsM1Drive.sSpeed.sSpeedPiParamsGFLIB.fltInErrK_1  = 0.0F;
    gsM1Drive.sSpeed.sSpeedPiParamsGFLIB.bLimFlag     = FALSE;
    gsM1Drive.sSpeed.fltErrorSpeed                    = 0.0F;
    gsM1Drive.sSpeed.bSpeedPISatFlag                  = FALSE;
    
    gsM1Drive.sFocACIM.sMRAS.sCtrl.sCtrl.fltSpeedIIR1Param.fltFltBfrX[0] = 0.0F;
    gsM1Drive.sFocACIM.sMRAS.sCtrl.sCtrl.fltSpeedIIR1Param.fltFltBfrY[0] = 0.0F;
    gsM1Drive.sFocACIM.sMRAS.sCtrlDebug.fltSpeedIIR1                     = 0.0F;
        
}

/******************************************************************************
| global functions
|----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief   Set the app switch function

@param   N/A

@return  N/A
******************************************************************************/
void M1_SetAppSwitch(bool_t bValue)
{
    mbM1SwitchAppOnOff = bValue;
}

/***************************************************************************//*!
@brief   Read the app switch function

@param   N/A

@return  N/A
******************************************************************************/
bool_t M1_GetAppSwitch()
{
    return (mbM1SwitchAppOnOff);
}

/***************************************************************************//*!
@brief   Set the speed command function

@param   N/A

@return  N/A
******************************************************************************/
void M1_SetSpeed(float fltSpeedCmd)
{
    if(mbM1SwitchAppOnOff)
    {
        /* set speed */
        if(MLIB_Abs_FLT(fltSpeedCmd) < gsM1Drive.sFaultThresholds.fltSpeedMin)
        {
            gsM1Drive.sSpeed.fltCmd = 0;
        }
        else if(MLIB_Abs_FLT(fltSpeedCmd) > gsM1Drive.sFaultThresholds.fltSpeedNom)
        {
            gsM1Drive.sSpeed.fltCmd = 0;
        }
        else
        {
            gsM1Drive.sSpeed.fltCmd = fltSpeedCmd;    
        }
    }
    else
    {    
        /* set zero speed */
        gsM1Drive.sSpeed.fltRamp = 0;
    } 
}

/***************************************************************************//*!
@brief   Get the speed command function

@param   N/A

@return  N/A
******************************************************************************/
float M1_GetSpeed(void)
{
    /* return speed */
    return gsM1Drive.sSpeed.fltCmd;
}

/***************************************************************************//*!
@brief   Fault Detection function

@param   N/A

@return  N/A
******************************************************************************/
void M1_FaultDetection(void)
{    
    /* clearing actual faults before detecting them again */   
  
    /* clear all faults */
    MC_FAULT_CLEAR_ALL(gsM1Drive.sFaultIdPending);
    
    /* fault: DC-bus over-current */
    if(M1_MCDRV_PWM3PH_FLT_GET(&msM1Pwm3ph))
        MC_FAULT_SET(gsM1Drive.sFaultIdPending, MC_FAULT_I_DCBUS_OVER);
    
    /* fault: DC-bus over-voltage */
    if(gsM1Drive.sFocACIM.fltUDcBusFilt > 
       gsM1Drive.sFaultThresholds.fltUDcBusOver)
        MC_FAULT_SET(gsM1Drive.sFaultIdPending, MC_FAULT_U_DCBUS_OVER);

    /* fault: DC-bus under-voltage */
    if(gsM1Drive.sFocACIM.fltUDcBusFilt < 
       gsM1Drive.sFaultThresholds.fltUDcBusUnder)
        MC_FAULT_SET(gsM1Drive.sFaultIdPending, MC_FAULT_U_DCBUS_UNDER);
     
    /* fault: over-load  */
    if ((MLIB_Abs_FLT(gsM1Drive.sSpeed.fltFilt) < 
         gsM1Drive.sFaultThresholds.fltSpeedOverLoad) &&\
        (MLIB_Abs_FLT(gsM1Drive.sSpeed.sSpeedRampParams.fltState) > 
         gsM1Drive.sFaultThresholds.fltSpeedMin) &&\
        (gsM1Drive.sSpeed.bSpeedPISatFlag))
            MC_FAULT_SET(gsM1Drive.sFaultIdPending, MC_FAULT_LOAD_OVER);              
    
                                
    /* fault: over-speed  */
    if ((MLIB_Abs_FLT(gsM1Drive.sSpeed.fltFilt) > 
         gsM1Drive.sFaultThresholds.fltSpeedOver) &&\
        (MLIB_Abs_FLT(gsM1Drive.sSpeed.fltCmd) > 
         gsM1Drive.sFaultThresholds.fltSpeedMin))
            MC_FAULT_SET(gsM1Drive.sFaultIdPending, MC_FAULT_SPEED_OVER);
    
    /* pass fault to fault ID */
    gsM1Drive.sFaultId |= gsM1Drive.sFaultIdPending; 
}


/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
