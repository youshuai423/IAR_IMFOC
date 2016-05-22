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
#ifndef _MLIB_SHIFT_F32_H_
#define _MLIB_SHIFT_F32_H_

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
#define MLIB_Sh1L_F32_Ci(f32Val)          MLIB_Sh1L_F32_FCi(f32Val) 
#define MLIB_Sh1R_F32_Ci(f32Val)          MLIB_Sh1R_F32_FCi(f32Val)   
#define MLIB_ShL_F32_Ci(f32Val, u16Sh)    MLIB_ShL_F32_FCi(f32Val, u16Sh) 
#define MLIB_ShR_F32_Ci(f32Val, u16Sh)    MLIB_ShR_F32_FCi(f32Val, u16Sh)  

/***************************************************************************//*!
*
* This function returns the f32Val input shifted by 1 to the left. 
* The function does not saturate the output.
*              
*******************************************************************************/  
static inline frac32_t MLIB_Sh1L_F32_FCi(register frac32_t f32Val)
{
    return((frac32_t)(f32Val << 1));
}

/***************************************************************************//*!
*
* This function returns the f32Val input shifted by 1 to the right. 
*              
*******************************************************************************/
static inline frac32_t MLIB_Sh1R_F32_FCi(register frac32_t f32Val)
{
    return((frac32_t)(f32Val >> 1));
}

/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of u16Sh to the left. 
* The function does not saturate the output. 
*             
*******************************************************************************/
static inline frac32_t MLIB_ShL_F32_FCi(register frac32_t f32Val, register uint16_t u16Sh)
{
    return((frac32_t)(f32Val << u16Sh));
}

/***************************************************************************//*!
*
* This function returns the f32Val input shifted by the number of u16Sh to the right.  
*             
*******************************************************************************/ 
static inline frac32_t MLIB_ShR_F32_FCi(register frac32_t f32Val, register uint16_t u16Sh)
{
  return((frac32_t)(f32Val >> u16Sh));
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SHIFT_F32_H_ */
