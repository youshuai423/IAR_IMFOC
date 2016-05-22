/******************************************************************************
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
* @file     mcdrv_adc_adc16.c
*
* @brief    3-phase current and voltage sensing module
* 
* @board    -
*
*******************************************************************************/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/
#include "mcdrv_adc_adc16.h"

/******************************************************************************
| defines and macros                                      (scope: module-local)
|----------------------------------------------------------------------------*/

/******************************************************************************
| typedefs and structures                                 (scope: module-local)
|----------------------------------------------------------------------------*/

/******************************************************************************
| local variable definitions                          (scope: module-exported)
|----------------------------------------------------------------------------*/
static bool_t statusPass;

/******************************************************************************
| global variable definitions                             (scope: module-local)
|----------------------------------------------------------------------------*/

/******************************************************************************
| function prototypes                                     (scope: module-local)
|----------------------------------------------------------------------------*/

/******************************************************************************
| function implementations                                (scope: module-local)
|----------------------------------------------------------------------------*/
/******************************************************************************
@brief   bool_t MCDRV_Adc16Init(MCDRV_ADC16_T * this, MCDRV_ADC16_INIT_T * init)
           - Initializes ADC driver to measure 3 currents, dcb voltage and
             auxiliary samples

@param   this   Pointer to the current object.
         init   Pointer to initialization structure

@return  boot_t
******************************************************************************/
bool_t MCDRV_Adc16Init(MCDRV_ADC16_T * this, MCDRV_ADC16_INIT_T * init)
{
    statusPass = TRUE;
    uint16_t ui16CurrSelector = 0;
    uint16_t ui16Temp; 
    
    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;

    /* Check if there are one pair of assigned channels */
    if((init->ui16AdcArray[MCDRV_ADC0_IA] != MCDRV_CHAN_OFF)      && (init->ui16AdcArray[MCDRV_ADC1_IA] != MCDRV_CHAN_OFF))    ui16CurrSelector = 0;        /* Both ADCs assigned for IA */
    else if((init->ui16AdcArray[MCDRV_ADC0_IB] != MCDRV_CHAN_OFF) && (init->ui16AdcArray[MCDRV_ADC1_IB] != MCDRV_CHAN_OFF))    ui16CurrSelector = 2;        /* Both ADCs assigned for IB */
    else if((init->ui16AdcArray[MCDRV_ADC0_IC] != MCDRV_CHAN_OFF) && (init->ui16AdcArray[MCDRV_ADC1_IC] != MCDRV_CHAN_OFF))    ui16CurrSelector = 4;        /* Both ADCs assigned for IC */
    else                                                                                                                       ui16CurrSelector = FALSE;    /* None of current is assigned to both ADCs */

    /* DO NOT MODIFY THIS CODE - to all MCDRV_ADC channel assigns use config files in board folder */
    switch(ui16CurrSelector)
    {
    case 0 :   
        /*  Phase A current on both ADCs */        
        /* Phase B - ADC 0, Phase C - ADC1 */
        if((!(init->ui16AdcArray[MCDRV_ADC0_IB] == MCDRV_CHAN_OFF)) && (!(init->ui16AdcArray[MCDRV_ADC1_IC] == MCDRV_CHAN_OFF))) 
        {   
            this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];  
            this->sCurrSec16.pAdcBasePhaC = init->pui32Adc1Base;       
            this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];  
            this->sCurrSec16.pAdcBasePhaB = init->pui32Adc0Base; 

            this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];  
            this->sCurrSec23.pAdcBasePhaA = init->pui32Adc0Base; 
            this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC]; 
            this->sCurrSec23.pAdcBasePhaC = init->pui32Adc1Base;                               

            this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];  
            this->sCurrSec45.pAdcBasePhaA = init->pui32Adc1Base; 
            this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];  
            this->sCurrSec45.pAdcBasePhaB = init->pui32Adc0Base;                
        }

        /* Phase A - MCDRV_ADC 1, Phase C - MCDRV_ADC0 */
        if((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) && (!(init->ui16AdcArray[MCDRV_ADC0_IC] == MCDRV_CHAN_OFF))) 
        {   
            this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];  
            this->sCurrSec16.pAdcBasePhaC = init->pui32Adc0Base;     
            this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];  
            this->sCurrSec16.pAdcBasePhaB = init->pui32Adc1Base; 

            this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];  
            this->sCurrSec23.pAdcBasePhaA = init->pui32Adc1Base; 
            this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC]; 
            this->sCurrSec23.pAdcBasePhaC = init->pui32Adc0Base;                               

            this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];  
            this->sCurrSec45.pAdcBasePhaA = init->pui32Adc0Base;     
            this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];  
            this->sCurrSec45.pAdcBasePhaB = init->pui32Adc1Base;              
        }      
        break;
    case 2 :   
        /*  Phase B current on both MCDRV_ADCs */
        /* Phase A - MCDRV_ADC 0, Phase C - MCDRV_ADC1 */
        if((!(init->ui16AdcArray[MCDRV_ADC0_IA] == MCDRV_CHAN_OFF)) && (!(init->ui16AdcArray[MCDRV_ADC1_IC] == MCDRV_CHAN_OFF))) 
        {  
            this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];  
            this->sCurrSec16.pAdcBasePhaC = init->pui32Adc1Base;       
            this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];  
            this->sCurrSec16.pAdcBasePhaB = init->pui32Adc0Base; 

            this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];  
            this->sCurrSec23.pAdcBasePhaA = init->pui32Adc0Base; 
            this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC]; 
            this->sCurrSec23.pAdcBasePhaC = init->pui32Adc1Base;                               

            this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];  
            this->sCurrSec45.pAdcBasePhaA = init->pui32Adc0Base;       
            this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];  
            this->sCurrSec45.pAdcBasePhaB = init->pui32Adc1Base;                
        }
        /* Phase A - MCDRV_ADC 1, Phase C - MCDRV_ADC0 */
        if((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) && (!(init->ui16AdcArray[MCDRV_ADC0_IC] == MCDRV_CHAN_OFF))) 
        {  
            this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];  
            this->sCurrSec16.pAdcBasePhaC = init->pui32Adc0Base;      
            this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];  
            this->sCurrSec16.pAdcBasePhaB = init->pui32Adc1Base; 

            this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];  
            this->sCurrSec23.pAdcBasePhaA = init->pui32Adc1Base; 
            this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC]; 
            this->sCurrSec23.pAdcBasePhaC = init->pui32Adc0Base;                               

            this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];  
            this->sCurrSec45.pAdcBasePhaA = init->pui32Adc1Base;     
            this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];  
            this->sCurrSec45.pAdcBasePhaB = init->pui32Adc0Base;              
        } 
        break;
     case 4 :  
        /*  Phase C current on both MCDRV_ADCs */
        /* Phase A - MCDRV_ADC 0, Phase B - MCDRV_ADC1 */ 
        if((!(init->ui16AdcArray[MCDRV_ADC0_IA] == MCDRV_CHAN_OFF)) && (!(init->ui16AdcArray[MCDRV_ADC1_IB] == MCDRV_CHAN_OFF))) 
        {  
            this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC];  
            this->sCurrSec16.pAdcBasePhaC = init->pui32Adc0Base;         
            this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];  
            this->sCurrSec16.pAdcBasePhaB = init->pui32Adc1Base; 

            this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];  
            this->sCurrSec23.pAdcBasePhaA = init->pui32Adc0Base; 
            this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC]; 
            this->sCurrSec23.pAdcBasePhaC = init->pui32Adc1Base;                               

            this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC0_IA];  
            this->sCurrSec45.pAdcBasePhaA = init->pui32Adc0Base;    
            this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC1_IB];  
            this->sCurrSec45.pAdcBasePhaB = init->pui32Adc1Base;                
        }
        /* Phase A - MCDRV_ADC 1, Phase B - MCDRV_ADC0 */
        if((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) && (!(init->ui16AdcArray[MCDRV_ADC0_IB] == MCDRV_CHAN_OFF))) 
        {  
            this->sCurrSec16.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC1_IC];  
            this->sCurrSec16.pAdcBasePhaC = init->pui32Adc1Base;          
            this->sCurrSec16.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];  
            this->sCurrSec16.pAdcBasePhaB = init->pui32Adc0Base; 

            this->sCurrSec23.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];  
            this->sCurrSec23.pAdcBasePhaA = init->pui32Adc1Base;       
            this->sCurrSec23.ui16ChanNumPhaC = init->ui16AdcArray[MCDRV_ADC0_IC]; 
            this->sCurrSec23.pAdcBasePhaC = init->pui32Adc0Base;                               

            this->sCurrSec45.ui16ChanNumPhaA = init->ui16AdcArray[MCDRV_ADC1_IA];  
            this->sCurrSec45.pAdcBasePhaA = init->pui32Adc1Base;       
            this->sCurrSec45.ui16ChanNumPhaB = init->ui16AdcArray[MCDRV_ADC0_IB];  
            this->sCurrSec45.pAdcBasePhaB = init->pui32Adc0Base;              
        }             
        break;
    default :
        statusPass  = FALSE;  /* If there is no both MCDRV_ADC assigned, this fail occur */
        break;
    }

    /* Check whether Udcb is on ADC0 and Aux on ADC1 */
    if((init->ui16AdcArray[MCDRV_ADC0_UDCB] != MCDRV_CHAN_OFF) && (init->ui16AdcArray[MCDRV_ADC1_AUX] != MCDRV_CHAN_OFF))
    {   
        /* Udcb channel assigned ot ADC0 */
        this->pui32UdcbAdcBase = init->pui32Adc0Base;
        ui16Temp = (ADC_RD_SC1(this->pui32UdcbAdcBase, 1) & 0x60) | init->ui16AdcArray[MCDRV_ADC0_UDCB];   
        ADC_WR_SC1(this->pui32UdcbAdcBase, 1, ui16Temp);
        /* Auxiliary channel assigned ot ADC1 */
        this->pui32AuxAdcBase = init->pui32Adc1Base;
        ui16Temp = (ADC_RD_SC1(this->pui32AuxAdcBase, 1) & 0x60) | init->ui16AdcArray[MCDRV_ADC1_AUX];   
        ADC_WR_SC1(this->pui32AuxAdcBase, 1, ui16Temp);
        
    }    
    /* Check whether Udcb is on ADC1 and Aux on ADC0 */
    else if((init->ui16AdcArray[MCDRV_ADC1_UDCB] != MCDRV_CHAN_OFF) && (init->ui16AdcArray[MCDRV_ADC0_AUX] != MCDRV_CHAN_OFF))
    {
        /* Udcb channel assigned ot ADC1 */
        this->pui32UdcbAdcBase = init->pui32Adc1Base;
        ui16Temp = (ADC_RD_SC1(this->pui32UdcbAdcBase, 1) & 0x60) | init->ui16AdcArray[MCDRV_ADC1_UDCB];   
        ADC_WR_SC1(this->pui32UdcbAdcBase, 1, ui16Temp);
        /* Udcb channel assigned ot ADC1 */
        this->pui32AuxAdcBase = init->pui32Adc0Base;
        ui16Temp = (ADC_RD_SC1(this->pui32AuxAdcBase, 1) & 0x60) | init->ui16AdcArray[MCDRV_ADC0_AUX];   
        ADC_WR_SC1(this->pui32AuxAdcBase, 1, ui16Temp);
    }    
    else    
        statusPass = FALSE;


    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShGet(MCDRV_ADC16_T *this)
           - reads and calculates 3 phase samples based on SVM sector

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShGet(MCDRV_ADC16_T *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;
    statusPass = TRUE;

    switch (*this->pui16SVMSector) 
    {
    case 2:
    case 3: 
        //direct sensing of A, C, calculation of B        
          sIABCtemp.f16A = MLIB_ShLSat_F16(((frac16_t)((int16_t) ADC_RD_R_D(this->sCurrSec23.pAdcBasePhaA, 0)) - this->sCurrSec23.ui16OffsetPhaA),4); 
          sIABCtemp.f16C = MLIB_ShLSat_F16(((frac16_t)((int16_t) ADC_RD_R_D(this->sCurrSec23.pAdcBasePhaC, 0)) - this->sCurrSec23.ui16OffsetPhaC),4);  
          sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A ,sIABCtemp.f16C));      
        break;
    
    case 4: 
    case 5: 
        //direct sensing of A, B, calculation of C 
        sIABCtemp.f16A = MLIB_ShLSat_F16(((frac16_t)((int16_t) ADC_RD_R_D(this->sCurrSec45.pAdcBasePhaA, 0)) - this->sCurrSec45.ui16OffsetPhaA),4);
        sIABCtemp.f16B = MLIB_ShLSat_F16(((frac16_t)((int16_t) ADC_RD_R_D(this->sCurrSec45.pAdcBasePhaB, 0)) - this->sCurrSec45.ui16OffsetPhaB),4);  
        sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A ,sIABCtemp.f16B)); 
        break;

    case 1:
    case 6:
    default:
        //direct sensing of B, C, calculation of A  
        sIABCtemp.f16B = MLIB_ShLSat_F16(((frac16_t)((int16_t) ADC_RD_R_D(this->sCurrSec16.pAdcBasePhaB, 0)) - this->sCurrSec16.ui16OffsetPhaB),4);
        sIABCtemp.f16C = MLIB_ShLSat_F16(((frac16_t)((int16_t) ADC_RD_R_D(this->sCurrSec16.pAdcBasePhaC, 0)) - this->sCurrSec16.ui16OffsetPhaC),4);  
        sIABCtemp.f16A = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16B ,sIABCtemp.f16C));       
        break;
    }

     /* pass measured phase currents to the main module structure */
    this->psIABC->f16A = sIABCtemp.f16A;
    this->psIABC->f16B = sIABCtemp.f16B;
    this->psIABC->f16C = sIABCtemp.f16C;
    
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShChanAssign(MCDRV_ADC16_T *this)
           - set new channel assingment for next sampling based on SVM sector

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShChanAssign(MCDRV_ADC16_T *this)
{
    uint16_t ui16Temp;    
    
    statusPass = TRUE;
    /* in order to keep the setting of ADC_SC1 (the AIEN and DIFF flags), 
      the original value of the register is first "anded" with 0x60 and then 
      "ored" with mask that sets the MCDRV_ADCH
    */
    
    switch (*this->pui16SVMSector) 
    {
    case 2:
    case 3: /* currents Ia, Ic will be measured */
            ui16Temp = (ADC_RD_SC1(this->sCurrSec23.pAdcBasePhaA, 0) & 0x60);   // Load value from ADC_SC1n register to temp variable
            ui16Temp = ui16Temp | this->sCurrSec23.ui16ChanNumPhaA;        // Assign MCDRV_ADC channel to temp variable 
            ADC_WR_SC1(this->sCurrSec23.pAdcBasePhaA, 0, ui16Temp);             // Load value from variable and set MCDRV_ADC channel to MCDRV_ADCn register
                       
            ui16Temp = (ADC_RD_SC1(this->sCurrSec23.pAdcBasePhaC, 0) & 0x60);   // Load value from ADC_SC1n register to temp variable
            ui16Temp = ui16Temp | this->sCurrSec23.ui16ChanNumPhaC;        // Assign MCDRV_ADC channel to temp variable 
            ADC_WR_SC1(this->sCurrSec23.pAdcBasePhaC, 0, ui16Temp);             // Load value from variable and set MCDRV_ADC channel to MCDRV_ADCn register
            break;
        case 4:
        case 5: /*  currents Ia, Ib will be measured */   
            ui16Temp = (ADC_RD_SC1(this->sCurrSec45.pAdcBasePhaA, 0) & 0x60);   // Load value from ADC_SC1n register to temp variable
            ui16Temp = ui16Temp | this->sCurrSec45.ui16ChanNumPhaA;        // Assign MCDRV_ADC channel to temp variable 
            ADC_WR_SC1(this->sCurrSec45.pAdcBasePhaA, 0, ui16Temp);             // Load value from variable and set MCDRV_ADC channel to MCDRV_ADCn register
                       
            ui16Temp = (ADC_RD_SC1(this->sCurrSec45.pAdcBasePhaB, 0) & 0x60);   // Load value from ADC_SC1n register to temp variable
            ui16Temp = ui16Temp | this->sCurrSec45.ui16ChanNumPhaB;        // Assign MCDRV_ADC channel to temp variable 
            ADC_WR_SC1(this->sCurrSec45.pAdcBasePhaB, 0, ui16Temp);             // Load value from variable and set MCDRV_ADC channel to MCDRV_ADCn register
            break;
        case 1:
        case 6: /* currents Ib, Ic will be measured */
        default:
            ui16Temp = (ADC_RD_SC1(this->sCurrSec16.pAdcBasePhaB, 0) & 0x60);   // Load value from ADC_SC1n register to temp variable
            ui16Temp = ui16Temp | this->sCurrSec16.ui16ChanNumPhaB;        // Assign MCDRV_ADC channel to temp variable 
            ADC_WR_SC1(this->sCurrSec16.pAdcBasePhaB, 0, ui16Temp);             // Load value from variable and set MCDRV_ADC channel to MCDRV_ADCn register
                       
            ui16Temp = (ADC_RD_SC1(this->sCurrSec16.pAdcBasePhaC, 0) & 0x60);   // Load value from ADC_SC1n register to temp variable
            ui16Temp = ui16Temp | this->sCurrSec16.ui16ChanNumPhaC;        // Assign MCDRV_ADC channel to temp variable 
            ADC_WR_SC1(this->sCurrSec16.pAdcBasePhaC, 0, ui16Temp);             // Load value from variable and set MCDRV_ADC channel to MCDRV_ADCn register
            break;
    }
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShChanAssign(MCDRV_ADC16_T *this)
           - intializes phase current channel offest measurement

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShCalibInit(MCDRV_ADC16_T *this)
{
    statusPass = TRUE;
    
    /* clear offset values */
    this->sCurrSec16.ui16OffsetPhaB = 0;
    this->sCurrSec16.ui16OffsetPhaC = 0;
    this->sCurrSec23.ui16OffsetPhaA = 0;
    this->sCurrSec23.ui16OffsetPhaC = 0;
    this->sCurrSec45.ui16OffsetPhaA = 0;
    this->sCurrSec45.ui16OffsetPhaB = 0;
    
    this->sCurrSec16.ui16CalibPhaB = 0;
    this->sCurrSec16.ui16CalibPhaC = 0;
    this->sCurrSec23.ui16CalibPhaA = 0;
    this->sCurrSec23.ui16CalibPhaC = 0;
    this->sCurrSec45.ui16CalibPhaA = 0;
    this->sCurrSec45.ui16CalibPhaB = 0;
    
    /* initalize offset filters */
    this->sCurrSec16.ui16FiltPhaB.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec16.ui16FiltPhaC.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec23.ui16FiltPhaA.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec23.ui16FiltPhaC.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec45.ui16FiltPhaA.u16Sh = this->ui16OffsetFiltWindow;
    this->sCurrSec45.ui16FiltPhaB.u16Sh = this->ui16OffsetFiltWindow;
      
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec16.ui16FiltPhaB);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec16.ui16FiltPhaC); 
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec23.ui16FiltPhaA);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec23.ui16FiltPhaC);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec45.ui16FiltPhaA);
    GDFLIB_FilterMAInit_F16((frac16_t)0, &this->sCurrSec45.ui16FiltPhaB);
   
    return(statusPass);
}    

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShCalib(MCDRV_ADC16_T *this)
           - reads current samples and filter them based on SVM sector

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShCalib(MCDRV_ADC16_T *this)
{
    statusPass = TRUE;

    switch (*this->pui16SVMSector) 
    {
    case 2:
    case 3: 
        /* sensing of offset IA -> MCDRV_ADC1_A and IC -> MCDRV_ADC0_A */
        this->sCurrSec23.ui16CalibPhaA = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_R_D(this->sCurrSec23.pAdcBasePhaA, 0), &this->sCurrSec23.ui16FiltPhaA);
      
        this->sCurrSec23.ui16CalibPhaC = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_R_D(this->sCurrSec23.pAdcBasePhaC, 0), &this->sCurrSec23.ui16FiltPhaC);      
        
        break;
    
    case 4: 
    case 5: 
        /* sensing of offset IA -> MCDRV_ADC1_A and IB -> MCDRV_ADC0_A */
        this->sCurrSec45.ui16CalibPhaA = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_R_D(this->sCurrSec45.pAdcBasePhaA, 0), &this->sCurrSec45.ui16FiltPhaA);
      
        this->sCurrSec45.ui16CalibPhaB = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_R_D(this->sCurrSec45.pAdcBasePhaB, 0), &this->sCurrSec45.ui16FiltPhaB); 
        break;

    case 1:
    case 6:
    default:
        /* sensing of offset IB -> ADC_A and IC -> ADC_B */
        this->sCurrSec16.ui16CalibPhaB = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_R_D(this->sCurrSec16.pAdcBasePhaB, 0), &this->sCurrSec16.ui16FiltPhaB);
      
        this->sCurrSec16.ui16CalibPhaC = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_R_D(this->sCurrSec16.pAdcBasePhaC, 0), &this->sCurrSec16.ui16FiltPhaC); 
        break;
    }
    
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShCalibSet(MCDRV_ADC16_T *this)
           - passes measured offset values to main structure 

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShCalibSet(MCDRV_ADC16_T *this)
{
    statusPass = TRUE;
    
    /* pass calib data for sector 1&6 */
    this->sCurrSec16.ui16OffsetPhaB = this->sCurrSec16.ui16CalibPhaB;
    this->sCurrSec16.ui16OffsetPhaC = this->sCurrSec16.ui16CalibPhaC;
    /* pass calib data for sector 2&3 */
    this->sCurrSec23.ui16OffsetPhaA = this->sCurrSec23.ui16CalibPhaA;
    this->sCurrSec23.ui16OffsetPhaC = this->sCurrSec23.ui16CalibPhaC;    
    /* pass calib data for sector 4&5 */
    this->sCurrSec45.ui16OffsetPhaA = this->sCurrSec45.ui16CalibPhaA;
    this->sCurrSec45.ui16OffsetPhaB = this->sCurrSec45.ui16CalibPhaB;    
    
    return(statusPass);
} 

/******************************************************************************
@brief   bool_t MCDRV_VoltDcBusGet(MCDRV_ADC16_T * this)
           - reads and passes DCB voltage sample 

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_VoltDcBusGet(MCDRV_ADC16_T *this)
{
    statusPass = TRUE;

    /* read DC Bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)(MLIB_ShLSat_F16((ADC_RD_R_D(this->pui32UdcbAdcBase, 1)), 3));
    
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_AuxValGet(MCDRV_ADC16_T * this)
           - reads and passes auxiliary sample 

@param   this   Pointer to the current object.

@return  boot_t
******************************************************************************/
bool_t MCDRV_AuxValGet(MCDRV_ADC16_T *this)
{
    statusPass = TRUE;
        
    /* read Auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = (frac16_t)(MLIB_ShLSat_F16((ADC_RD_R_D(this->pui32AuxAdcBase, 1)), 3));
    
    return(statusPass);
}

/* End of file */
