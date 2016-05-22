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
* @file     mcdrv_hvp-kv46f.h
*
* @brief    Motor control peripheral driver header file
*
* @board    hvp-kv46f150m
*
******************************************************************************/
#ifndef _MCDRV_HVP_KV46F_H_
#define _MCDRV_HVP_KV46F_H_

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "fsl_device_registers.h"
#include "mcdrv_pwma_pwm3ph.h"
#include "mcdrv_adc_adc12.h"

/******************************************************************************
| defines and macros                          
|----------------------------------------------------------------------------*/
/* define motor 1 ADC periphery (ADC12)  */
#define M1_MCDRV_ADC            MCDRV_ADC12

/* define motor 1 3-ph PWM peiphery (FTM or PWMA) */
#define M1_MCDRV_PWM3PH         MCDRV_PWMA

/* motor 1 PWM modulo
   M1_PWM_MODULO = f_FastPerClk / f_PWM = 74MHz / 10kHz = 7400 */
//#define M1_PWM_MODULO           (7400) 
#define M1_PWM_MODULO           (9250) 
 
/* assignment of PWM channels to motor phases 
   0 - PWM channels 0&1
   1 - PWM channels 2&3
   2 - PWM channels 4&5 */
#define M1_PWM_PAIR_PHA  	(0)
#define M1_PWM_PAIR_PHB  	(1)
#define M1_PWM_PAIR_PHC  	(2)

/* adjustable DC-bus over-current protection threshold
   can be set in range 250~7750 mA with 250 mA resolution*/
#define OVER_CURRENT_THRESHOLD  (7750)

/* predefined values of ADC channels */                   
#define ADC_NO_CHAN             (31)                                            /* default value for unassigned channel */                          

/* Configuration table of ADC channels according to the input pin signals:
Valid for Kinetis KV46 HVP board (HVP-KV46F150M) together with HVP-MC3PH

Proper ADC channel assignment needs to follow these rules:
  - at least one phase current must be assigned to both ADC modules
  - two other phase current channels must be assigned to different ADC 
    modules
  - DC-bus voltage and auxiliary channels must be assigned to differrent ADC 
    modules

 Quantity     | ADC module 0                  | ADC module 1
--------------------------------------------------------------------------
 I_phA        | ADCA_CH2 (pin18)              | -
 I_phB        | -                             | ADCB_CH2 (PTB0) 
 I_phC        | ADCA_CH3 (pin19)              | ADCB_CH3 (PTB1)
 U_dcbus      | ADCA_CH1 (PTE17)              | -
 Aux(IPM tmp) | -                             | ADCB_CH1 (PTE19) 
*/

/* phase current A assingned to ADC0 and ADC1 only */
#define M1_ADC0_PH_A            (2)
#define M1_ADC1_PH_A            ADC_NO_CHAN
/* phase current B assingned to ADC1 only */
#define M1_ADC0_PH_B            ADC_NO_CHAN
#define M1_ADC1_PH_B            (2)
/* phase current C assingned to ADC0 only */
#define M1_ADC0_PH_C            (3)
#define M1_ADC1_PH_C            (3)
/* DC-bus voltage is assigned to ADC0 only */
#define ADC0_UDCB               (1)
#define ADC1_UDCB               ADC_NO_CHAN
/* Aux channel is assigned to ADC1 only */
#define ADC0_AUX                ADC_NO_CHAN
#define ADC1_AUX                (1)

/* MC driver macro definition and check - do not change this part */
/* define motor 1 ADC control functions */
#ifdef M1_MCDRV_ADC
#if (M1_MCDRV_ADC == MCDRV_ADC12)
        #define M1_MCDRV_ADC_T                     MCDRV_ADC12_T  
        #define M1_MCDRV_ADC_INIT_T                MCDRV_ADC12_INIT_T
        #define M1_MCDRV_ADC_PERIPH_INIT()         InitADC12()
        #define M1_MCDRV_CURR_3PH_GET(par)         MCDRV_Curr3Ph2ShGet(par);
        #define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par) MCDRV_Curr3Ph2ShChanAssign(par);  
        #define M1_MCDRV_CURR_3PH_CALIB_INIT(par)  MCDRV_Curr3Ph2ShCalibInit(par);
        #define M1_MCDRV_CURR_3PH_CALIB(par)       MCDRV_Curr3Ph2ShCalib(par);
        #define M1_MCDRV_CURR_3PH_CALIB_SET(par)   MCDRV_Curr3Ph2ShCalibSet(par);
        #define M1_MCDRV_DCBUS_VOLTAGE_GET(par)    MCDRV_VoltDcBusGet(par);
        #define M1_MCDRV_AUX_GET(par)              MCDRV_AuxValGet(par);
    #else
        #error MCDRV: Unsupported ADC periphery!
    #endif 
#else
    #error MCDRV: ADC periphery was not specified!       
#endif 

/* define motor 1 3-ph PWM control functions */
#ifdef M1_MCDRV_PWM3PH
#if (M1_MCDRV_PWM3PH == MCDRV_FTM)
        #define M1_MCDRV_PWM3PH_T                  MCDRV_FTM_PWM3PH_T
        #define M1_MCDRV_PWM3PH_INIT_T             MCDRV_FTM_PWM3PH_INIT_T
        #define M1_MCDRV_PWM_PERIPH_INIT()         InitFTM0()
        #define M1_MCDRV_PWM3PH_SET(par)           MCDRV_FtmPwm3PhSet(par)
        #define M1_MCDRV_PWM3PH_EN(par)            MCDRV_FtmPwm3PhOutEn(par)
        #define M1_MCDRV_PWM3PH_DIS(par)           MCDRV_FtmPwm3PhOutDis(par)
        #define M1_MCDRV_PWM3PH_FLT_GET(par)       MCDRV_FtmPwm3PhFltGet(par)
#elif (M1_MCDRV_PWM3PH == MCDRV_PWMA)
        #define M1_MCDRV_PWM3PH_T                  MCDRV_PWMA_PWM3PH_T
        #define M1_MCDRV_PWM3PH_INIT_T             MCDRV_PWMA_PWM3PH_INIT_T
        #define M1_MCDRV_PWM_PERIPH_INIT()         InitPWMA()
        #define M1_MCDRV_PWM3PH_SET(par)           MCDRV_eFlexPwm3PhSet(par)
        #define M1_MCDRV_PWM3PH_EN(par)            MCDRV_eFlexPwm3PhOutEn(par)
        #define M1_MCDRV_PWM3PH_DIS(par)           MCDRV_eFlexPwm3PhOutDis(par)
        #define M1_MCDRV_PWM3PH_FLT_GET(par)       MCDRV_eFlexPwm3PhFltGet(par)
    #else
        #error MCDRV: Unsupported PWM periphery!
    #endif
#else
    #error MCDRV: PWM periphery was not specified!   
#endif

/* check whether ADC channel assigment is correct */
#if !(((M1_ADC0_PH_A != MCDRV_CHAN_OFF) && (M1_ADC1_PH_A != MCDRV_CHAN_OFF)) ||\
      ((M1_ADC0_PH_B != MCDRV_CHAN_OFF) && (M1_ADC1_PH_B != MCDRV_CHAN_OFF)) ||\
      ((M1_ADC0_PH_C != MCDRV_CHAN_OFF) && (M1_ADC1_PH_C != MCDRV_CHAN_OFF)))
    #error MCDRV: None of phase current is assigned to both ADCs!
#endif

#if (ADC0_UDCB != MCDRV_CHAN_OFF) && (ADC0_AUX != MCDRV_CHAN_OFF) ||\
    (ADC1_UDCB != MCDRV_CHAN_OFF) && (ADC1_AUX != MCDRV_CHAN_OFF)
     #error MCDRV: The DC-bus voltage and Aux cannot be measured at the same ADC!
#endif

/* check over-current protection settings */
#if (OVER_CURRENT_THRESHOLD >= 250) && (OVER_CURRENT_THRESHOLD <= 7750)
    #define OVER_CURRENT_THRESHOLD_INT (32 + ((OVER_CURRENT_THRESHOLD * 4)/1000)) 
#else
    #define OVER_CURRENT_THRESHOLD_INT (32) 
    #error MCDRV: Incorrect setting of over-current protection range! 
#endif      
      
      
/******************************************************************************
| local function prototypes                          
|----------------------------------------------------------------------------*/
void InitADC12(void);
void InitXBARA(void);
void InitPWMA(void);


      
/******************************************************************************
| global variable definitions                          
|----------------------------------------------------------------------------*/
extern M1_MCDRV_PWM3PH_T     msM1Pwm3ph;
extern M1_MCDRV_ADC_T        msM1AdcSensor;

/******************************************************************************
* exported function prototypes
*******************************************************************************/   
#ifdef __cplusplus
extern "C" {
#endif 
  
void MCDRV_Init_M1(void);

#ifdef __cplusplus
}
#endif
#endif /* _MCDRV_HVP_KV46F_H_ */ 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/