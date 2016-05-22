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
* @brief  Subtraction
* 
*******************************************************************************/
#ifndef _MLIB_SUB_F64_H_
#define _MLIB_SUB_F64_H_

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
#define MLIB_Sub_F64_Ci(f64Min, f64Sub)    MLIB_Sub_F64_FCi(f64Min, f64Sub) 
#define MLIB_SubSat_F64_Ci(f64Min, f64Sub) MLIB_SubSat_F64_FCi(f64Min, f64Sub)  
  
/***************************************************************************//*!
*
* f64Out = f64Min - f64Sub
* Without saturation
*******************************************************************************/ 
static inline frac64_t MLIB_Sub_F64_FCi(register frac64_t f64Min, register frac64_t f64Sub)
{
    return(f64Min - f64Sub);
}

/***************************************************************************//*!
*
* f64Out = f64Min - f64Sub
* With saturation
*******************************************************************************/
static inline frac64_t MLIB_SubSat_F64_FCi(register frac64_t f64Min, register frac64_t f64Sub)
{
    register int64_t i64Temp;
    register int64_t i64satmin, i64satmax;

    i64Temp = MLIB_Sub_F64_FCi(f64Min, f64Sub);
    i64satmax = (~f64Min & f64Sub) & i64Temp;
    i64satmin = f64Min & (~f64Sub) & (~i64Temp);

    i64Temp = (i64satmin < 0) ? INT64_MIN : i64Temp;
    i64Temp = (i64satmax < 0) ? INT64_MAX : i64Temp;
    return(i64Temp);
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_SUB_F64_H_ */ 
