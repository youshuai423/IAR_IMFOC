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
* @brief  Hysteresis function 
* 
*******************************************************************************/
#ifndef _GFLIB_HYST_FLT_H_
#define _GFLIB_HYST_FLT_H_

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
#define GFLIB_Hyst_FLT_Ci(fltVal, psParam) GFLIB_Hyst_FLT_FCi(fltVal, psParam) 
  
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    float_t fltHystOn;    /* Value determining the upper threshold */
    float_t fltHystOff;   /* Value determining the lower threshold */
    float_t fltOutValOn;  /* Value of the output when input is higher than the upper threshold */
    float_t fltOutValOff; /* Value of output when input is lower than lower threshold */
    float_t fltOutState;  /* Actual state of the output */
}   GFLIB_HYST_T_FLT;

/***************************************************************************//*!
*
* @brief     Hysteresis function
*
* @param     ptr  GFLIB_HYST_T_FLT *psParam
*                  - fltHystOn: Upper threshold
*                  - fltHystOff: Lower threshold
*                  - fltOutValOn: Output when input is above f16HystOn
*                  - fltOutValOff: Output when input is below f16HystOff
*                  - fltOutState: Output of the function
*
* @param     in  float_t fltVal
*
* @return    N/A
*
* @remarks   The function represents a hysteresis (or relay) function. The
*            function switches output between the two predefined values. When
*            the input is higher than upper threshold fltHystOn, the output
*            is high; when the input is below another (lower) threshold
*            fltHystOff, the output is low; when the input is between the two,
*            the output retains its value.
*
****************************************************************************/
static inline float_t GFLIB_Hyst_FLT_FCi(float_t fltVal, GFLIB_HYST_T_FLT *psParam)
{
    float_t InK_1;
    
    InK_1 = psParam->fltOutState;
    psParam->fltOutState = psParam->fltOutValOff;
    
    if(fltVal < psParam->fltHystOff) 
    {
        psParam->fltOutState = psParam->fltOutValOff;
    }
    else if(fltVal > psParam->fltHystOn)  
    {
        psParam->fltOutState = psParam->fltOutValOn;
    }
        else if ((fltVal < psParam->fltHystOn)&& (InK_1 ==  psParam->fltOutValOff))
        {
            psParam->fltOutState = psParam->fltOutValOff;
        }
            else if ((fltVal > psParam->fltHystOff)&& (InK_1 ==  psParam->fltOutValOn))
            {
                psParam->fltOutState = psParam->fltOutValOn;
            }
    return(psParam->fltOutState);
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_HYST_FLT_H_ */

