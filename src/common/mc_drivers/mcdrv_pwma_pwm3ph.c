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
* @brief    Motor control peripheral driver for eFlexPWm 3-phase PWM
*
* @periph   Three sub-modules of eFlexPWM 
*
******************************************************************************/

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "mcdrv_pwma_pwm3ph.h"

/******************************************************************************
| defines and macros                                  
|----------------------------------------------------------------------------*/

/******************************************************************************
| typedefs and structures                  
|----------------------------------------------------------------------------*/

/******************************************************************************
| global variable definitions                    
|----------------------------------------------------------------------------*/

/******************************************************************************
| global variable definitions                          
|----------------------------------------------------------------------------*/
static bool_t statusPass;

/******************************************************************************
| function prototypes                                
|----------------------------------------------------------------------------*/

/******************************************************************************
| function implementations                      
|----------------------------------------------------------------------------*/

/******************************************************************************
@brief   bool_t MCDRV_FtmPwm3PhInit(MCDRV_PWMA_PWM3PH_T *this, 
                                    MCDRV_PWMA_PWM3PH_INIT_T *init)
           - Initialization function of 3-phase PWM
           - PWMA structure initialization

@param   this   Pointer to the current object.
         init   Pointer to initialization structure

@return  boot_t
******************************************************************************/
bool_t MCDRV_eFlexPwm3PhInit(MCDRV_PWMA_PWM3PH_T *this, 
                             MCDRV_PWMA_PWM3PH_INIT_T *init)
{
    statusPass = TRUE;
    
    this->pui32PwmBaseAddress = init->pui32PwmBaseAddress;                      /* FTM Base address */

    this->ui16PhASubNum = init->ui16PhASubNum;                                  /* PWMA phase A submodule number */
    this->ui16PhBSubNum = init->ui16PhBSubNum;                                  /* PWMA phase B submodule number */
    this->ui16PhCSubNum = init->ui16PhCSubNum;                                  /* PWMA phase C submodule number */
      
    this->ui16FaultFixNum = init->ui16FaultFixNum;                              /* PWMA fixed-value over-current fault number */
    this->ui16FaultAdjNum = init->ui16FaultAdjNum;                              /* PWMA adjustable over-current fault number */
       
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_FtmPwm3PhSet(MCDRV_PWMA_PWM3PH_T *this)
           - FTM value register updates

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_eFlexPwm3PhSet(MCDRV_PWMA_PWM3PH_T *this)
{
    frac16_t              f16DutyCycle, f16DutyCycleTemp, f16ModuloTemp;
    GMCLIB_3COOR_T_F16    sUABCtemp;  
    
    statusPass = TRUE;
 
    /* pointer to duty cycle structure */
    sUABCtemp = *this->psUABC;
    
    /* get modulo value from module 0 VAL1 register  */ 
    f16ModuloTemp = 
        PWM_RD_VAL1(this->pui32PwmBaseAddress, this->ui16PhASubNum) + 1; 
    
    /* phase A */
    f16DutyCycle = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16A);
    f16DutyCycleTemp = MLIB_Neg_F16(f16DutyCycle);
    PWM_WR_VAL2(this->pui32PwmBaseAddress, 
                this->ui16PhASubNum, 
                f16DutyCycleTemp);
    PWM_WR_VAL3(this->pui32PwmBaseAddress, 
                this->ui16PhASubNum, 
                f16DutyCycle);
    
    /* phase B */
    f16DutyCycle = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16B);
    f16DutyCycleTemp = MLIB_Neg_F16(f16DutyCycle);
    PWM_WR_VAL2(this->pui32PwmBaseAddress, 
                this->ui16PhBSubNum, 
                f16DutyCycleTemp);
    PWM_WR_VAL3(this->pui32PwmBaseAddress, 
                this->ui16PhBSubNum, 
                f16DutyCycle);
    
    /* phase C */
    f16DutyCycle = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16C);           
    f16DutyCycleTemp = MLIB_Neg_F16(f16DutyCycle);
    PWM_WR_VAL2(this->pui32PwmBaseAddress, 
                this->ui16PhCSubNum, 
                f16DutyCycleTemp);
    PWM_WR_VAL3(this->pui32PwmBaseAddress, 
                this->ui16PhCSubNum, 
                f16DutyCycle);
      
    /* set LDOK bits */
    PWM_WR_MCTRL_LDOK(this->pui32PwmBaseAddress, 0x7);

    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_FtmPwm3PhOutEn(MCDRV_PWMA_PWM3PH_T *this)
           - Enables PWM outputs

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_eFlexPwm3PhOutEn(MCDRV_PWMA_PWM3PH_T *this)
{ 
    uint16_t ui16Temp;
    
    statusPass = TRUE;
 
    /* PWM outputs of submodules 0,1 and 2 enabled */
    /* PWM_A output */
    ui16Temp = PWM_RD_OUTEN_PWMA_EN(this->pui32PwmBaseAddress);
    PWM_WR_OUTEN_PWMA_EN(this->pui32PwmBaseAddress, (ui16Temp | 0x7));
    /* PWM_B output */
    ui16Temp = PWM_RD_OUTEN_PWMB_EN(this->pui32PwmBaseAddress);
    PWM_WR_OUTEN_PWMB_EN(this->pui32PwmBaseAddress, (ui16Temp | 0x7));
    
    return(statusPass);
}

/***************************************************************************//*!
@brief   bool_t MCDRV_FtmPwm3PhOutDis(MCDRV_PWMA_PWM3PH_T *this)
           - Disables PWM outputs

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_eFlexPwm3PhOutDis(MCDRV_PWMA_PWM3PH_T *this)
{
    uint16_t ui16Temp;
  
    statusPass = TRUE;
    
    /* PWM outputs of submodules 0,1 and 2 disabled */
    /* PWM_A output */
    ui16Temp = PWM_RD_OUTEN_PWMA_EN(this->pui32PwmBaseAddress);
    PWM_WR_OUTEN_PWMA_EN(this->pui32PwmBaseAddress, (ui16Temp & 0x8));
    /* PWM_B output */
    ui16Temp = PWM_RD_OUTEN_PWMB_EN(this->pui32PwmBaseAddress);
    PWM_WR_OUTEN_PWMB_EN(this->pui32PwmBaseAddress, (ui16Temp & 0x8));
    
    return(statusPass);
}

/***************************************************************************//*!
@brief   bool_t MCDRV_FtmPwm3PhFltGet(MCDRV_PWMA_PWM3PH_T *this)
           - Return actual value of over current flag

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_eFlexPwm3PhFltGet(MCDRV_PWMA_PWM3PH_T *this)
{
    
    /* read over-current flags */
    statusPass = (PWM_RD_FSTS_FFLAG(this->pui32PwmBaseAddress) & 
                 (this->ui16FaultFixNum | this->ui16FaultAdjNum));
     
    /* clear faults flag */
    PWM_WR_FSTS_FFLAG(this->pui32PwmBaseAddress, (this->ui16FaultFixNum | 
                                                  this->ui16FaultAdjNum));
  
    return((statusPass > 0));
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
