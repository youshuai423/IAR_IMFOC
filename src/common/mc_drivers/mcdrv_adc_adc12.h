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
* @file    mcdrv_adc_adc12.h
*
* @brief   A header file of 3-phase current, DC-bus voltage and auxiliary 
*          quantity sensing module
*
* @periph  12-bit cyclic ADC 
* 
******************************************************************************/
#ifndef _MCDRV_ADC_ADC12_H_
#define _MCDRV_ADC_ADC12_H_

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
#define MCDRV_ADC12                   (1)

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
typedef struct MCDRV_ADC12_INIT_T           MCDRV_ADC12_INIT_T;    
typedef struct MCDRV_ADC12_T                MCDRV_ADC12_T;                      

/******************************************************************************
| structures 
|----------------------------------------------------------------------------*/
typedef struct 
{
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaB;                                   /* phase B offset filter */
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaC;                                   /* phase C offset filter */
    uint16_t                    ui16RsltNumPhaB;                                /* phase B channel number */
    uint16_t                    ui16RsltNumPhaC;                                /* phase C channel number */
    uint16_t                    ui16Sample0;                                    /* channel assigned to sample 0 */
    uint16_t                    ui16Sample8;                                    /* channel assigned to sample 8 */
    uint16_t                    ui16CalibPhaB;                                  /* phase B offset calibration */
    uint16_t                    ui16CalibPhaC;                                  /* phase C offset calibration */
    uint16_t                    ui16OffsetPhaB;                                 /* phase B offset result */
    uint16_t                    ui16OffsetPhaC;                                 /* phase C offset result */
} PHA_BC_T;

typedef struct 
{
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaA;                                   /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaC;                                   /* phase C offset filter */
    uint16_t                    ui16RsltNumPhaA;                                /* phase A channel number */
    uint16_t                    ui16RsltNumPhaC;                                /* phase C channel number */
    uint16_t                    ui16Sample0;                                    /* channel assigned to sample 0 */
    uint16_t                    ui16Sample8;                                    /* channel assigned to sample 8 */
    uint16_t                    ui16CalibPhaA;                                  /* phase A offset calibration */
    uint16_t                    ui16CalibPhaC;                                  /* phase C offset calibration */
    uint16_t                    ui16OffsetPhaA;                                 /* phase A offset result */
    uint16_t                    ui16OffsetPhaC;                                 /* phase C offset result */     
} PHA_AC_T;

typedef struct
{
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaA;                                   /* phase A offset filter */
    GDFLIB_FILTER_MA_T_A32      ui16FiltPhaB;                                   /* phase B offset filter */
    uint16_t                    ui16RsltNumPhaA;                                /* phase A channel number */
    uint16_t                    ui16RsltNumPhaB;                                /* phase B channel number */
    uint16_t                    ui16Sample0;                                    /* channel assigned to sample 0 */
    uint16_t                    ui16Sample8;                                    /* channel assigned to sample 8 */
    uint16_t                    ui16CalibPhaA;                                  /* phase A offset calibration */
    uint16_t                    ui16CalibPhaB;                                  /* phase B offset calibration */
    uint16_t                    ui16OffsetPhaA;                                 /* phase A offset result */
    uint16_t                    ui16OffsetPhaB;                                 /* phase B offset result */ 
}PHA_AB_T;

struct MCDRV_ADC12_T
{
    GMCLIB_3COOR_T_F16        * psIABC;                                         /* pointer to the 3-phase currents */
    PHA_BC_T                    sCurrSec16;                                     /* ADC setting for SVM sectors 1&6 */
    PHA_AC_T                    sCurrSec23;                                     /* ADC setting for SVM sectors 2&3 */
    PHA_AB_T                    sCurrSec45;                                     /* ADC setting for SVM sectors 4&5 */
                                                                                
    ADC_Type                  * pui32AdcBase;                                   /* pointer to ADC all quantities are measured */
    uint16_t                  * pui16SVMSector;                                 /* pointer to the SVM sector */ 
    frac16_t                  * pui16AuxChan;                                   /* pointer to auxiliary ADC channel number */
    frac16_t                  * pf16UDcBus;                                     /* pointer to DC Bus voltage variable */
                                                                                
    uint16_t                    ui16RsltNumDcBus;                               /* DC-bus voltage channel number */
    uint16_t                    ui16RsltNumAuxChan;                             /* auxiliary ADC channel number */
    
    uint16_t                    ui16OffsetFiltWindow;                           /* ADC Offset filter window */
};

struct MCDRV_ADC12_INIT_T
{
    uint16_t                  * ui16AdcArray;                                   /* pointer to ADC Array of channel configuration*/
    ADC_Type                  * pui32AdcBase;                                   /* pointer to ADC module */
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

bool_t MCDRV_Adc12Init(MCDRV_ADC12_T *this, MCDRV_ADC12_INIT_T *init);
bool_t MCDRV_Curr3Ph2ShGet(MCDRV_ADC12_T *this);
bool_t MCDRV_Curr3Ph2ShChanAssign(MCDRV_ADC12_T *this);  
bool_t MCDRV_Curr3Ph2ShCalibInit(MCDRV_ADC12_T *this);
bool_t MCDRV_Curr3Ph2ShCalib(MCDRV_ADC12_T *this);
bool_t MCDRV_Curr3Ph2ShCalibSet(MCDRV_ADC12_T *this);

bool_t MCDRV_VoltDcBusGet(MCDRV_ADC12_T *this);
bool_t MCDRV_AuxValGet(MCDRV_ADC12_T *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADC_ADC12_H_ */
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/