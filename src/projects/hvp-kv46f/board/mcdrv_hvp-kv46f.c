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
* @file     mcdrv_hvp-kv46f150m.c
*
* @brief    Motor control peripheral driver main board file
*
* @board    hvp-kv46f150m
*
*******************************************************************************/
/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "mcdrv_hvp-kv46f.h"
#include "fsl_device_registers.h" 
#include "main.h"

/******************************************************************************
| local variable definitions                          
|----------------------------------------------------------------------------*/
/* configuration structure for 3-phase PWM MC driver */
M1_MCDRV_PWM3PH_INIT_T     msM1Pwm3phInit;

/* configuration structure for ADC driver - phase currents, DC-bus voltage 
   and aux meaurement */
M1_MCDRV_ADC_INIT_T        msM1AdcInit;

/******************************************************************************
| global variable definitions                          
|----------------------------------------------------------------------------*/
/* structure for 3-phase PWM MC driver */
M1_MCDRV_PWM3PH_T          msM1Pwm3ph;

/* structure for ADC driver - phase currents, DC-bus voltage and aux
   meaurement */
M1_MCDRV_ADC_T             msM1AdcSensor;

/******************************************************************************
| function implementations                                
|----------------------------------------------------------------------------*/
/******************************************************************************
@brief   Motor control driver main initialization
          - Calls initialization functions of peripherals required for motor
            control fucntionality

@param   N/A

@return  N/A
******************************************************************************/
void MCDRV_Init_M1(void)
{
    /* init XBARA */
    //InitXBARA();
    
    /* six-channel PWM peripheral init  */
    M1_MCDRV_PWM_PERIPH_INIT(); 
}

#if (M1_MCDRV_PWM3PH == MCDRV_PWMA)
void InitPWMA(void)
{
  
    /* enable clock for eFlexPWM modules 0,1 and 2 in SIM module */
    SIM_WR_SCGC4_eFlexPWM0(SIM, TRUE);
    SIM_WR_SCGC4_eFlexPWM1(SIM, TRUE);
    SIM_WR_SCGC4_eFlexPWM2(SIM, TRUE);

    /* full cycle reload */
    PWM_WR_CTRL_FULL(PWMA, 0, TRUE);
    PWM_WR_CTRL_FULL(PWMA, 1, TRUE);
    PWM_WR_CTRL_FULL(PWMA, 2, TRUE);
    
    /* value register initial values, duty cycle 50% */
    PWM_WR_INIT(PWMA, 0, (uint16_t)(-(M1_PWM_MODULO/2)));
    PWM_WR_INIT(PWMA, 1, (uint16_t)(-(M1_PWM_MODULO/2)));
    PWM_WR_INIT(PWMA, 2, (uint16_t)(-(M1_PWM_MODULO/2)));
    
    PWM_WR_VAL0(PWMA, 0, (uint16_t)(0));
    PWM_WR_VAL0(PWMA, 1, (uint16_t)(0));
    PWM_WR_VAL0(PWMA, 2, (uint16_t)(0));
    
    PWM_WR_VAL1(PWMA, 0, (uint16_t)((M1_PWM_MODULO/2)-1));
    PWM_WR_VAL1(PWMA, 1, (uint16_t)((M1_PWM_MODULO/2)-1));
    PWM_WR_VAL1(PWMA, 2, (uint16_t)((M1_PWM_MODULO/2)-1));
    
    PWM_WR_VAL2(PWMA, 0, (uint16_t)(-(M1_PWM_MODULO/4)));
    PWM_WR_VAL2(PWMA, 1, (uint16_t)(-(M1_PWM_MODULO/4)));
    PWM_WR_VAL2(PWMA, 2, (uint16_t)(-(M1_PWM_MODULO/4)));
    
    PWM_WR_VAL3(PWMA, 0, (uint16_t)((M1_PWM_MODULO/4)));
    PWM_WR_VAL3(PWMA, 1, (uint16_t)((M1_PWM_MODULO/4)));
    PWM_WR_VAL3(PWMA, 2, (uint16_t)((M1_PWM_MODULO/4)));
    
    PWM_WR_VAL4(PWMA, 0, (uint16_t)(0));
    PWM_WR_VAL4(PWMA, 1, (uint16_t)(0));
    PWM_WR_VAL4(PWMA, 2, (uint16_t)(0));
    
    PWM_WR_VAL5(PWMA, 0, (uint16_t)(0)); 
    PWM_WR_VAL5(PWMA, 1, (uint16_t)(0));
    PWM_WR_VAL5(PWMA, 2, (uint16_t)(0));
    
    /* PWMA module 0 trigger on VAL4 enabled for ADC synchronization */
    //PWM_WR_TCTRL_OUT_TRIG_EN(PWMA, 0, (1<<4));

    /* recomended value of deadtime for FNB41560 on HVP-MC3PH is 1.5us
       DTCNT0,1 = T_dead * f_fpc = 1.5us * 74MHz = 111 */
    PWM_WR_DTCNT0(PWMA, 0, 370);
    PWM_WR_DTCNT0(PWMA, 1, 370);
    PWM_WR_DTCNT0(PWMA, 2, 370);
    PWM_WR_DTCNT0(PWMA, 3, 370);
    PWM_WR_DTCNT1(PWMA, 0, 370);
    PWM_WR_DTCNT1(PWMA, 1, 370);
    PWM_WR_DTCNT1(PWMA, 2, 370);
    PWM_WR_DTCNT1(PWMA, 3, 370);
      
    /* channels A and B disabled when fault 0 occurs */
    PWM_WR_DISMAP_DIS0A(PWMA, 0, 0, 0x0);
    PWM_WR_DISMAP_DIS0A(PWMA, 1, 0, 0x0);
    PWM_WR_DISMAP_DIS0A(PWMA, 2, 0, 0x0); 
    PWM_WR_DISMAP_DIS0B(PWMA, 0, 0, 0x0);
    PWM_WR_DISMAP_DIS0B(PWMA, 1, 0, 0x0);
    PWM_WR_DISMAP_DIS0B(PWMA, 2, 0, 0x0); 

    /* modules one and two gets clock from module zero */
    PWM_WR_CTRL2_CLK_SEL(PWMA, 1, 0x2);
    PWM_WR_CTRL2_CLK_SEL(PWMA, 2, 0x2);
    
    /* master reload active for modules one and two*/
    PWM_WR_CTRL2_RELOAD_SEL(PWMA, 1, TRUE);
    PWM_WR_CTRL2_RELOAD_SEL(PWMA, 2, TRUE);
    
    /* master sync active for modules one and two*/
    PWM_WR_CTRL2_INIT_SEL(PWMA, 1, 0x2);
    PWM_WR_CTRL2_INIT_SEL(PWMA, 2, 0x2);
    
    /* fault 0 active in high, fault 1 active in low, manual clearing */
    PWM_WR_FCTRL_FLVL(PWMA, 0x1);
    PWM_WR_FCTRL_FAUTO(PWMA, 0x0);

    /* PWMs are re-enabled at PWM full cycle */
    PWM_WR_FSTS_FFULL(PWMA, 0x1); 
     
    /* PWM fault filter - 5 Fast periph. clocks sample rate, 5 agreeing 
       samples to activate */
    PWM_WR_FFILT_FILT_PER(PWMA, 5);
    PWM_WR_FFILT_FILT_CNT(PWMA, 5);
        
    /* enable A&B PWM outputs for submodules one, two and three */
    PWM_WR_OUTEN_PWMA_EN(PWMA, 0x7);
    PWM_WR_OUTEN_PWMB_EN(PWMA, 0x7);
    
    PWM_WR_CTRL_PRSC(PWMA, 0, 3);
    //PWM_WR_CTRL_PRSC(PWMA, 1, 3);
    //PWM_WR_CTRL_PRSC(PWMA, 2, 3);
       
    /* start PWMs (set load OK flags and run) */
    PWM_WR_MCTRL_CLDOK(PWMA, 0x7);
    PWM_WR_MCTRL_LDOK(PWMA, 0x7);
    PWM_WR_MCTRL_RUN(PWMA, 0x7);
    
    /* set ports */
    PORT_WR_PCR_MUX(PORTD, 0, 6);                                               /* HVP-MC3PH phase A top */
    PORT_WR_PCR_MUX(PORTD, 1, 6);                                               /* HVP-MC3PH phase A bottom */
    PORT_WR_PCR_MUX(PORTD, 2, 6);                                               /* HVP-MC3PH phase B top */
    PORT_WR_PCR_MUX(PORTD, 3, 6);                                               /* HVP-MC3PH phase B bottom */
    PORT_WR_PCR_MUX(PORTD, 4, 5);                                               /* HVP-MC3PH phase C top */
    PORT_WR_PCR_MUX(PORTD, 5, 5);                                               /* HVP-MC3PH phase C bottom */
    
    PWM_WR_INTEN_RIE(PWMA, 0 , TRUE);
    /* enable & setup interrupts */
    NVIC_EnableIRQ(PWMA_RELOAD0_IRQn);                                                  /* enable Interrupt */
    NVIC_SetPriority(PWMA_RELOAD0_IRQn, 3);                                             /* set priority to interrupt */
}
#endif /* M1_MCDRV_PWM3PH == MCDRV_PWMA */


/******************************************************************************
@brief   Initialization of the cyclic 12-bit ADC peripheral for current and 
         voltage sensing

@param   N/A

@return  N/A
******************************************************************************/
void InitADC12(void)
{
    /* ADC channel number assignment array to be passeed to MC ADC driver */
    static uint16_t ui16AdcArray[10] =  {M1_ADC0_PH_A, M1_ADC1_PH_A,            /* phase current A */
                                         M1_ADC0_PH_B, M1_ADC1_PH_B,            /* phase current B */
                                         M1_ADC0_PH_C, M1_ADC1_PH_C,            /* phase current C */
                                         ADC0_UDCB, ADC1_UDCB,                  /* DC-bus voltage */
                                         ADC0_AUX,  ADC1_AUX};                  /* auxiliary channel */
                                                                                
    /* enable clock for ADC modules */
    SIM_WR_SCGC5_ADC(SIM, TRUE);
  
    /* triggered parallel mode */
    ADC_WR_CTRL1_SMODE(ADC, 0x5);
    
    /* enable end-of-scan interrupt */
    ADC_WR_CTRL1_EOSIE0(ADC, TRUE);

    /* enable hwardware triggering */
    ADC_WR_CTRL1_SYNC0(ADC, TRUE);
    
    /* start ADC */
    ADC_WR_CTRL1_STOP0(ADC, FALSE);
    
    /* input clock is 24.66MHz (148MHz fast peripheral clock divided by 6), 
       single ended */
    ADC_WR_CTRL2_DIV0(ADC, 0x005);
    
    /* disable ADC */
    ADC_WR_CTRL2_STOP1(ADC, TRUE);

    /* enable samples first two samples on both ADCA and ADCB */
    ADC_WR_SDIS(ADC, 0xFCFC);
        
    /* power-up ADCA and ADCB */
    ADC_WR_PWR_PD0(ADC, FALSE);
    ADC_WR_PWR_PD1(ADC, FALSE);
        
    /* pass initialization structure to ADC MC driver */
    msM1AdcInit.ui16AdcArray = (&ui16AdcArray[0]);
    msM1AdcInit.pui32AdcBase = (ADC_Type *)ADC;
    
    MCDRV_Adc12Init(&msM1AdcSensor, &msM1AdcInit); 
    
    /* assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssign(&msM1AdcSensor);
    //msM1AdcSensor.pf16UDcBus     = &gsM1Drive.sFocACIM.f16UDcBusMeas;
    //msM1AdcSensor.psIABC         = &gsM1Drive.sFocACIM.sIABCMeas;
    //msM1AdcSensor.pui16SVMSector = &gsM1Drive.sFocACIM.ui16SectorSVM;
    //msM1AdcSensor.pui16AuxChan   = &gsM1Drive.f16AdcAuxSample;
    
    /* enable & setup interrupt from ADC */
    NVIC_EnableIRQ(ADCA_IRQn);                                                  /* enable Interrupt */
    NVIC_SetPriority(ADCA_IRQn, 2);                                             /* set priority to interrupt */
} 

/******************************************************************************
@brief   void InitXBARA(void)
          - PWMA0 trigger 0 connected to ADC0 trigger
          - pin 79 (overcurrent) connected to PWMA FAULT0

@param   N/A

@return  N/A
******************************************************************************/
void InitXBARA(void)
{
    /* Enable clock for XBARA */
    SIM_WR_SCGC5_XBARA(SIM, TRUE);
 
#if (M1_MCDRV_PWM3PH == MCDRV_FTM)
    
    /* connect FTM0_INIT trigger to ADC */
    XBARA_WR_SEL6_SEL12(XBARA, 17);
    
    /* connect XBARA_IN7 to FTM0_FLT3 (fixed over-current fault from 
       PTE1/ALT5) */
    PORT_WR_PCR_MUX(PORTE, 1, 5);
    XBARA_WR_SEL24_SEL49(XBARA, 7);

#elif (M1_MCDRV_PWM3PH == MCDRV_PWMA)
    
    /* connect PWMA0_TRG0 trigger to ADC */
    XBARA_WR_SEL6_SEL12(XBARA, 20);
    
    /* connect CMP1_OUT to PWMA_FAULT0 (adjustable over-current fault) */
    XBARA_WR_SEL14_SEL29(XBARA, 13);
    
    /* connect XBARA_IN7 to PWMA_FAULT1 (fixed over-current fault) */
    PORT_WR_PCR_MUX(PORTE, 1, 5);
    XBARA_WR_SEL15_SEL30(XBARA, 7);
    
#endif
    
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/ 