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
* @brief Main AMCLIB header file for devices with FPU. 
* 
*******************************************************************************/
#ifndef _AMCLIB_FP_H_
#define _AMCLIB_FP_H_

#if defined(__cplusplus) 
extern "C" {
#endif  
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "amclib.h"    
#include "AMCLIB_AngleTrackObsrv_FLT.h"              
#include "AMCLIB_PMSMBemfObsrvDQ_FLT.h"
#include "AMCLIB_TrackObsrv_FLT.h"   

/*******************************************************************************
* Macros 
*******************************************************************************/      
#define AMCLIB_AngleTrackObsrv_A32ff(psSinCos, psCtrl)                         \
        AMCLIB_AngleTrackObsrv_A32ff_C(psSinCos, psCtrl)                           
#define AMCLIB_AngleTrackObsrvInit_A32(f16ThetaInit, psCtrl)                   \
        AMCLIB_AngleTrackObsrvInit_A32_C(f16ThetaInit, psCtrl)                              
#define AMCLIB_PMSMBemfObsrvDQ_A32fff(psIDQ, psUDQ, fltSpeed, psCtrl)          \
        AMCLIB_PMSMBemfObsrvDQ_A32fff_C(psIDQ, psUDQ, fltSpeed, psCtrl)       
#define AMCLIB_PMSMBemfObsrvDQInit_A32fff(psCtrl)                              \
        AMCLIB_PMSMBemfObsrvDQInit_A32fff_C(psCtrl)                                                                                 
#define AMCLIB_TrackObsrv_A32af(psSinCos, psCtrl)                              \
        AMCLIB_TrackObsrv_A32af_C(psSinCos, psCtrl)                                  
#define AMCLIB_TrackObsrvInit_A32af(a32ThetaInit, psCtrl)                      \
        AMCLIB_TrackObsrvInit_A32af_Ci(a32ThetaInit, psCtrl)
  
#if defined(__cplusplus) 
}
#endif

#endif /* _AMCLIB_FP_H_ */ 
