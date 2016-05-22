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
* @brief  Square root 
* 
*******************************************************************************/

#ifndef _GFLIB_SQRT_F16_H_
#define _GFLIB_SQRT_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_types.h"
#include "mlib.h"

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern frac16_t GFLIB_Sqrt_F16_FC(frac16_t f16Val);

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GFLIB_Sqrt_F16_C(f16Val) GFLIB_Sqrt_F16_FC(f16Val)


#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_SQRT_F16_H_ */

