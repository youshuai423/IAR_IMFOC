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
* @brief  Vector limit functions
* 
*******************************************************************************/
#ifndef _GFLIB_VECTORLIMIT_F32_H_
#define _GFLIB_VECTORLIMIT_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "gflib.h"  
 
/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_VectorLimit_F32_C(psVectorIn, f32Lim, psVectorOut)               \
        GFLIB_VectorLimit_F32_FC(psVectorIn, f32Lim, psVectorOut)
        
/*******************************************************************************
* Types
*******************************************************************************/   
typedef struct
{
    frac32_t f32A;   /* First argument */
    frac32_t f32B;   /* Second argument */
} GFLIB_VECTORLIMIT_T_F32;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern bool_t GFLIB_VectorLimit_F32_FC(const GFLIB_VECTORLIMIT_T_F32 *psVectorIn, 
                                       frac32_t f32Lim, 
                                       GFLIB_VECTORLIMIT_T_F32 *psVectorOut);
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_VECTORLIMIT_F32_H_ */
