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
* @brief  Multiply subtract
* 
*******************************************************************************/
#ifndef _MLIB_MSU_F16_ASM_H_
#define _MLIB_MSU_F16_ASM_H_

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
#define MLIB_Msu_F16_Asmi(f16Accum, f16Mult1, f16Mult2) MLIB_Msu_F16_FAsmi(f16Accum, f16Mult1, f16Mult2)
#define MLIB_MsuSat_F16_Asmi(f16Accum, f16Mult1, f16Mult2) MLIB_MsuSat_F16_FAsmi(f16Accum, f16Mult1, f16Mult2)

/***************************************************************************//*!
*
* f16Out = f16Accum - ( f16Mult1 * f16Mult2)
* Without saturation
*******************************************************************************/
static inline frac16_t MLIB_Msu_F16_FAsmi(register frac16_t f16Accum, register frac16_t f16Mult1, register frac16_t f16Mult2)
{
	#if defined(__CC_ARM)                                             /* For ARM Compiler */
    	__asm volatile{ sxth f16Accum, f16Accum                       /* Transforms 16-bit input f16Val to 32-bit */
    		            smulbb f16Mult1, f16Mult1, f16Mult2           /* f16Mult1 * f16Mult2 */
                    	sub f16Accum, f16Accum, f16Mult1, asr #15 };  /* f16Accum = f16Accum - f16Mult1 * f16Mult2 */
	#else
    	__asm volatile( "sxth %0, %0 \n"                              /* Transforms 16-bit input f16Val to 32-bit */
                    	"smulbb %1, %1, %2 \n"                        /* f16Mult1 * f16Mult2 */
                    	"sub %0, %0, %1, asr #15 \n"                  /* f16Accum = f16Accum - f16Mult1 * f16Mult2 */
    					: "+l"(f16Accum), "+l"(f16Mult1): "l"(f16Mult2));
	#endif

    return f16Accum;
}

/***************************************************************************//*!
*
* f16Out = f16Accum - ( f16Mult1 * f16Mult2)
* With saturation
*******************************************************************************/
static inline frac16_t MLIB_MsuSat_F16_FAsmi(register frac16_t f16Accum, register frac16_t f16Mult1, register frac16_t f16Mult2)
{
    #if defined(__CC_ARM)                                         /* For ARM Compiler */
        __asm volatile{ sxth f16Accum, f16Accum                   /* Transforms 16-bit input f16Val to 32-bit */
        	            smulbb f16Mult1, f16Mult1, f16Mult2       /* f16Mult1 * f16Mult2 */
                        sub f16Accum, f16Accum, f16Mult1, asr #15 /* f16Accum = f16Accum - f16Mult1 * f16Mult2 */
                        ssat f16Accum, #16, f16Accum };           /* Saturation */
    #else
        __asm volatile( "sxth %0, %0 \n"                          /* Transforms 16-bit input f16Val to 32-bit */
                        "smulbb %1, %1, %2 \n"                    /* f16Mult1 * f16Mult2 */
                        "sub %0, %0, %1, asr #15 \n"              /* f16Accum = f16Accum - f16Mult1 * f16Mult2 */
                        "ssat %0, #16, %0 \n"                     /* Saturation */
                        : "+l"(f16Accum), "+l"(f16Mult1): "l"(f16Mult2));
    #endif

    return f16Accum;
}


#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_MSU_F16_ASM_H_ */
