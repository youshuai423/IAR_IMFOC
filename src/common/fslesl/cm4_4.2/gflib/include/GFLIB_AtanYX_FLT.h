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
* @brief  Arcus tangent function based on the provided x,y coordinates as arguments
*         using division and piece-wise polynomial approximation
* 
*******************************************************************************/
#ifndef _GFLIB_ATANYX_FLT_H_
#define _GFLIB_ATANYX_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "GFLIB_Atan_FLT.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_AtanYX_FLT_C(fltY, fltX, pbErrFlag)                             \
        GFLIB_AtanYX_FLT_FC(fltY, fltX, pbErrFlag)                            
#define GFLIB_AtanYX_A32f_C(fltY, fltX, pbErrFlag)                            \
        GFLIB_AtanYX_A32f_FC(fltY, fltX, pbErrFlag)

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern float_t GFLIB_AtanYX_FLT_FC(float_t fltY, float_t fltX, 
                                   bool_t *pbErrFlag);
extern acc32_t GFLIB_AtanYX_A32f_FC(float_t fltY, float_t fltX, 
                                    bool_t *pbErrFlag);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_ATANYX_FLT_H_ */
