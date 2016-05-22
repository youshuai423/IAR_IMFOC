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
* @brief Main GMCLIB header file for devices with FPU. 
* 
*******************************************************************************/
#ifndef _GMCLIB_FP_H_
#define _GMCLIB_FP_H_

#if defined(__cplusplus) 
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "GMCLIB_Clark_FLT.h" 
#include "GMCLIB_Decoupling_FLT.h"       
#include "GMCLIB_ElimDcBusRip_FLT.h"  
#include "GMCLIB_Park_FLT.h"    

/*******************************************************************************
* Macros 
*******************************************************************************/                                                                                    
#define GMCLIB_Clark_FLT(psIn, psOut)                                                 \
        GMCLIB_Clark_FLT_Ci(psIn, psOut)                                              
#define GMCLIB_ClarkInv_FLT(psIn, psOut)                                              \
        GMCLIB_ClarkInv_FLT_Ci(psIn, psOut)                                           
#define GMCLIB_DecouplingPMSM_FLT(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)        \
        GMCLIB_DecouplingPMSM_FLT_C(psUDQ, psIDQ, fltSpeedEl, psParam, psUDQDec)    
#define GMCLIB_ElimDcBusRip_F16fff(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp)        \
        GMCLIB_ElimDcBusRip_F16fff_C(fltUDCBus, fltIdxMod, psUAlBe, psUAlBeComp) 	
#define GMCLIB_ElimDcBusRipFOC_F16ff(fltUDCBus, psUAlBe, psUAlBeComp)                 \
        GMCLIB_ElimDcBusRipFOC_F16ff_C(fltUDCBus, psUAlBe, psUAlBeComp) 			  
#define GMCLIB_Park_FLT(psIn, psAnglePos, psOut)                                      \
        GMCLIB_Park_FLT_Ci(psIn, psAnglePos, psOut)                                   
#define GMCLIB_ParkInv_FLT(psIn, psAnglePos, psOut)                                   \
        GMCLIB_ParkInv_FLT_Ci(psIn, psAnglePos, psOut)                                

#if defined(__cplusplus) 
}
#endif 

#endif /* _GMCLIB_FP_H_ */
