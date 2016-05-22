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
* @brief  Addition
* 
*******************************************************************************/
#ifndef _MLIB_ADD_A32_H_
#define _MLIB_ADD_A32_H_

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
#define MLIB_Add_A32ss_Ci(f16Add1, f16Add2) MLIB_Add_A32ss_FCi(f16Add1, f16Add2) 
#define MLIB_Add_A32as_Ci(a32Accum, f16Add) MLIB_Add_A32as_FCi(a32Accum, f16Add)  

/***************************************************************************//*!
*  Addition        
*  a32Out = f16Add1 + f16Add2
*  Result can overflow throught fractional range and is stored in accumulator type
*******************************************************************************/
static inline acc32_t MLIB_Add_A32ss_FCi(register frac16_t f16Add1, register frac16_t f16Add2)
{ 
    return((acc32_t)((acc32_t)f16Add1 + (acc32_t)f16Add2));     
}

/***************************************************************************//*!
*  Addition        
*  a32Out = a32Accum + f16Add
*  Result can overflow throught fractional range and is stored in accumulator type
*******************************************************************************/
static inline acc32_t MLIB_Add_A32as_FCi(register acc32_t a32Accum, register frac16_t f16Add)
{  
    return((acc32_t)(a32Accum + (acc32_t)(f16Add)));        
}

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_ADD_A32_H_*/
