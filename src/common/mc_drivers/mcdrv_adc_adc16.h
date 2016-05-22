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
* @file    mcdrv_adc_adc16.h
*
* @brief    A header file of 3-phase current and voltage sensing module
* 
* @board    -
* 
******************************************************************************/
#ifndef __MCDRV_ADC_ADC16_H_
#define __MCDRV_ADC_ADC16_H_

/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "gdflib.h"
#include "mlib_types.h"
#include "fsl_device_registers.h"
#include "gmclib.h"

/******************************************************************************
| defines and macros
|----------------------------------------------------------------------------*/
#define MCDRV_ADC16                   (1)

#define MCDRV_CHAN_OFF                (0x1F)                 
#define MCDRV_ADC0_IA                 (0)
#define MCDRV_ADC1_IA                 (1)
#define MCDRV_ADC0_IB                 (2)
#define MCDRV_ADC1_IB                 (3)
#define MCDRV_ADC0_IC                 (4)
#define MCDRV_ADC1_IC                 (5)
#define MCDRV_ADC0_UDCB               (6)
#define MCDRV_ADC1_UDCB               (7)
#define MCDRV_ADC0_AUX                (8)
#define MCDRV_ADC1_AUX                (9)

/******************************************************************************
| typedefs 
|----------------------------------------------------------------------------*/
typedef struct MCDRV_ADC16_INIT_T     MCDRV_ADC16_INIT_T;    
typedef struct MCDRV_ADC16_T          MCDRV_ADC16_T;                      /* public */

/******************************************************************************
| structures 
|----------------------------------------------------------------------------*/
typedef struct 
{
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaB;       /* Phase B offset filter */
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaC;       /* Phase C offset filter */
    uint16_t                    ui16ChanNumPhaB;    /* Phase B channel number */
    uint16_t                    ui16ChanNumPhaC;    /* Phase C channel number */
    uint16_t                    ui16CalibPhaB;      /* Phase B offset calibration */
    uint16_t                    ui16CalibPhaC;      /* Phase C offset calibration */
    uint16_t                    ui16OffsetPhaB;     /* Phase B offset result */
    uint16_t                    ui16OffsetPhaC;     /* Phase C offset result */
    ADC_Type                  * pAdcBasePhaB;       /* Pointer to Phase B base address */
    ADC_Type                  * pAdcBasePhaC;       /* Pointer to Phase C base address */    
} PHA_BC_T;

typedef struct 
{
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaA;       /* Phase A offset filter */
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaC;       /* Phase C offset filter */
    uint16_t                    ui16ChanNumPhaA;    /* Phase A channel number */
    uint16_t                    ui16ChanNumPhaC;    /* Phase C channel number */
    uint16_t                    ui16CalibPhaA;      /* Phase A offset calibration */
    uint16_t                    ui16CalibPhaC;      /* Phase C offset calibration */
    uint16_t                    ui16OffsetPhaA;     /* Phase A offset result */
    uint16_t                    ui16OffsetPhaC;     /* Phase C offset result */
    ADC_Type                  * pAdcBasePhaA;       /* Pointer to phase A base address */
    ADC_Type                  * pAdcBasePhaC;       /* Pointer to phase C base address */      
} PHA_AC_T;

typedef struct
{
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaA;       /* Phase A offset filter */
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaB;       /* Phase B offset filter */
    uint16_t                    ui16ChanNumPhaA;    /* Phase A channel number */
    uint16_t                    ui16ChanNumPhaB;    /* Phase B channel number */
    uint16_t                    ui16CalibPhaA;      /* Phase A offset calibration */
    uint16_t                    ui16CalibPhaB;      /* Phase B offset calibration */
    uint16_t                    ui16OffsetPhaA;     /* Phase A offset result */
    uint16_t                    ui16OffsetPhaB;     /* Phase B offset result */
    ADC_Type                  * pAdcBasePhaA;       /* Pointer to phase A base address */
    ADC_Type                  * pAdcBasePhaB;       /* Pointer to phase B base address */  
}PHA_AB_T;

struct MCDRV_ADC16_T
{
    GMCLIB_3COOR_T_F16        * psIABC;             /* Pointer to the 3-phase currents */
    PHA_BC_T                    sCurrSec16;         /* ADC setting for SVM sectors 1&6 */
    PHA_AC_T                    sCurrSec23;         /* ADC setting for SVM sectors 2&3 */
    PHA_AB_T                    sCurrSec45;         /* ADC setting for SVM sectors 4&5 */
    
    ADC_Type                  * pui32UdcbAdcBase;   /* Pointer to ADC where Udcb channel is assigned */
    ADC_Type                  * pui32AuxAdcBase;    /* Pointer to ADC where auxiliary channel is assigned */
    uint16_t                  * pui16SVMSector;     /* Pointer to the SVM sector */ 
    frac16_t                  * pui16AuxChan;       /* Pointer to auxiliary ADC channel number */
    frac16_t                  * pf16UDcBus;         /* Pointer to DC Bus voltage variable */ 
    
    uint16_t                    ui16OffsetFiltWindow; /* ADC Offset filter window */
};

struct MCDRV_ADC16_INIT_T
{
    uint16_t                  * ui16AdcArray;       /* Pointer to ADC Array of channel configuration*/
    ADC_Type                  * pui32Adc0Base;      /* Pointer to ADC0 module */
    ADC_Type                  * pui32Adc1Base;      /* Pointer to ADC1 module */
};
/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| exported function prototypes
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

bool_t MCDRV_Adc16Init(MCDRV_ADC16_T * this, MCDRV_ADC16_INIT_T * init);
bool_t MCDRV_Curr3Ph2ShGet(MCDRV_ADC16_T *this);
bool_t MCDRV_Curr3Ph2ShChanAssign(MCDRV_ADC16_T *this);  
bool_t MCDRV_Curr3Ph2ShCalibInit(MCDRV_ADC16_T *this);
bool_t MCDRV_Curr3Ph2ShCalib(MCDRV_ADC16_T *this);
bool_t MCDRV_Curr3Ph2ShCalibSet(MCDRV_ADC16_T *this);

bool_t MCDRV_VoltDcBusGet(MCDRV_ADC16_T *this);
bool_t MCDRV_AuxValGet(MCDRV_ADC16_T *this);

#ifdef __cplusplus
}
#endif

#endif /* MCDRV_ADC_ADC16_H_ */
