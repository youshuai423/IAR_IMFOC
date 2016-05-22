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
* @brief  Shift
* 
*******************************************************************************/
#ifndef _MLIB_SHIFT_F16_H_
#define _MLIB_SHIFT_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MLIB_Sh1L_F16_Ci(f16Val)          MLIB_Sh1L_F16_FCi(f16Val)
#define MLIB_Sh1R_F16_Ci(f16Val)          MLIB_Sh1R_F16_FCi(f16Val)  
#define MLIB_Sh1R_F16_Ci(f16Val)          MLIB_Sh1R_F16_FCi(f16Val)  
#define MLIB_ShL_F16_Ci(f16Val, u16Sh)    MLIB_ShL_F16_FCi(f16Val, u16Sh)
#define MLIB_ShR_F16_Ci(f16Val, u16Sh)    MLIB_ShR_F16_FCi(f16Val, u16Sh)  
  
/***************************************************************************//*!
*
* This function returns the f16Val input shifted by 1 to the left. 
* The function does not saturate the output.
*              
*******************************************************************************/ 
static inline frac16_t MLIB_Sh1L_F16_FCi(register frac16_t f16Val)
{
    return((frac16_t)(f16Val << 1));
}

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by 1 to the right. 
*              
*******************************************************************************/
static inline frac16_t MLIB_Sh1R_F16_FCi(register frac16_t f16Val)
{
    return((frac16_t)(f16Val >> 1)); 
}

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of u16Sh to the left. 
* The function does not saturate the output. 
*             
*******************************************************************************/ 
static inline frac16_t MLIB_ShL_F16_FCi(register frac16_t f16Val, register uint16_t u16Sh)
{
    return((frac16_t)(f16Val << u16Sh));
}

/***************************************************************************//*!
*
* This function returns the f16Val input shifted by the number of u16Sh to the right.  
*             
*******************************************************************************/ 
static inline frac16_t MLIB_ShR_F16_FCi(register frac16_t f16Val, register uint16_t u16Sh)
{
    return((frac16_t)(f16Val >> u16Sh));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SHIFT_F16_H_ */
