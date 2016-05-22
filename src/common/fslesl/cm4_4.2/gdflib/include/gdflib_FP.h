/*******************************************************************************
*
* Copyright 2007-2015 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
* 
****************************************************************************//*!
*
* @brief Main GDFLIB header file for devices with FPU. 
* 
*******************************************************************************/
#ifndef _GDFLIB_FP_H_
#define _GDFLIB_FP_H_

#if defined(__cplusplus)
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "gdflib.h"
#include "GDFLIB_FilterIIR1_FLT.h"
#include "GDFLIB_FilterIIR2_FLT.h"
#include "GDFLIB_FilterIIR3_FLT.h"
#include "GDFLIB_FilterIIR4_FLT.h"    
#include "GDFLIB_FilterMA_FLT.h"      

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterIIR1Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR1Init_FLT_Ci(psParam)
#define GDFLIB_FilterIIR1_FLT(f16InX, psParam)                                 \
        GDFLIB_FilterIIR1_FLT_Ci(f16InX, psParam)                              
#define GDFLIB_FilterIIR2Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR2Init_FLT_C(psParam)                                  
#define GDFLIB_FilterIIR2_FLT(f16InX, psParam)                                 \
        GDFLIB_FilterIIR2_FLT_C(f16InX, psParam) 
#define GDFLIB_FilterIIR3Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR3Init_FLT_C(psParam)                                  
#define GDFLIB_FilterIIR3_FLT(f16InX, psParam)                                 \
        GDFLIB_FilterIIR3_FLT_C(f16InX, psParam)                              
#define GDFLIB_FilterIIR4Init_FLT(psParam)                                     \
        GDFLIB_FilterIIR4Init_FLT_C(psParam)                                  
#define GDFLIB_FilterIIR4_FLT(f16InX, psParam)                                 \
        GDFLIB_FilterIIR4_FLT_C(f16InX, psParam)             
#define GDFLIB_FilterMAInit_FLT(f16InitVal, psParam)                           \
        GDFLIB_FilterMAInit_FLT_Ci(f16InitVal, psParam)                        
#define GDFLIB_FilterMA_FLT(f16InX, psParam)                                   \
        GDFLIB_FilterMA_FLT_Ci(f16InX, psParam)                 
            
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FP_H_ */
