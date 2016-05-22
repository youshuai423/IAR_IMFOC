/*******************************************************************************
*
* Copyright 2007-2015 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
* 
*
****************************************************************************//*!
*
* @brief Main AMCLIB header file for devices without FPU.  
* 
*******************************************************************************/
#ifndef _AMCLIB_H_
#define _AMCLIB_H_

#if defined(__cplusplus) 
extern "C" {
#endif  
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"    
#include "AMCLIB_AngleTrackObsrv_F32.h"  
#include "AMCLIB_PMSMBemfObsrvDQ_A32.h"
#include "AMCLIB_TrackObsrv_F32.h"   
#include "AMCLIB_ACIMFluxObsv_FLT.h"  
/*******************************************************************************
* Macros 
*******************************************************************************/      
#define AMCLIB_AngleTrackObsrvInit_F16(f16ThetaInit, psCtrl)                   \
        AMCLIB_AngleTrackObsrvInit_F16_C(f16ThetaInit, psCtrl)    
#define AMCLIB_AngleTrackObsrv_F16(f16ThetaErr, psCtrl)                        \
        AMCLIB_AngleTrackObsrv_F16_C(f16ThetaErr, psCtrl)       
#define AMCLIB_PMSMBemfObsrvDQ_F16(psIDQ, psUDQ, f16Speed, psCtrl)             \
        AMCLIB_PMSMBemfObsrvDQ_F16_Asm(psIDQ, psUDQ, f16Speed, psCtrl)
#define AMCLIB_PMSMBemfObsrvDQInit_F16(psCtrl)                                 \
        AMCLIB_PMSMBemfObsrvDQInit_F16_Ci(psCtrl)                      
#define AMCLIB_TrackObsrvInit_F16(f16ThetaInit, psCtrl)                        \
        AMCLIB_TrackObsrvInit_F16_Ci(f16ThetaInit, psCtrl)             
#define AMCLIB_TrackObsrv_F16(f16ThetaErr, psCtrl)                             \
        AMCLIB_TrackObsrv_F16_Asm(f16ThetaErr, psCtrl)
#define AMCLIB_RotFluxObsrv_A32ffa( psIsAlBe, sUSAlBe, acc32RotPos, psCtrl, sPsiRAlBe, psCtrlD)  \
        AMCLIB_RotFluxObsrv_A32ffa_C(psIsAlBe, sUSAlBe, acc32RotPos, psCtrl, sPsiRAlBe, psCtrlD)             
#define AMCLIB_MRAS_A32ffa( psIsAlBe, psPsiRAlBe, acc32RotPos,  psCtrl, psCtrlD)                 \
        AMCLIB_MRAS_A32ffa_C(psIsAlBe, psPsiRAlBe, acc32RotPos,  psCtrl, psCtrlD)                 
#define AMCLIB_MRASInit_A32ffa( psCtrl)                                                          \
        AMCLIB_MRASInit_A32ffa_Ci(psCtrl)                                                                      
#define AMCLIB_RotFluxObsrvInit_FLT( psCtrl)                                                     \
        AMCLIB_RotFluxObsrvInit_FLT_Ci(psCtrl) 
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_H_ */ 
