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
* @file     app_init.h
*
* @brief    Application initialization of ports, LEDs and UART for FreeMASTER
* 
* @board    hvp-kv46f150m
*
******************************************************************************/
#ifndef _APP_INIT_H_
#define _APP_INIT_H_

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "fsl_device_registers.h"
#include "mlib_types.h"
#include "mcdrv_hvp-kv46f.h"

/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| global macros                          
|----------------------------------------------------------------------------*/

/* FreeMASTER settings */
/* select UART0 or UART1 based on user platform (hvp-kv46f150m -> UART1)*/
#define FMSTR_UART_PORT              UART1  
#define FMSTR_UART_IRQ               UART1_RX_TX_IRQn  

/* baud rate */
#define FMSTR_UART_BAUD              (19200)     

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT()        (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT()         (SysTick->LOAD - SysTick->VAL)

/* led control on hvp-kv46f150m */
#define LED1_ON                      (GPIO_WR_PSOR(PTB, 1<<19))
#define LED1_OFF                     (GPIO_WR_PCOR(PTB, 1<<19))
#define LED2_ON                      (GPIO_WR_PSOR(PTB, 1<<2))
#define LED2_OFF                     (GPIO_WR_PCOR(PTB, 1<<2))

/* debug pin on MB_TP22 (PTB10/ALT1) */
#define DEBUG_MBTP22_HIGH            (GPIO_WR_PSOR(PTB, 1<<10))
#define DEBUG_MBTP22_LOW             (GPIO_WR_PCOR(PTB, 1<<10))

/* FNB41560 internal thermistor fourth-order approximation coeffs */
#define IPM_TEMP_APROX_P1            (-2.6028e-016F)
#define IPM_TEMP_APROX_P2            (2.4469e-011F)
#define IPM_TEMP_APROX_P3            (-7.4962e-007F)
#define IPM_TEMP_APROX_P4            (0.012514F)
#define IPM_TEMP_APROX_P5            (-19.265F)


/******************************************************************************
| global functions prototypes                        
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
    
void InitPorts(void);
void FreeMASTER_Init(void);
void InitUART(uint32_t u32UClockSpeedinHz, uint32_t u32BaundRate);
void SysTickInit(void);
void Init_PIT(void);
float_t M1_GetIPMTemperature(M1_MCDRV_ADC_T *sADCStruc);

#ifdef __cplusplus
}
#endif

#endif /* _APP_INIT_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/ 
