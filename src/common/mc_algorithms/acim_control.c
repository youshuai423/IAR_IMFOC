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
* @file         acim_control.c
*
* @brief        Control algorithms for ACIM
* 
*******************************************************************************/
/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "acim_control.h"

/******************************************************************************
| constants                         
|----------------------------------------------------------------------------*/

/******************************************************************************
| macros                          
|----------------------------------------------------------------------------*/

/******************************************************************************
| types                         
|----------------------------------------------------------------------------*/

/******************************************************************************
| functions                         
|----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief   ACIM open loop control (scalar control)

@param   MCS_ACIM_FOC_A1_T *psFocACIM
           -structure of ACIM FOC parameters
           OUT: -> sSC - Generated voltage vector position structure
           OUT: -> sUDQReq - Required stator voltage [V]
         MCS_ACIM_SCALAR_CTRL_A1_T *psScalarACIM
           -structure of ACIM scalar control parameters
           IN: -> sSpeed.sSpeedRampParams - Speed ramp structure 
           IN: -> sSpeed.fltReq - Required speed [rpm]
           IN: -> sCnst.fltSpeed2PosEl - Speed to electrical position 
                                         constant [rad*s]
           IN: -> sCnst.fltVoltRpmFactor - Volt per hertz constant [V/rpm]

@return  N/A
******************************************************************************/
void MCS_ACIMOpenLoopScalarCtrlA1(MCS_ACIM_FOC_A1_T *psFocACIM, 
                                  MCS_SCALAR_A1_T *psScalarACIM)
{
    /* required speed ramp */
    psScalarACIM->fltSpeedScalarCmdRamp = GFLIB_Ramp_FLT(
        psScalarACIM->fltSpeedScalarCmd, &psScalarACIM->sScalarRampParams);
    
    /* frequency generation by integrating required speed */
    psScalarACIM->fltPositionScalar += psScalarACIM->fltScalarIntegGain *
                                       psScalarACIM->fltSpeedScalarCmd;
    
    /* limit angle to -pi to pi range */
    if(psScalarACIM->fltPositionScalar > M1_PI_CNST ) /*2*PI = 6.2831853072*/
        psScalarACIM->fltPositionScalar -= 2*M1_PI_CNST;
    if(psScalarACIM->fltPositionScalar < -M1_PI_CNST ) /*2*PI = 6.2831853072*/
        psScalarACIM->fltPositionScalar += 2*M1_PI_CNST;

    /* calculate required stator voltage */
    psFocACIM->sUDQReq.fltD = 0.0F;
    psFocACIM->sUDQReq.fltQ = psScalarACIM->fltScalarVrpmRatioGain * 
                              psScalarACIM->fltSpeedScalarCmd;
    
    /* check whether q-axis voltage is above minimal */
    if(psFocACIM->sUDQReq.fltQ < psScalarACIM->fltScalarUqMin)
        psFocACIM->sUDQReq.fltQ = psScalarACIM->fltScalarUqMin;
    
    /* calculate sine & cosine for Park's transformation */
    psFocACIM->sSCFOC.fltSin = GFLIB_Sin_FLT(psScalarACIM->fltPositionScalar); 
    psFocACIM->sSCFOC.fltCos = GFLIB_Cos_FLT(psScalarACIM->fltPositionScalar); 

    /* call voltage control to calculate PWM duty cycles */
    MCS_ACIMFocCtrlVoltageA1(psFocACIM);
}

/***************************************************************************//*!
@brief   ACIM open loop (scalar control) with no stator voltage control for 
         motor parameter identification purpose 

@param   MCS_ACIM_FOC_A1_T *psFocACIM
           -structure of ACIM FOC parameters
           OUT: -> sSC - Generated voltage vector position structure
           OUT: -> sUDQReq - Required stator voltage [V]
         MCS_ACIM_SCALAR_CTRL_A1_T *psScalarACIM
           -structure of ACIM scalar control parameters
           IN: -> sSpeed.sSpeedRampParams - Speed ramp structure 
           IN: -> sSpeed.fltReq - Required speed [rpm]
           IN: -> sCnst.fltSpeed2PosEl - Speed to electrical position 
                                         constant [rad*s]
           IN: -> sCnst.fltVoltRpmFactor - Volt per hertz constant [V/rpm]

@return  N/A
******************************************************************************/
void MCS_ACIMOpenLoopScalarCtrlA2(MCS_ACIM_FOC_A1_T *psFocACIM, 
                                  MCS_SCALAR_A1_T *psScalarACIM)
{   
    /* required speed ramp */
    psScalarACIM->fltSpeedScalarCmdRamp = GFLIB_Ramp_FLT(
        psScalarACIM->fltSpeedScalarCmd, &psScalarACIM->sScalarRampParams);
    
    /* frequency generation by integrating required speed */
    psScalarACIM->fltPositionScalar += 2.0F * M1_PI_CNST * M1_SAMPLE_TIME * 
        psScalarACIM->fltSpeedScalarCmd;
    
    /* limit angle to -pi to pi range */
    if(psScalarACIM->fltPositionScalar > M1_PI_CNST ) /* 2*PI = 6.2831853072 */
        psScalarACIM->fltPositionScalar -= 2*M1_PI_CNST;
    if(psScalarACIM->fltPositionScalar < -M1_PI_CNST ) /* 2*PI = 6.2831853072 */
        psScalarACIM->fltPositionScalar += 2*M1_PI_CNST;

    /* calculate sine & cosine for Park's transformation */
    psFocACIM->sSCFOC.fltSin = GFLIB_Sin_FLT(psScalarACIM->fltPositionScalar); 
    psFocACIM->sSCFOC.fltCos = GFLIB_Cos_FLT(psScalarACIM->fltPositionScalar); 

    /* call voltage control to calculate PWM duty cycles */
    MCS_ACIMFocCtrlVoltageA1(psFocACIM);
}

/***************************************************************************//*!
@brief   ACIM field oriented voltage control

@param   MCS_ACIM_FOC_A1_T *psFocACIM
           -warning: no check of required voltage is done
           -structure of ACIM FOC parameters
           IN: -> sSC- Angle of rotor flux vector
           IN: -> sUDQReq - Actual DCBus voltage value
           IN: -> f16UDcBusMeasFilt - Filtered DC bus voltage
           OUT: -> sDutyABC - ABC duty cycles
           OUT: -> ui16SectorSVM - Next step SVM sector       

@return  N/A
******************************************************************************/
void MCS_ACIMFocCtrlVoltageA1(MCS_ACIM_FOC_A1_T *psFocACIM)
{
    /* Clarke's transformation of stator currents */
    GMCLIB_Clark_FLT(&psFocACIM->sIABC, &psFocACIM->sIAlBe);

    /* run rotor flux observer on backgournd */
    psFocACIM->sRFO.acc32RotFulxPos = AMCLIB_RotFluxObsrv_A32ffa_FC(
                                          &psFocACIM->sIAlBe,
                                          &psFocACIM->sUAlBe,
                                           psFocACIM->sRFO.acc32RotFulxPos,
                                          &psFocACIM->sRFO.sCtrl,
                                          &psFocACIM->sRFO.sPsiRAlBe,
                                          &psFocACIM->sRFO.sCtrlDebug);

    /* run MRAS speed estimator on background */
    psFocACIM->sMRAS.acc32RotPos = AMCLIB_MRAS_A32ffa_FC(
                                       &psFocACIM->sIAlBe,
                                       &psFocACIM->sRFO.sPsiRAlBe,
                                        psFocACIM->sMRAS.acc32RotPos,
                                       &psFocACIM->sMRAS.sCtrl,
                                       &psFocACIM->sMRAS.sCtrlDebug);

    /* Park's transformation of stator currents */
    GMCLIB_Park_FLT(&psFocACIM->sIAlBe, 
                    &psFocACIM->sSCFOC, 
                    &psFocACIM->sIDQ);
    
    /* transform required voltage into stator reference frame */
    GMCLIB_ParkInv_FLT(&psFocACIM->sUDQReq, 
                       &psFocACIM->sSCFOC, 
                       &psFocACIM->sUAlBe);

    /* DC-bus voltage ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocACIM->fltUDcBus, 
                                 &psFocACIM->sUAlBe,
                                 &psFocACIM->sUAlBeComp_F16);
    
    /* space vector modulation */
    psFocACIM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocACIM->sUAlBeComp_F16, 
                                                 &psFocACIM->sDutyABC);
}

/***************************************************************************//*!
@brief   ACIM field oriented voltage control including calculation of 
         sine & cosine of rotor flux

@param   MCS_ACIM_FOC_A1_T *psFocACIM
           -warning: no check of required voltage is done
           -structure of ACIM FOC parameters
           IN: -> sSC- Angle of rotor flux vector
           IN: -> sUDQReq - Actual DCBus voltage value
           IN: -> f16UDcBusFilt - Filtered DC bus voltage
           OUT: -> sDutyABC - ABC duty cycles
           OUT: -> ui16SectorSVM - Next step SVM sector

@return  N/A
******************************************************************************/
void MCS_ACIMFocCtrlVoltageA2(MCS_ACIM_FOC_A1_T *psFocACIM)
{
    /* Clarke's transformation of stator currents */
    GMCLIB_Clark_FLT(&psFocACIM->sIABC, &psFocACIM->sIAlBe);

    /* run rotor flux observer on backgournd */
    psFocACIM->sRFO.acc32RotFulxPos = AMCLIB_RotFluxObsrv_A32ffa_FC(
                                          &psFocACIM->sIAlBe,
                                          &psFocACIM->sUAlBe,
                                           psFocACIM->sRFO.acc32RotFulxPos,
                                          &psFocACIM->sRFO.sCtrl,
                                          &psFocACIM->sRFO.sPsiRAlBe,
                                          &psFocACIM->sRFO.sCtrlDebug);

    /* run MRAS speed estimator on background */
    psFocACIM->sMRAS.acc32RotPos = AMCLIB_MRAS_A32ffa_FC(
                                       &psFocACIM->sIAlBe,
                                       &psFocACIM->sRFO.sPsiRAlBe,
                                        psFocACIM->sMRAS.acc32RotPos,
                                       &psFocACIM->sMRAS.sCtrl,
                                       &psFocACIM->sMRAS.sCtrlDebug);

    /* calculate sine & cosine of rotor flux position */
    psFocACIM->sSCFOC.fltSin = GFLIB_Sin_FLTa(psFocACIM->sRFO.acc32RotFulxPos);
    psFocACIM->sSCFOC.fltCos = GFLIB_Cos_FLTa(psFocACIM->sRFO.acc32RotFulxPos);

    /* Park's transformation of stator currents */
    GMCLIB_Park_FLT(&psFocACIM->sIAlBe, 
                    &psFocACIM->sSCFOC, 
                    &psFocACIM->sIDQ);

    /* transform required voltage into stator reference frame */
    GMCLIB_ParkInv_FLT(&psFocACIM->sUDQReq, 
                       &psFocACIM->sSCFOC, 
                       &psFocACIM->sUAlBe);

    /* DC-bus voltage ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocACIM->fltUDcBus, 
                                 &psFocACIM->sUAlBe,
                                 &psFocACIM->sUAlBeComp_F16);
    
    /* space vector modulation */
    psFocACIM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocACIM->sUAlBeComp_F16, 
                                                 &psFocACIM->sDutyABC);
}
/***************************************************************************//*!
@brief   ACIM field oriented current control

@param   MCS_ACIM_FOC_A1_T *psFocACIM
           -warning: no check of required stator current is done
           -structure of ACIM FOC parameters
           IN: -> sSC- Angle of rotor flux vector
           IN: -> sIAlBe - Stator current in stator reference frame
           IN: -> fltDutyCycleLimit - Maximal value of duty cycle
           IN: -> fltUDcBus - DC-bus voltage [V]
           IN/OUT: -> sIdPiParams - D current controller structure [A]
           IN/OUT: -> sIqPiParams - Q current controller structure [A]
           OUT: -> sDutyABC - ABC duty cycles
           OUT: -> ui16SectorSVM - Next step SVM sector

@return  N/A
******************************************************************************/
void MCS_ACIMFocCtrlCurrentA1(MCS_ACIM_FOC_A1_T *psFocACIM)
{
    /* transform stator phase currents into stator reference frame */    
    GMCLIB_Clark_FLT(&(psFocACIM->sIABC), &(psFocACIM->sIAlBe));
    
    /* do not estimate position if in startup*/
    if (!psFocACIM->bSpeedStartup)
    {
        /* rotor flux observer */
        psFocACIM->sRFO.acc32RotFulxPos = 
            AMCLIB_RotFluxObsrv_A32ffa_FC(
                &psFocACIM->sIAlBe,
                &psFocACIM->sUAlBe,
                 psFocACIM->sRFO.acc32RotFulxPos,
                &psFocACIM->sRFO.sCtrl,
                &psFocACIM->sRFO.sPsiRAlBe,
                &psFocACIM->sRFO.sCtrlDebug);
    
        /* MRAS speed estimation */
        psFocACIM->sMRAS.acc32RotPos = AMCLIB_MRAS_A32ffa_FC(
                                           &psFocACIM->sIAlBe,
                                           &psFocACIM->sRFO.sPsiRAlBe,
                                            psFocACIM->sMRAS.acc32RotPos,
                                           &psFocACIM->sMRAS.sCtrl,
                                           &psFocACIM->sMRAS.sCtrlDebug);
    
        /* calculate sine & cosine of rotor flux position */
        psFocACIM->sSCFOC.fltSin = 
            GFLIB_Sin_FLTa(psFocACIM->sRFO.acc32RotFulxPos);
        psFocACIM->sSCFOC.fltCos = 
            GFLIB_Cos_FLTa(psFocACIM->sRFO.acc32RotFulxPos);
    }
    else
    {
        /* force rotor flux position to zero */
        psFocACIM->sSCFOC.fltSin = 0.0F;
        psFocACIM->sSCFOC.fltCos = 1.0F;
    }
    
    /* transform stator currents in stator ref. frame into rotating rotor 
       flux ref. frame */
    GMCLIB_Park_FLT(&psFocACIM->sIAlBe, 
                    &psFocACIM->sSCFOC, 
                    &psFocACIM->sIDQ);
    
    /* calculate D-axis voltage limitation */      
    psFocACIM->sIdPiParamsGFLIB.fltLowerLim = 
        -psFocACIM->fltDutyCycleLimit*psFocACIM->fltUDcBus;
    psFocACIM->sIdPiParamsGFLIB.fltUpperLim =  
        psFocACIM->fltDutyCycleLimit*psFocACIM->fltUDcBus;
    
    /* calculate d-axis current control error */
    psFocACIM->sIDQError.fltD = 
        (psFocACIM->sIDQReq.fltD - psFocACIM->sIDQ.fltD);
    
    /* calculate d-axis required volatge */
    psFocACIM->sUDQReq.fltD = GFLIB_CtrlPIpAW_FLT(
                                   psFocACIM->sIDQError.fltD, 
                                  &psFocACIM->bIdPISatFlag, 
                                  &psFocACIM->sIdPiParamsGFLIB);
    
    /* calculate q-axis voltage limitation */
    psFocACIM->sIqPiParamsGFLIB.fltUpperLim = GFLIB_Sqrt_FLT(
        psFocACIM->sIdPiParamsGFLIB.fltUpperLim *
        psFocACIM->sIdPiParamsGFLIB.fltUpperLim -
        psFocACIM->sUDQReq.fltD * psFocACIM->sUDQReq.fltD); 
    psFocACIM->sIqPiParamsGFLIB.fltLowerLim = 
        -psFocACIM->sIqPiParamsGFLIB.fltUpperLim;
      
    /* calculate q-axis current control error */
    psFocACIM->sIDQError.fltQ = 
        (psFocACIM->sIDQReq.fltQ - psFocACIM->sIDQ.fltQ);
    
    /* calculate q-axis required volatge */
    psFocACIM->sUDQReq.fltQ  = GFLIB_CtrlPIpAW_FLT(
                                    psFocACIM->sIDQError.fltQ,
                                   &psFocACIM->bIqPISatFlag, 
                                   &psFocACIM->sIqPiParamsGFLIB);
        
    
    /* transform required voltage into stator reference frame */
    GMCLIB_ParkInv_FLT(&psFocACIM->sUDQReq, 
                       &psFocACIM->sSCFOC, 
                       &psFocACIM->sUAlBe);

    /* DC-bus voltage ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocACIM->fltUDcBus, 
                                 &psFocACIM->sUAlBe,
                                 &psFocACIM->sUAlBeComp_F16);
    
    /* space vector modulation */
    psFocACIM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocACIM->sUAlBeComp_F16, 
                                                 &psFocACIM->sDutyABC);
}

/***************************************************************************//*!
@brief   ACIM foc speed control

@param   MCS_ACIM_FOC_A1_T *psFocACIM
           -structure of ACIM FOC parameters
           IN: -> bIdPISatFlag - Limit flags d-axis current PI controller
           IN: -> bIqPISatFlag - Limit flags q-axis current PI controller
           IN: -> sSpeed.fltCmd - Required rotor speed
           IN: -> sSpeed.fltFilt - Filtered rotor speed
           IN: -> sSpeedRampParams - Speed ramp structure
           IN: -> sSpeedPiParamsGFLIB - Speed PI controller parameters
           OUT: -> sIDQReq.fltQ - Required q-axis current value
           OUT: -> bSpeedPISatFlag - Speed controller saturation flag

@return  N/A
******************************************************************************/
void MCS_ACIMSpeedCtrlA1(MCS_ACIM_FOC_A1_T *psFocACIM, MCS_SPEED_A1_T *psSpeed)
{ 
    /* Ramp speed cmd */
    psSpeed->fltRamp =  GFLIB_Ramp_FLT(psSpeed->fltCmd, 
                                       &psSpeed->sSpeedRampParams);
    
    /* Limit speed controller integrator in case the speed cannot be reached */
    psSpeed->bSpeedPISatFlag = (psFocACIM->sIqPiParamsGFLIB.bLimFlag ||
                                psFocACIM->sIdPiParamsGFLIB.bLimFlag) && 
                               (MLIB_Abs_FLT(psSpeed->fltRamp) > 
                                MLIB_Abs_FLT(psSpeed->fltFilt));
      
    /* Calculate required Q-axis current */
    psSpeed->fltErrorSpeed = psSpeed->fltRamp - psSpeed->fltFilt;
 
    /* Speed controller */
    psFocACIM->sIDQReq.fltQ = GFLIB_CtrlPIpAW_FLT(
                                  psSpeed->fltErrorSpeed,
                                  &psSpeed->bSpeedPISatFlag,
                                  &psSpeed->sSpeedPiParamsGFLIB);   
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/