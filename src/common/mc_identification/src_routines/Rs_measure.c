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
*
* @file     Rs_measure.c
*
* @brief    Stator resistance measurement routine
*
***************************************************************************//*!*/

/******************************************************************************
| includes
|----------------------------------------------------------------------------*/

#include "rs_measure.h"

/******************************************************************************
| global variables
|----------------------------------------------------------------------------*/
/*Default transfer characteristics LookUp for FSL HV PowerStage and 325V DCbus*/
float_t	fltTransferCharError[] =
{
-1859, -1857, -1869, -1882, -1883, -1887, -1878, -1868, -1865, -1870,
-1873, -1865, -1869, -1862, -1860, -1846, -1847, -1835, -1823, -1816,
-1816, -1796, -1783, -1758, -1736, -1713, -1709, -1663, -1594, -1494,
-1373, -1063, -302,   275,   1106,  1459,  1547,  1635,  1705,  1722,
 1731,  1744,  1783,  1804,  1823,  1821,  1835,  1836,  1847,  1865,
 1869,  1875,  1866,  1864,  1869,  1878,  1899,  1899,  1904,  1891,
 1886,  1890,  1896,  1908,  1909
};

/******************************************************************************
| constant and macros
|----------------------------------------------------------------------------*/

/******************************************************************************
| local types
|----------------------------------------------------------------------------*/

/******************************************************************************
| local function prototypes
|----------------------------------------------------------------------------*/

/******************************************************************************
| local variables
|----------------------------------------------------------------------------*/

/******************************************************************************
| global functions
|----------------------------------------------------------------------------*/

/*******************************************************************************
 * Implementation variant: Kinetis and FSL ESL Kinetis libraries
 ******************************************************************************/
/******************************************************************************
@brief        Rs estimation function

@param[in]    *sRsMeasFcn   input structure of type #MID_GET_RS_A1_T for passing
              all necessary parameters.

@return       none
******************************************************************************/
void MID_getRs(MID_GET_RS_A1_T* sRsMeasFcn)
{
    float_t              fltUdCorrect;

    /* Initialization */
    if(sRsMeasFcn->ui16Active == 0)
    {
        sRsMeasFcn->ui16Active              = TRUE;
        sRsMeasFcn->ui16LoopCounter         = 0;
        sRsMeasFcn->fltRs                   = FRAC16(0.0);

        /* Set the measuring current Id_meas*/
        *(sRsMeasFcn->pfltIdReq) = sRsMeasFcn->fltIdMeas;
    }

    /* LoopCounter for time keeping */
    sRsMeasFcn->ui16LoopCounter++;

    /* After 600ms start calculation */
    if(sRsMeasFcn->ui16LoopCounter == M1_TIME_1200MS)
    {
        /* Set required current to zero */
        *(sRsMeasFcn->pfltIdReq) = 0.0F;

        /* Calculate corrected voltage as a subtraction of the required voltage and the compensation voltage from the LUT */
        fltUdCorrect = (*(sRsMeasFcn->pfltUdReq)) - GFLIB_Lut1D_FLT(*(sRsMeasFcn->pfltIdfbck), sRsMeasFcn->pfltUdErrorLookUp, &sRsMeasFcn->sLUTparams);

        /* Return frac16_t inductance */
        sRsMeasFcn->fltRs = fltUdCorrect / (*(sRsMeasFcn->pfltIdfbck));

        /* Set Id_req to zero */
        *(sRsMeasFcn->pfltIdReq) = 0.0F;

        /* Check Faults */
        /* Check if motor is connected */
        if(MLIB_Abs_FLT(*(sRsMeasFcn->pfltIdfbck)) < M1_K_I_50MA)
            ui16FaultMID |= MID_FAULT_NO_MOTOR;

        /* Check if Rs is negative or saturated*/
        if(sRsMeasFcn->fltRs < 0.0F || sRsMeasFcn->fltRs == 0.0F)
            ui16FaultMID |= MID_FAULT_RS_OUT_OF_RANGE;

        /* Check if measuring current was reached */
        if(*(sRsMeasFcn->pfltIdfbck) < (sRsMeasFcn->fltIdMeas - M1_K_I_50MA))
            ui16FaultMID |= MID_FAULT_DC_CUR_NOT_REACHED;
    }

    /* Wait additional 600ms to stabilize Id at 0A */
    /* Exit the function after 1200ms */
    if(sRsMeasFcn->ui16LoopCounter > M1_TIME_2400MS)
    {
        sRsMeasFcn->ui16Active = FALSE;
    }
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/