/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "gflib_FP.h"
#include "gmclib_FP.h"
#include "gdflib_FP.h"
#include "amclib_FP.h"
#include "mlib_FP.h"
#include "mlib_types.h"
#include "arm_math.h"
#include <math.h>
#include "m1_acim_appconfig.h"

/******************************************************************************
| constants
|----------------------------------------------------------------------------*/

/******************************************************************************
| types
|----------------------------------------------------------------------------*/
/* RFO structure */
typedef struct
{
    float_t                             sFlux;  
    float_t                             fltErrorFlux;                          /* rotor flux error [rpm] */
    GFLIB_CTRL_PI_P_AW_T_FLT            sFluxPiParamsGFLIB;
    bool_t                              bFluxPISatFlag;                        /* rotor flux PI controller saturation flag */
} MCS_ACIM_RFO_A2_T;

/* speed structure */
typedef struct
{
    GFLIB_CTRL_PI_P_AW_T_FLT            sSpeedPiParamsGFLIB;                    /* speed controller structure */
    GFLIB_RAMP_T_FLT                    sSpeedRampParams;                       /* speed ramp structure */
    float_t                             fltFilt;                                /* filtered mechanical speed value [rpm] */
    float_t                             fltRamp;                                /* ramped mechanical speed [rpm] */
    float_t                             fltCmd;                                 /* speed command [rpm] */
    float_t                             fltErrorSpeed;                          /* speed error [rpm] */
    float_t                             fltIdStartup;                           /* excitation current at speed FOC startup [A] */
    float_t                             fltIdStartupMinPct;                     /* d-axis current threshold to go spin state at speed FOC startup [%] */
    uint16_t                            ui16LoopCounter;                        /* loop counter */
    bool_t                              bSpeedPISatFlag;                        /* speed PI controller saturation flag */
} MCS_SPEED_A1_T;

/* ACIM FOC structure */
typedef struct
{
    MCS_ACIM_RFO_A2_T                   sRFO;                                   /* Rotor Flux Observer structure */
    MCS_SPEED_A1_T                      sSpeed;                                 /* Speed struct */
    GMCLIB_3COOR_T_F16                  sIABCMeas;                              /* measured 3-phase current (FRAC16) */
    GMCLIB_3COOR_T_FLT                  sIABC;                                  /* measured 3-phase current [A] */
    GMCLIB_2COOR_ALBE_T_FLT             sIAlBe;                                 /* alpha/Beta current [A] */
    GMCLIB_2COOR_DQ_T_FLT               sIDQ;                                   /* DQ current in rotor flux coordinates [A] */    
    GMCLIB_2COOR_DQ_T_FLT               sIDQReq;                                /* required DQ current in rotor flux coordinates [A] */  
    GMCLIB_2COOR_DQ_T_FLT               sIDQError;                              /* DQ current error in rotor flux coordinates [A] */  
    GMCLIB_3COOR_T_F16                  sDutyABC;                               /* applied duty cycles ABC (FRAC16) */
    GMCLIB_2COOR_ALBE_T_FLT             sUAlBe;                                 /* Alpha/Beta stator voltage */
    GMCLIB_2COOR_DQ_T_FLT               sUDQReq;                                /* required DQ voltage in rotor flux coordinates [V] */  
    GMCLIB_2COOR_SINCOS_T_FLT           sSCFOC;                                 /* sine & cosine of rotor flux angle [-] */
    float_t                             fltDutyCycleLimit;                      /* max. allowable duty cycle [-] */
    float_t                             fltUDcBus;                              /* DC-bus voltage [V] */
    float_t                             fltUDcBusFilt;                          /* filtered DC-bus voltage [V] */
    frac16_t                            f16UDcBusMeas;                          /* DC-bus voltage (FRAC16) */
    uint16_t                            ui16SectorSVM;                          /* SVM sector (sector) */     
    bool_t                              bSpeedStartup;                          /* motor startup flag */
} MCS_ACIM_FOC_A1_T;
/******************************************************************************
| local functions prototypes
|----------------------------------------------------------------------------*/
extern void MCS_ACIMFocCtrlCurrentA1(MCS_ACIM_FOC_A1_T *psFocACIM);
double RotFluxObserve(double ism, double lamdar);
double SlipFreq(double lamdar, double ist);
double integrator(double sum, double current);

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/