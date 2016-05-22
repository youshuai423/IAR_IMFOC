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
* @file     mcdrv_adc_adc12.c
*
* @brief   A header file of 3-phase current, DC-bus voltage and auxiliary 
*          quantity sensing module
*
* @periph  12-bit cyclic ADC 
*
*******************************************************************************/
/******************************************************************************
| includes
|----------------------------------------------------------------------------*/
#include "mcdrv_adc_adc12.h"

/******************************************************************************
| defines and macros                                   
|----------------------------------------------------------------------------*/

/******************************************************************************
| typedefs and structures                             
|----------------------------------------------------------------------------*/

/******************************************************************************
| local variable definitions                       
|----------------------------------------------------------------------------*/
static bool_t statusPass;

/******************************************************************************
| global variable definitions                     
|----------------------------------------------------------------------------*/

/******************************************************************************
| function prototypes                                   
|----------------------------------------------------------------------------*/

/******************************************************************************
| function implementations                            
|----------------------------------------------------------------------------*/
/******************************************************************************
@brief   bool_t MCDRV_Adc12Init(MCDRV_ADC12_T * this, MCDRV_ADC12_INIT_T * init)
           - Initializes ADC driver to measure 3 currents, DC-bus voltage and
             auxiliary quantity samples

@param   this   Pointer to the current object
         init   Pointer to initialization structure

@return  boot_t True on success
******************************************************************************/
bool_t MCDRV_Adc12Init(MCDRV_ADC12_T * this, MCDRV_ADC12_INIT_T * init)
{
    uint16_t ui16CurrSelector = 0;
    
    statusPass = TRUE;
    
    /* pass ADC base from init structure */
    this->pui32AdcBase = init->pui32AdcBase;
    
    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;

    /* check if there are one pair of assigned channels */
    if((init->ui16AdcArray[MCDRV_ADC0_IA] != MCDRV_CHAN_OFF) && 
       (init->ui16AdcArray[MCDRV_ADC1_IA] != MCDRV_CHAN_OFF))    
    {
        /* both ADCs assigned for IA */
        ui16CurrSelector = 0;       
    }
    else if((init->ui16AdcArray[MCDRV_ADC0_IB] != MCDRV_CHAN_OFF) && 
            (init->ui16AdcArray[MCDRV_ADC1_IB] != MCDRV_CHAN_OFF))   
    {
        /* both ADCs assigned for IB */
        ui16CurrSelector = 2;       
    }
    else if((init->ui16AdcArray[MCDRV_ADC0_IC] != MCDRV_CHAN_OFF) && 
            (init->ui16AdcArray[MCDRV_ADC1_IC] != MCDRV_CHAN_OFF))   
    {
        /* both ADCs assigned for IC */
        ui16CurrSelector = 4;     
    }
    else
    {
        /* none of current is assigned to both ADCs */
        statusPass = FALSE;
    }
    
    /* DO NOT MODIFY THIS CODE - to all MCDRV_ADC channel assigns use config 
       files in board folder */
    switch(ui16CurrSelector)
    {
    case 0 :   
        /* phase A current on both ADCs */        
        /* phase B on ADCA, phase C on ADCB */
        if((!(init->ui16AdcArray[MCDRV_ADC0_IB] == MCDRV_CHAN_OFF)) && 
           (!(init->ui16AdcArray[MCDRV_ADC1_IC] == MCDRV_CHAN_OFF))) 
        {   
            this->sCurrSec16.ui16RsltNumPhaC = 8;  
            this->sCurrSec16.ui16RsltNumPhaB = 0;
            this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
            this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

            this->sCurrSec23.ui16RsltNumPhaA = 0;  
            this->sCurrSec23.ui16RsltNumPhaC = 8;
            this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
            this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

            this->sCurrSec45.ui16RsltNumPhaA = 8;
            this->sCurrSec45.ui16RsltNumPhaB = 0;
            this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
            this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;
        }
        /* phase A on ADCB, phase C on ADCA */
        else if((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) && 
           (!(init->ui16AdcArray[MCDRV_ADC0_IC] == MCDRV_CHAN_OFF))) 
        {   
            this->sCurrSec16.ui16RsltNumPhaC = 0;  
            this->sCurrSec16.ui16RsltNumPhaB = 8;
            this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
            this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;

            this->sCurrSec23.ui16RsltNumPhaA = 8;  
            this->sCurrSec23.ui16RsltNumPhaC = 0;
            this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
            this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8; 

            this->sCurrSec45.ui16RsltNumPhaA = 0;
            this->sCurrSec45.ui16RsltNumPhaB = 8;
            this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
            this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;
        }  
        else statusPass = FALSE;
        break;
    case 2 :   
        /* phase B current on both ADCs */  
        /* phase A on ADCA, phase C on ADCB */
        if((!(init->ui16AdcArray[MCDRV_ADC0_IA] == MCDRV_CHAN_OFF)) && 
           (!(init->ui16AdcArray[MCDRV_ADC1_IC] == MCDRV_CHAN_OFF))) 
        {  
            this->sCurrSec16.ui16RsltNumPhaC = 8;
            this->sCurrSec16.ui16RsltNumPhaB = 0;
            this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
            this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

            this->sCurrSec23.ui16RsltNumPhaA = 0;
            this->sCurrSec23.ui16RsltNumPhaC = 8;
            this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
            this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

            this->sCurrSec45.ui16RsltNumPhaA = 0;
            this->sCurrSec45.ui16RsltNumPhaB = 8;
            this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
            this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;
        }
        /* phase A  on ADCB, phase C on ADCA */
        else if((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) && 
           (!(init->ui16AdcArray[MCDRV_ADC0_IC] == MCDRV_CHAN_OFF))) 
        {  
            this->sCurrSec16.ui16RsltNumPhaC = 0;  
            this->sCurrSec16.ui16RsltNumPhaB = 8;
            this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
            this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;

            this->sCurrSec23.ui16RsltNumPhaA = 8;  
            this->sCurrSec23.ui16RsltNumPhaC = 0;
            this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
            this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;

            this->sCurrSec45.ui16RsltNumPhaA = 8;  
            this->sCurrSec45.ui16RsltNumPhaB = 0;
            this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
            this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;
        } 
        else statusPass = FALSE;
        break;
     case 4 :  
        /* phase C current on both ADCs */
        /* phase A on ADCA, phase B on ADCB */ 
        if((!(init->ui16AdcArray[MCDRV_ADC0_IA] == MCDRV_CHAN_OFF)) && 
           (!(init->ui16AdcArray[MCDRV_ADC1_IB] == MCDRV_CHAN_OFF))) 
        {  
            this->sCurrSec16.ui16RsltNumPhaC = 0;
            this->sCurrSec16.ui16RsltNumPhaB = 8;
            this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
            this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;

            this->sCurrSec23.ui16RsltNumPhaA = 0;
            this->sCurrSec23.ui16RsltNumPhaC = 8;
            this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
            this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

            this->sCurrSec45.ui16RsltNumPhaA = 0;
            this->sCurrSec45.ui16RsltNumPhaB = 8;
            this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IA];
            this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IB] + 8;
        }
        /* phase A on ADCB, phase B on ADCA */
        else if((!(init->ui16AdcArray[MCDRV_ADC1_IA] == MCDRV_CHAN_OFF)) && 
           (!(init->ui16AdcArray[MCDRV_ADC0_IB] == MCDRV_CHAN_OFF))) 
        {  
            this->sCurrSec16.ui16RsltNumPhaC = 8;
            this->sCurrSec16.ui16RsltNumPhaB = 0;
            this->sCurrSec16.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
            this->sCurrSec16.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IC] + 8;

            this->sCurrSec23.ui16RsltNumPhaA = 8;
            this->sCurrSec23.ui16RsltNumPhaC = 0;
            this->sCurrSec23.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IC];
            this->sCurrSec23.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;

            this->sCurrSec45.ui16RsltNumPhaA = 8;
            this->sCurrSec45.ui16RsltNumPhaB = 0;
            this->sCurrSec45.ui16Sample0 = init->ui16AdcArray[MCDRV_ADC0_IB];
            this->sCurrSec45.ui16Sample8 = init->ui16AdcArray[MCDRV_ADC1_IA] + 8;
        }             
        else statusPass = FALSE;
        break;
    default:
        break;
    }

    /* check whether DC-bus voltage is on ADCA and Aux on ADCB */
    if((init->ui16AdcArray[MCDRV_ADC0_UDCB] != MCDRV_CHAN_OFF) && 
       (init->ui16AdcArray[MCDRV_ADC1_AUX] != MCDRV_CHAN_OFF))
    {   
        /* DC-bus voltage channel assigned ot ADCA */      
        ADC_WR_CLIST1_SAMPLE1(this->pui32AdcBase, 
                              init->ui16AdcArray[MCDRV_ADC0_UDCB]);
        this->ui16RsltNumDcBus   = 1;
        
        /* Auxiliary channel assigned ot ADCB */
        ADC_WR_CLIST3_SAMPLE9(this->pui32AdcBase, 
                              init->ui16AdcArray[MCDRV_ADC1_AUX] + 8);
        this->ui16RsltNumAuxChan = 9;
        
    }    
    /* check whether DC-bus voltage is on ADCB and Aux on ADCA */
    else if((init->ui16AdcArray[MCDRV_ADC1_UDCB] != MCDRV_CHAN_OFF) && 
            (init->ui16AdcArray[MCDRV_ADC0_AUX] != MCDRV_CHAN_OFF))
    {
        /* DC-bus voltage channel assigned ot ADCB */
        ADC_WR_CLIST3_SAMPLE9(this->pui32AdcBase, 
                              init->ui16AdcArray[MCDRV_ADC1_UDCB] + 8);
        this->ui16RsltNumDcBus   = 9;
        
        /* Auxiliary channel assigned ot ADCA */
        ADC_WR_CLIST1_SAMPLE1(this->pui32AdcBase, 
                              init->ui16AdcArray[MCDRV_ADC0_AUX]);
        this->ui16RsltNumAuxChan = 1;
    }    
    else statusPass = FALSE;


    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShGet(MCDRV_ADC12_T *this)
           - reads and calculates 3 phase samples based on SVM sector

@param   this   Pointer to the current object.

@return  boot_t True on success
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShGet(MCDRV_ADC12_T *this)
{
    GMCLIB_3COOR_T_F16 sIABCtemp;
    
    statusPass = TRUE;

    switch(*this->pui16SVMSector) 
    {
    case 2:
    case 3: 
        /* direct sensing of phases A and C, calculation of phase B */ 
        sIABCtemp.f16A = MLIB_ShLSat_F16(((frac16_t)((int16_t) 
            ADC_RD_RSLT(this->pui32AdcBase, this->sCurrSec23.ui16RsltNumPhaA)) - 
            this->sCurrSec23.ui16OffsetPhaA), 1); 
        sIABCtemp.f16C = MLIB_ShLSat_F16(((frac16_t)((int16_t) 
            ADC_RD_RSLT(this->pui32AdcBase, this->sCurrSec23.ui16RsltNumPhaC)) - 
            this->sCurrSec23.ui16OffsetPhaC), 1);  
        sIABCtemp.f16B = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, 
                                                      sIABCtemp.f16C));                                                                                 
        break;                                                                                                                                                          
                                                                                                                                                                        
    case 4:                                                                                                                                                             
    case 5:                                                                                                                                                            
        /* direct sensing of phases A and B, calculation of phase C */                                                                                                                     
        sIABCtemp.f16A = MLIB_ShLSat_F16(((frac16_t)((int16_t) 
            ADC_RD_RSLT(this->pui32AdcBase, this->sCurrSec45.ui16RsltNumPhaA)) - 
            this->sCurrSec45.ui16OffsetPhaA), 1);
        sIABCtemp.f16B = MLIB_ShLSat_F16(((frac16_t)((int16_t) 
            ADC_RD_RSLT(this->pui32AdcBase, this->sCurrSec45.ui16RsltNumPhaB)) - 
            this->sCurrSec45.ui16OffsetPhaB), 1);  
        sIABCtemp.f16C = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16A, 
                                                      sIABCtemp.f16B));                                                                                 
        break;                                                                                                                                                          
                                                                                                                                                                        
    case 1:                                                                                                                                                             
    case 6:                                                                                                                                                             
    default:                                                                                                                                                            
        /* direct sensing of phases B and C, calculation of A */                                                                                                                     
        sIABCtemp.f16B = MLIB_ShLSat_F16(((frac16_t)((int16_t) 
            ADC_RD_RSLT(this->pui32AdcBase, this->sCurrSec16.ui16RsltNumPhaB)) - 
            this->sCurrSec16.ui16OffsetPhaB), 1);
        sIABCtemp.f16C = MLIB_ShLSat_F16(((frac16_t)((int16_t) 
            ADC_RD_RSLT(this->pui32AdcBase, this->sCurrSec16.ui16RsltNumPhaC)) - 
            this->sCurrSec16.ui16OffsetPhaC), 1);  
        sIABCtemp.f16A = MLIB_Neg_F16(MLIB_AddSat_F16(sIABCtemp.f16B,
                                                      sIABCtemp.f16C));       
        break;
    }

    /* pass measured phase currents to the main module structure */
    this->psIABC->f16A = sIABCtemp.f16A;
    this->psIABC->f16B = sIABCtemp.f16B;
    this->psIABC->f16C = sIABCtemp.f16C;
    
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShChanAssign(MCDRV_ADC12_T *this)
           - set new channel assingment for next sampling based on SVM sector

@param   this   Pointer to the current object.

@return  boot_t True on success
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShChanAssign(MCDRV_ADC12_T *this)
{

    statusPass = TRUE;
       
    switch(*this->pui16SVMSector) 
    {
    case 2:
    case 3: 
        /* direct sensing of phases A and C */ 
        ADC_WR_CLIST1_SAMPLE0(this->pui32AdcBase, this->sCurrSec23.ui16Sample0);
        ADC_WR_CLIST3_SAMPLE8(this->pui32AdcBase, this->sCurrSec23.ui16Sample8);
        break;
        
    case 4:
    case 5: 
        /* direct sensing of phases A and B  */ 
        ADC_WR_CLIST1_SAMPLE0(this->pui32AdcBase, this->sCurrSec45.ui16Sample0);
        ADC_WR_CLIST3_SAMPLE8(this->pui32AdcBase, this->sCurrSec45.ui16Sample8);
        break;
        
    case 1:
    case 6: 
    default:
        /* direct sensing of phases B and C */ 
        ADC_WR_CLIST1_SAMPLE0(this->pui32AdcBase, this->sCurrSec16.ui16Sample0);
        ADC_WR_CLIST3_SAMPLE8(this->pui32AdcBase, this->sCurrSec16.ui16Sample8);
        break;
    }

    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShChanAssign(MCDRV_ADC12_T *this)
           - intializes phase current channel offest measurement

@param   this   Pointer to the current object.

@return  boot_t True on success
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShCalibInit(MCDRV_ADC12_T *this)
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
@brief   bool_t MCDRV_Curr3Ph2ShCalib(MCDRV_ADC12_T *this)
           - reads current samples and filter them based on SVM sector

@param   this   Pointer to the current object.

@return  boot_t True on success
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShCalib(MCDRV_ADC12_T *this)
{
    statusPass = TRUE;

    switch (*this->pui16SVMSector) 
    {
    case 2:
    case 3: 
        /* direct sensing of measurement offsets on phases A and C */ 
        this->sCurrSec23.ui16CalibPhaA = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_RSLT(this->pui32AdcBase, 
             this->sCurrSec23.ui16RsltNumPhaA), &this->sCurrSec23.ui16FiltPhaA);
        this->sCurrSec23.ui16CalibPhaC = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_RSLT(this->pui32AdcBase, 
             this->sCurrSec23.ui16RsltNumPhaC), &this->sCurrSec23.ui16FiltPhaC);
        break;
    
    case 4: 
    case 5: 
        /* direct sensing of measurement offsets on phases A and B */ 
        this->sCurrSec45.ui16CalibPhaA = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_RSLT(this->pui32AdcBase, 
             this->sCurrSec45.ui16RsltNumPhaA), &this->sCurrSec45.ui16FiltPhaA);
        this->sCurrSec45.ui16CalibPhaB = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_RSLT(this->pui32AdcBase, 
             this->sCurrSec45.ui16RsltNumPhaB), &this->sCurrSec45.ui16FiltPhaB); 
        break;

    case 1:
    case 6:
    default:
        /* direct sensing of measurement offsets on phases B and C */ 
        this->sCurrSec16.ui16CalibPhaB = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_RSLT(this->pui32AdcBase, 
             this->sCurrSec16.ui16RsltNumPhaB), &this->sCurrSec16.ui16FiltPhaB);
        this->sCurrSec16.ui16CalibPhaC = 
             GDFLIB_FilterMA_F16((frac16_t) ADC_RD_RSLT(this->pui32AdcBase, 
             this->sCurrSec16.ui16RsltNumPhaC), &this->sCurrSec16.ui16FiltPhaC); 
        break;        
    }
    
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_Curr3Ph2ShCalibSet(MCDRV_ADC12_T *this)
           - passes measured offset values to main structure 

@param   this   Pointer to the current object.

@return  boot_t True on success
******************************************************************************/
bool_t MCDRV_Curr3Ph2ShCalibSet(MCDRV_ADC12_T *this)
{
    statusPass = TRUE;
    
    /* pass calibration data for sector 1 and 6 */
    this->sCurrSec16.ui16OffsetPhaB = this->sCurrSec16.ui16CalibPhaB;
    this->sCurrSec16.ui16OffsetPhaC = this->sCurrSec16.ui16CalibPhaC;
    
    /* pass calibration data for sector 2 and 3 */
    this->sCurrSec23.ui16OffsetPhaA = this->sCurrSec23.ui16CalibPhaA;
    this->sCurrSec23.ui16OffsetPhaC = this->sCurrSec23.ui16CalibPhaC; 
    
    /* pass calibration data for sector 4 and 5 */
    this->sCurrSec45.ui16OffsetPhaA = this->sCurrSec45.ui16CalibPhaA;
    this->sCurrSec45.ui16OffsetPhaB = this->sCurrSec45.ui16CalibPhaB;    
    
    return(statusPass);
} 

/******************************************************************************
@brief   bool_t MCDRV_VoltDcBusGet(MCDRV_ADC12_T * this)
           - reads and passes DCB voltage sample 

@param   this   Pointer to the current object.

@return  boot_t True on success
******************************************************************************/
bool_t MCDRV_VoltDcBusGet(MCDRV_ADC12_T *this)
{
    statusPass = TRUE;
        
    /* read DC-bus voltage sample from defined ADCx result register */
    *this->pf16UDcBus = (frac16_t)(ADC_RD_RSLT(this->pui32AdcBase, 
                                               this->ui16RsltNumDcBus));
    
    return(statusPass);
}

/******************************************************************************
@brief   bool_t MCDRV_AuxValGet(MCDRV_ADC12_T * this)
           - reads and passes auxiliary sample 

@param   this   Pointer to the current object.

@return  boot_t True on success
******************************************************************************/
bool_t MCDRV_AuxValGet(MCDRV_ADC12_T *this)
{
    statusPass = TRUE;
        
    /* read Auxiliary channel sample from defined ADCx result register */
    *this->pui16AuxChan = (frac16_t)(ADC_RD_RSLT(this->pui32AdcBase, 
                                                  this->ui16RsltNumAuxChan));
    
    return(statusPass);
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
