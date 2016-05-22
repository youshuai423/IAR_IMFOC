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
* @file      MID_def.h
*
* @brief     Common header file for measurement routines
*
******************************************************************************/
#ifndef _MID_DEF_H_
#define _MID_DEF_H_

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/
#include "m1_acim_appconfig.h"
#include "mlib_FP.h"

/******************************************************************************
| constants
|----------------------------------------------------------------------------*/
/* MID Fault numbers */
#define MID_FAULT_NO_MOTOR              (1)
#define MID_FAULT_TOO_HIGH_RS           (2)
#define MID_FAULT_DC_CUR_NOT_REACHED    (4)
#define MID_FAULT_RS_OUT_OF_RANGE       (8)
#define MID_FAULT_U_RMS_NOT_REACHED     (16)
#define MID_FAULT_I_RMS_NOT_REACHED     (32)

/* Current controllers' coefficients ensuring slow response for variable parameters */
#define MID_KP_GAIN                     (1.0F)
#define MID_KI_GAIN                     (0.1F)

#define MID_IRMS_KP_GAIN                (1.0F)
#define MID_IRMS_KI_GAIN                (0.1F)

/******************************************************************************
| constants
|----------------------------------------------------------------------------*/

/******************************************************************************
| macros
|----------------------------------------------------------------------------*/

/******************************************************************************
| types
|----------------------------------------------------------------------------*/

/******************************************************************************
| exported variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| exported functions
|----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/******************************************************************************
| inline functions
|----------------------------------------------------------------------------*/

#endif /* _MID_DEF_H_ */
