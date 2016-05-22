/*******************************************************************************
*
* Copyright 2007-2014 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
* 
*
****************************************************************************//*!
*
* @brief  ACIM Flux Observer  
* 
*******************************************************************************/

#ifndef _AMCLIB_ACIMROTFLUXOBSERVER_FLT_H_
#define _AMCLIB_ACIMROTFLUXOBSERVER_FLT_H_

#if defined(__cplusplus) 
extern "C" {
#endif
  
/****************************************************************************
* Includes
****************************************************************************/   
#include "amclib_types.h" 
#include "gdflib_FP.h"  
#include "gflib_FP.h"     
#include "gmclib_FP.h"     
#include "mlib_FP.h"

/******************************************************************************
* Macros 
******************************************************************************/   
#define AMCLIB_RotFluxObsrv_A32ffa_C( psIsAlBe, sUSAlBe, acc32RotPos, psCtrl, sPsiRAlBe, psCtrlD) \
        AMCLIB_RotFluxObsrv_A32ffa_FC(psIsAlBe, sUSAlBe, acc32RotPos, psCtrl, sPsiRAlBe, psCtrlD)            
#define AMCLIB_MRAS_A32ffa_C( psIsAlBe, psPsiRAlBe, acc32RotPos,  psCtrl, psCtrlD)                \
        AMCLIB_MRAS_A32ffa_FC(psIsAlBe, psPsiRAlBe, acc32RotPos,  psCtrl, psCtrlD)                
#define AMCLIB_MRASInit_A32ffa_Ci( psCtrl)                                                        \
        AMCLIB_MRASInit_A32ffa_FCi(psCtrl)                                                                     
#define AMCLIB_RotFluxObsrvInit_FLT_Ci( psCtrl)                                                   \
        AMCLIB_RotFluxObsrvInit_FLT_FCi(psCtrl)              
            
/******************************************************************************
* Types 
******************************************************************************/     
typedef struct
{        
    struct  
    {    
        GMCLIB_2COOR_DQ_T_FLT sPsiRotRDQ;     /* Rotor flux state structure */   
        GMCLIB_2COOR_ALBE_T_FLT sPsiRotSAlBe; /* Stator flux state structure */
        GMCLIB_2COOR_ALBE_T_FLT sPsiStatSAlBe;/* Stator flux state structure */
        float_t  fltCompAlpha_1;              /* Integral part state variable for Alpha coefficient */                               
        float_t  fltCompBeta_1;               /* Integral part state variable for Beta coefficient */ 
        float_t  fltCompAlphaErr_1;           /* Error_1 state variable for Alpha coefficient */                               
        float_t  fltCompBetaErr_1;            /* Error_1 part state variable for Beta coefficient */ 
        float_t  fltPGain;                    /* Proportional gain Kp for stator model PI correction */                              
        float_t  fltIGain;                    /* Integration gain Ki for stator model PI correction */       
    } sCtrl;       
    
    float_t fltPsiRA1Gain;                    /* Constant determined by: Tau_r / (Tau_r+Ts) */                   
    float_t fltPsiRB1Gain;                    /* Constant determined by: Ts*Lm / (Tau_r+Ts) */                   
    float_t fltPsiSA1Gain;                    /* Constant determined by: Tau_1 / (Tau_r+Ts) */                   
    float_t fltPsiSA2Gain;                    /* Constant determined by: Tau_1*Ts / (Tau_r+Ts) */  
    float_t fltKrInvGain;                     /* Constant determined by: Lr / Lm */                              
    float_t fltKrLsTotLeakGain;               /* Constant determined by: Ls * (1-Lm*Lm/(Ls*Lr) */                           
    float_t fltRsEst;                         /* Estimated stator resistance [ohm] */                            
} AMCLIB_ROT_FLUX_OBSRV_T_FLT;    
 
typedef struct
{        
    struct
    {   
        GDFLIB_FILTER_IIR1_T_FLT fltSpeedIIR1Param;/* IIR1 filter structure for speed */       
        GMCLIB_2COOR_DQ_T_FLT sPsiRotRDQ;          /* Rotor flux state structure */ 
        float_t  fltSpeed_1;                       /* Integral part state variable for raw mechanical speed */  
        float_t  fltSpeedErr_1;                    /* Error_1 for raw mechanical speed controller */        
        float_t  fltPGain;                         /* MRAS proportional gain coefficient */                                              
        float_t  fltIGain;                         /* MRAS integral gain coefficient */ 
    } sCtrl;     
    float_t fltPsiRA1Gain;                         /* Constant determined by: Tau_r / (Tau_r+Ts) */                
    float_t fltPsiRB1Gain;                         /* Constant determined by: fltPsiRA1Gain * Ts * Lm / Tau_r */   
    float_t fltTs;                                                                  
} AMCLIB_MRAS_T_FLT;   

/*** temporary debug structures ***/
typedef struct
{        
    GMCLIB_2COOR_SINCOS_T_FLT sRotMod_sPos_sSCFluxR;
    GMCLIB_2COOR_DQ_T_FLT     sRotMod_sIS_sDQRotor;
    GMCLIB_2COOR_ALBE_T_FLT   sRotMod_sFluxR_sAlBe;
    GMCLIB_2COOR_ALBE_T_FLT   sCompensation_sError;
    GMCLIB_2COOR_ALBE_T_FLT   sCompensation;    
    GMCLIB_2COOR_ALBE_T_FLT   sCompensation_volt;
    GMCLIB_2COOR_ALBE_T_FLT   sStatMod_sUS_sAlBe;
    GMCLIB_2COOR_ALBE_T_FLT   sStatMod_sFluxS_sAlBe;
    GMCLIB_2COOR_ALBE_T_FLT   sStatMod_sFluxR_sAlBe;  

} AMCLIB_ROT_FLUX_OBSRV_DEBUG_T_FLT; 

typedef struct
{           
    GMCLIB_2COOR_SINCOS_T_FLT sPos_sSCRotor; 
    GMCLIB_2COOR_DQ_T_FLT     sRotMod_sIS_sDQRotor; 
    GMCLIB_2COOR_ALBE_T_FLT   sRotMod_sFluxR_sAlBe; 
    float_t                   fltFluxRError;
    float_t                   fltSpeed;
    float_t                   fltSpeedIIR1;
    float_t                   sPos_fltRotor;
                                                               
} AMCLIB_MRAS_DEBUG_FLT;   
   
/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern acc32_t AMCLIB_RotFluxObsrv_A32ffa_FC(const GMCLIB_2COOR_ALBE_T_FLT *psISAlBe,  
                                             const GMCLIB_2COOR_ALBE_T_FLT *psUSAlBe,  
                                             acc32_t acc32RotPos,                                             
                                             AMCLIB_ROT_FLUX_OBSRV_T_FLT *psCtrl,
                                             GMCLIB_2COOR_ALBE_T_FLT *psPsiRAlBe,
                                             AMCLIB_ROT_FLUX_OBSRV_DEBUG_T_FLT *psCtrlDebug);   

extern acc32_t AMCLIB_MRAS_A32ffa_FC(const GMCLIB_2COOR_ALBE_T_FLT *psISAlBe,    
                                     const GMCLIB_2COOR_ALBE_T_FLT *psPsiRAlBe,  
                                     acc32_t acc32RotPos,
                                     AMCLIB_MRAS_T_FLT *psCtrl,
                                     AMCLIB_MRAS_DEBUG_FLT *psCtrlDebug); 
                                     
/****************************************************************************
* Inline functions 
****************************************************************************/
                                     
/***************************************************************************//*!
* @brief  The function initializes the actual values of float AMCLIB_RotFluxObsrvAB function.
*
* @params:      ptr  AMCLIB_ROT_FLUX_OBSRV_T_FLT *psParam - Pointer to flux observer structure                
*
* @return       N/A
* 
*******************************************************************************/
static inline void AMCLIB_RotFluxObsrvInit_FLT_FCi(AMCLIB_ROT_FLUX_OBSRV_T_FLT *psCtrl)
{
    psCtrl->sCtrl.fltCompAlpha_1          = (float_t) 0.0F;
    psCtrl->sCtrl.fltCompBeta_1           = (float_t) 0.0F;
    psCtrl->sCtrl.fltCompAlphaErr_1       = (float_t) 0.0F;
    psCtrl->sCtrl.fltCompBetaErr_1        = (float_t) 0.0F; 
    psCtrl->sCtrl.sPsiRotRDQ.fltD         = (float_t) 0.0F;
    psCtrl->sCtrl.sPsiRotRDQ.fltQ         = (float_t) 0.0F;
    psCtrl->sCtrl.sPsiRotSAlBe.fltAlpha   = (float_t) 0.0F;
    psCtrl->sCtrl.sPsiRotSAlBe.fltBeta    = (float_t) 0.0F;     
    psCtrl->sCtrl.sPsiStatSAlBe.fltAlpha  = (float_t) 0.0F;
    psCtrl->sCtrl.sPsiStatSAlBe.fltBeta   = (float_t) 0.0F;
}

/***************************************************************************//*!
* @brief  The function initializes the actual values of float AMCLIB_MRAS_A32ffa function.
*
* @params:      ptr  AMCLIB_MRAS_AB_T_FLT *psParam - Pointer to MRAS structure                
*
* @return       N/A
* 
*******************************************************************************/
static inline void AMCLIB_MRASInit_A32ffa_FCi(AMCLIB_MRAS_T_FLT *psCtrl)
{
    GDFLIB_FilterIIR1Init_FLT (&psCtrl->sCtrl.fltSpeedIIR1Param);
    psCtrl->sCtrl.sPsiRotRDQ.fltD = (float_t) 0.0F;
    psCtrl->sCtrl.sPsiRotRDQ.fltQ = (float_t) 0.0F; 
    psCtrl->sCtrl.fltSpeed_1      = (float_t) 0.0F;    
    psCtrl->sCtrl.fltSpeedErr_1   = (float_t) 0.0F;
}
    
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_ACIMROTFLUXOBSERVER_FLT_H_ */
