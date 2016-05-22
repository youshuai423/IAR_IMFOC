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
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIV1Q_F16_H_
#define _MLIB_DIV1Q_F16_H_

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
#define MLIB_Div1Q_F16_Ci(f16Num, f16Denom)    MLIB_Div1Q_F16_FCi(f16Num, f16Denom) 
#define MLIB_Div1QSat_F16_Ci(f16Num, f16Denom) MLIB_Div1QSat_F16_FCi(f16Num, f16Denom)  

/***************************************************************************//*!
* @brief  16-bit inputs 16-output single quadrant division function
*
* @param  in frac16_t f16Num   - Numerator in <0;1) in frac16_t
*            frac16_t f16Denom - Denominator in <0;1) in frac16_t                      
*
* @return This function returns - frac16_t value <0;1)
*       
* @remarks  This function divides two non-negative fractional inputs:
*           result = f16Num / f16Denom.
*           The function does not saturate the output.
*           If the denominator is 0, the output is 0x7FFF.          
*
****************************************************************************/
static inline frac16_t MLIB_Div1Q_F16_FCi(register frac16_t f16Num, register frac16_t f16Denom)
{
    if (f16Denom == (frac16_t)0)
    {
        return((frac16_t)INT16_MAX);
    }
    else
    {
        return((frac16_t)((frac32_t)(f16Num << 15) / (frac32_t)f16Denom));
    }
}

/***************************************************************************//*!
* @brief  16-bit inputs 16-output single quadrant division function with saturation
*
* @param  in  frac16_t f16Num  - Numerator in <0;1) in frac16_t
*             frac16_t f16Denom- Denominator in <0;1) in frac16_t                      
*
* @return This function returns - frac16_t value <0;1)
*       
* @remarks  This function divides two non-negative fractional inputs:
*           result = f16Num / f16Denom.
*           The function saturates the output if f16Num > f16Denom
*           to 0x7FFF.
*
****************************************************************************/
static inline frac16_t MLIB_Div1QSat_F16_FCi(register frac16_t f16Num, register frac16_t f16Denom)
{
    if (f16Denom == (frac16_t)0)
    {
        return((frac16_t)INT16_MAX);
    }
    else
    {
        if (f16Num >= f16Denom)
        {
            return((frac16_t)INT16_MAX);
        }
        else
        {
            return(frac16_t)((frac32_t)(f16Num << 15) / (frac16_t)f16Denom);
        }
    }
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_DIV1Q_F16_H_ */ 
