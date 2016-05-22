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
* @file     mcdrv_ftm_pwm3ph.h
*
* @brief    Motor control peripheral driver for flexTimer 3-phase PWM
*
* @periph   8-channel FTM 
*
******************************************************************************/

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "mcdrv_ftm_pwm3ph.h"

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
@brief   bool_t MCDRV_FtmPwm3PhInit(MCDRV_FTM_PWM3PH_T *this, 
                                    MCDRV_FTM_PWM3PH_INIT_T *init)
           - initialization function of 3-phase PWM
           - FTM structure initialization

@param   this   Pointer to the current object.
         init   Pointer to initialization structure

@return  boot_t
******************************************************************************/
bool_t MCDRV_FtmPwm3PhInit(MCDRV_FTM_PWM3PH_T *this, 
                           MCDRV_FTM_PWM3PH_INIT_T *init)
{
    statusPass = TRUE;

    this->pui32PwmBase = init->pui32PwmBase;                                    /* FTM Base address */
    
    this->ui16ChanTopPhA    = 0 + init->ui16ChanPairNumPhA;                     /* FTM channel number for Phase A top */
    this->ui16ChanBottomPhA = 1 + init->ui16ChanPairNumPhA;                     /* FTM channel number for Phase A bottom */
    this->ui16ChanTopPhB    = 0 + init->ui16ChanPairNumPhB;                     /* FTM channel number for Phase B top */
    this->ui16ChanBottomPhB = 1 + init->ui16ChanPairNumPhB;                     /* FTM channel number for Phase B bottom */
    this->ui16ChanTopPhC    = 0 + init->ui16ChanPairNumPhC;                     /* FTM channel number for Phase C top */
    this->ui16ChanBottomPhC = 1 + init->ui16ChanPairNumPhC;                     /* FTM channel number for Phase C bottom */
    
    this->ui16FaultFixNum = init->ui16FaultFixNum;                              /* FTM fixed-value over-current fault number */
    this->ui16FaultAdjNum = init->ui16FaultAdjNum;                              /* FTM adjustable over-current fault number */
    
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_FtmPwm3PhSet(MCDRV_FTM_PWM3PH_T *this)
           - FTM value register updates

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_FtmPwm3PhSet(MCDRV_FTM_PWM3PH_T *this)
{
    frac16_t                        f16DutyCycle;
    GMCLIB_3COOR_T_F16              sUABCtemp;   
    statusPass = TRUE;
    
    /* pointer to duty cycle structure */
    sUABCtemp = *this->psUABC;
    
    /* phase A */
    f16DutyCycle = MLIB_Mul_F16(FTM_RD_MOD_MOD(this->pui32PwmBase),sUABCtemp.f16A);
    FTM_WR_CnV_VAL(this->pui32PwmBase, this->ui16ChanTopPhA, MLIB_Neg_F16(f16DutyCycle)); 
    FTM_WR_CnV_VAL(this->pui32PwmBase, this->ui16ChanBottomPhA, f16DutyCycle); 
    
    /* phase B */
    f16DutyCycle = MLIB_Mul_F16(FTM_RD_MOD_MOD(this->pui32PwmBase),sUABCtemp.f16B);
    FTM_WR_CnV_VAL(this->pui32PwmBase, this->ui16ChanTopPhB, MLIB_Neg_F16(f16DutyCycle)); 
    FTM_WR_CnV_VAL(this->pui32PwmBase, this->ui16ChanBottomPhB, f16DutyCycle); 
    
    /* phase C */
    f16DutyCycle = MLIB_Mul_F16(FTM_RD_MOD_MOD(this->pui32PwmBase),sUABCtemp.f16C);           
    FTM_WR_CnV_VAL(this->pui32PwmBase, this->ui16ChanTopPhC, MLIB_Neg_F16(f16DutyCycle)); 
    FTM_WR_CnV_VAL(this->pui32PwmBase, this->ui16ChanBottomPhC, f16DutyCycle);    
    
    /*
     * Set LDOK bit in FTm PWMLOAD register
     */
    FTM_SET_PWMLOAD(this->pui32PwmBase, 0x0200); 
    
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_FtmPwm3PhOutEn(MCDRV_FTM_PWM3PH_T *this)
           - enables PWM outputs

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_FtmPwm3PhOutEn(MCDRV_FTM_PWM3PH_T *this)
{
    statusPass = TRUE;
    
    /* FlexTIMER
     * OUTMASK register = mcPWM_BASE + $0x60
     *
     * Any write to the OUTMASK register, stores the value in its write buffer. The register is
     * updated with the value of its write buffer according to PWM synchronization.
     *
     * CHnOM = 0 - Channel output is not masked. It continues to operate normally.
     * CHnOM = 1 - Channel output is masked. It is forced to its inactive state.
     * |----------------------------------------------------------------------------|
     * |bits:   |   31..8   |  7   |   6  |   5  |  4   |   3  |  2   |  1   |  0   |     
     * |Meaning:| RESERVED  |CH7OM |CH6OM |CH5OM |CH4OM |CH3OM |CH2OM |CH1OM |CH0OM |
     * |----------------------------------------------------------------------------|
             * |Value:  |     0     |      |      |      |      |      |      |      |      |
     * |----------------------------------------------------------------------------|
     */
    
    FTM_WR_OUTMASK(this->pui32PwmBase, 0x00);                                   /* PWM outputs enabled in channels 0..5 */
    
    return(statusPass);
}

/***************************************************************************//*!
@brief   bool_t MCDRV_FtmPwm3PhOutDis(MCDRV_FTM_PWM3PH_T * this)
           - disables PWM outputs

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_FtmPwm3PhOutDis(MCDRV_FTM_PWM3PH_T *this)
{
    statusPass = TRUE;
    
    /* FlexTIMER
     * OUTMASK register = mcPWM_BASE + $0x60
     *
     * Any write to the OUTMASK register, stores the value in its write buffer. The register is
     * updated with the value of its write buffer according to PWM synchronization.
     *
     * CHnOM = 0 - Channel output is not masked. It continues to operate normally.
     * CHnOM = 1 - Channel output is masked. It is forced to its inactive state.
     * |----------------------------------------------------------------------------|
     * |bits:   |   31..8   |  7   |   6  |   5  |  4   |   3  |  2   |  1   |  0   |         
     * |Meaning:| RESERVED  |CH7OM |CH6OM |CH5OM |CH4OM |CH3OM |CH2OM |CH1OM |CH0OM |
     * |----------------------------------------------------------------------------|
     * |Value:  |     0     |      |      |      |      |      |      |      |      |
     * |----------------------------------------------------------------------------|
     */
                                          
    FTM_WR_OUTMASK(this->pui32PwmBase, 0x3F);                                   /* PWM outputs disabled in channels 0..5 */
    
    return(statusPass);
}

/***************************************************************************//*!
@brief   bool_t MCDRV_FtmPwm3PhFltGet(MCDRV_FTM_PWM3PH_T * this)
           - return actual value of over current flag

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_FtmPwm3PhFltGet(MCDRV_FTM_PWM3PH_T * this)
{
    /* Read over-current flag */
    statusPass = FTM_RD_FMS_FAULTF(this->pui32PwmBase);
       
    /* Clear fault flags */
    FTM_WR_FMS_FAULTF(this->pui32PwmBase, 0);
    
    return((statusPass > 0));
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
