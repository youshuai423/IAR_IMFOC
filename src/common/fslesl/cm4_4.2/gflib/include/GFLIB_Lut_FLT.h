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
* @brief  Look-up table algorithm using linear interpolation
* 
*******************************************************************************/
#ifndef _GFLIB_LUT1D_FLT_H_
#define _GFLIB_LUT1D_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_Lut1D_FLT_C(fltX, pfltTable, psParam)                            \
        GFLIB_Lut1D_FLT_FC(fltX, pfltTable, psParam)  

/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    uint16_t u16TableSize;
    float_t fltMin;
    float_t fltMax;
}   GFLIB_LUT1D_T_FLT;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Lut1D_FLT_FC(float_t fltX, 
                                  const float_t *pfltTable, 
                                  const GFLIB_LUT1D_T_FLT *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LUT1D_FLT_H_ */
