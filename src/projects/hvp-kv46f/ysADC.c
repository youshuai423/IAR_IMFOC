/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "ysADC.h"
#include "fsl_device_registers.h" 

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
void Init_ADC()
{                                                                                
    /* enable clock for ADC modules */
    SIM_WR_SCGC5_ADC(SIM, 1);
  
    /* loop parallel mode */
    ADC_WR_CTRL1_SMODE(ADC, 0x3);
    
    /* enable end-of-scan interrupt */
    ADC_WR_CTRL1_EOSIE0(ADC, 1);

    /* enable hwardware triggering */
    ADC_WR_CTRL1_SYNC0(ADC, 1);
    
    /* start ADC */
    ADC_WR_CTRL1_STOP0(ADC, 0);
    
    /* input clock is 24.66MHz (148MHz fast peripheral clock divided by 6), 
       single ended */
    ADC_WR_CTRL2_DIV0(ADC, 0x005);
    
    /* parallel scans done simultaneously */
    ADC_WR_CTRL2_SIMULT(ADC, 1);
    
    /* disable ADC */
    //ADC_WR_CTRL2_STOP1(ADC, TRUE);  // ?????????????

    /* enable samples first two samples on both ADCA and ADCB */
    //ADC_WR_SDIS(ADC, 0xFCFC);
    ADC_WR_SDIS(ADC, 0x0000);
        
    /* power-up ADCA and ADCB */
    ADC_WR_PWR_PD0(ADC, 0);
    ADC_WR_PWR_PD1(ADC, 0);
    
    /* enable & setup interrupt from ADC */
   // NVIC_EnableIRQ(ADCA_IRQn);                                                  /* enable Interrupt */
   // NVIC_SetPriority(ADCA_IRQn, 4);                                             /* set priority to interrupt */
}
