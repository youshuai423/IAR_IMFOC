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
* @file     app_init.c
*
* @brief    Application initialization of ports, LEDs and UART for FreeMASTER
* 
* @board    hvp-kv46f150m
*
******************************************************************************/
/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "freemaster.h"
#include "app_init.h"

/******************************************************************************
| local variable definitions                          
|----------------------------------------------------------------------------*/

/******************************************************************************
| global variable definitions                          
|----------------------------------------------------------------------------*/

/******************************************************************************
@brief   Initialization of ports

@param   N/A

@return  N/A
******************************************************************************/
void InitPorts(void)
{
    /* enable clock to all PORTs */
    SIM_WR_SCGC5_PORTA(SIM, TRUE);                                              /* PTA clock enabled */
    SIM_WR_SCGC5_PORTB(SIM, TRUE);                                              /* PTB clock enabled */
    SIM_WR_SCGC5_PORTC(SIM, TRUE);                                              /* PTC clock enabled */
    SIM_WR_SCGC5_PORTD(SIM, TRUE);                                              /* PTD clock enabled */
    SIM_WR_SCGC5_PORTE(SIM, TRUE);                                              /* PTE clock enabled */
    
    /* user led 1 on PTB19/ALT1 */
    PORT_WR_PCR_MUX(PORTB, 19, 1);                                              
    GPIO_SET_PDDR(PTB, 1<<19);
    LED1_OFF;
    
    /* user led 2 on PTA5/ALT1 */
    PORT_WR_PCR_MUX(PORTB, 2, 1);                                               
    GPIO_SET_PDDR(PTB, 1<<2);
    LED2_OFF;
    
    /* debug pin on MB_TP22 (PTB10/ALT1) */
    PORT_WR_PCR_MUX(PORTB, 10, 1);                                               
    GPIO_SET_PDDR(PTB, 1<<10);
}

/******************************************************************************
@brief   UARt initialization

@param   ui32UClockSpeedinHz - core clock frequency in Hz
         ui32BaudRate - required baud rate

@return  N/A
******************************************************************************/
void InitUART(uint32_t ui32UClockSpeedinHz, uint32_t ui32BaudRate)
{
    if(FMSTR_UART_PORT == UART1)
    {
        /* enable the clock to the selected UART */  
        SIM_WR_SCGC4_UART1(SIM, TRUE);
      
        /* calculate baud settings - UART1 is fed from Fast Peripheral Clock */ 
        ui32BaudRate = (uint16_t)((ui32UClockSpeedinHz)/(ui32BaudRate * 16 * 
            (SIM_RD_CLKDIV1_OUTDIV2(SIM) + 1)));
      
        /* enable the UART1_TXD function on PTC3/ALT3 */
        PORT_WR_PCR_MUX(PORTC, 4, 3);  
        
        /* enable the UART1_RXD function on PTC4/ALT3 */
        PORT_WR_PCR_MUX(PORTC, 3, 3);    
    }
    else if(FMSTR_UART_PORT == UART0)
    {
        /* enable the clock to the selected UART */  
        SIM_WR_SCGC4_UART0(SIM, TRUE);     

        /* calculate baud settings - UART0 is fed from Fast Peripheral Clock*/
        ui32BaudRate = (uint16_t)((ui32UClockSpeedinHz)/(ui32BaudRate * 16 * 
            (SIM_RD_CLKDIV1_OUTDIV2(SIM) + 1)));

        /* enable the UART0_TXD function on PTD7/ALT3 
          (alternatively PTE20/ALT4) */
        PORT_WR_PCR_MUX(PORTD, 7, 3);  
        
        /* enable the UART0_RXD function on PTD6/ALT3 
          (alternatively PTE21/ALT4) */
        PORT_WR_PCR_MUX(PORTD, 6, 3);    
    }
  
    /* make sure that the transmitter and reciever are disabled while we change 
       settings */
    UART_WR_C2_TE(FMSTR_UART_PORT, FALSE);                                      /* disable transmitter */
    UART_WR_C2_RE(FMSTR_UART_PORT, FALSE);                                      /* disable reciever */
   
    /* configure the UART for 8-bit mode, no parity */
    UART_WR_C1(FMSTR_UART_PORT, 0x00);                                          /* we need all default settings, so entire register is cleared */
    
    /* set calculated values to BDx registers */
    UART_WR_BDH_SBR(FMSTR_UART_PORT, ((ui32BaudRate & 0x1F00) >> 8)); 
    UART_WR_BDL(FMSTR_UART_PORT, (uint8_t)(ui32BaudRate));
    
    /* enable receiver and transmitter */
    UART_WR_C2_TE(FMSTR_UART_PORT, TRUE);                                       /* enable transmitter */
    UART_WR_C2_RE(FMSTR_UART_PORT, TRUE);                                       /* enable reciever */

    /* enable UART interrupt */
    NVIC_EnableIRQ(FMSTR_UART_IRQ);
}

/******************************************************************************
@brief   FreeMASTER communication initialization
          - Initialize FreeMASTER resources

@param   N/A

@return  N/A
******************************************************************************/
void FreeMASTER_Init(void)
{
    /* FMSTR UART Init */
    InitUART(DEFAULT_SYSTEM_CLOCK, FMSTR_UART_BAUD);

    /* initialize FMSTR stack */
    FMSTR_Init();
}

/******************************************************************************
@brief   Initialization of Systick timer
          - CM4 SYSTICK enable
          - maximum measurable period [sec]= 2^24 / CPU_clock  

@param   N/A

@return  N/A
******************************************************************************/
void SysTickInit(void)
{
    /* initialize SysTick core timer to run free */
    /* set period to maximum value 2^24 */
    SysTick->LOAD = 0xFFFFFF;
    
    /* clock source - System Clock */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    
    /* start SysTick timer */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/******************************************************************************
@brief   Transform raw FNB41560 IPM temperature measurement to Celsius degrees
         -fourth degree polynomial approximation is used 

@param   ADC MC driver structure

@return  float_t
******************************************************************************/
float_t M1_GetIPMTemperature(M1_MCDRV_ADC_T *sADCStruc)
{
    float_t fltRawData, fltTemperature;
    
    /* convert data to float */
    fltRawData = (float_t)(*sADCStruc->pui16AuxChan);
    
    /* get temperature in celsius degrees */
    fltTemperature =  ((((IPM_TEMP_APROX_P1*fltRawData) + 
                          IPM_TEMP_APROX_P2)*fltRawData + 
                          IPM_TEMP_APROX_P3)*fltRawData + 
                          IPM_TEMP_APROX_P4)*fltRawData + 
                          IPM_TEMP_APROX_P5;

    return fltTemperature; 
}

void Init_PIT()
{
  SIM_WR_SCGC6_PIT(SIM, TRUE);
  PIT_WR_MCR_MDIS(PIT, 0);
  PIT_WR_MCR_FRZ(PIT, 1);
  PIT_WR_LDVAL(PIT, 0, 24700);
  PIT_WR_TCTRL_TIE(PIT, 0, 1);
  PIT_WR_TCTRL_TEN(PIT, 0, 1);
  /* enable & setup interrupts */
  NVIC_EnableIRQ(PIT0_IRQn);                                                  /* enable Interrupt */
  NVIC_SetPriority(PIT0_IRQn, 2);                                             /* set priority to interrupt */
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

