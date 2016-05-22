/**********************************************************************/
// File Name: {FM_project_loc}/../../../src/projects/hvp-kv46f/acim_appconfig.h 
//
// Date:  November 13, 2015, 15:29:1
//
// Automatically generated file for static configuration of the ACIM FOC application
/**********************************************************************/

#ifndef __M1_ACIM_APPCONFIG_H
#define __M1_ACIM_APPCONFIG_H


//Motor Parameters                      
//----------------------------------------------------------------------
//Pole-pair number                      = 2 [-]
//Stator resistance                     = 26.06 [Ohms]
//Rotor resistance                      = 24.85 [Ohms]
//Stator inductance                     = 0.51 [H]
//Rotor inductance                      = 0.51 [H]
//Mutual inductance                     = 0.47 [H]
//Drive inertia                         = 0.000016 [kg.m2]
//Nominal current                       = 0.9 [A]

#define M1_MOTOR_PP                        (2)
#define M1_R_S                             (26.06F)
#define M1_R_R                             (24.85F)
#define M1_L_S                             (0.51F)
#define M1_L_R                             (0.51F)
#define M1_L_M                             (0.47F)
#define M1_T_R                             M1_L_R / M1_R_R
#define M1_T_S                             M1_L_S / M1_R_S
#define M1_PI_CNST                         (3.141592653589793F)
#define M1_SAMPLE_TIME                     (0.0001F)

//Application scales                    
//----------------------------------------------------------------------
#define M1_I_MAX                           (8.0F)
#define M1_U_DCB_MAX                       (433.0F)
#define M1_U_MAX                           (250.0F)
#define M1_N_MAX                           (5500.0F)
#define M1_FREQ_MAX                        (183.0F)
#define M1_U_DCB_TRIP                      (346.4F)
#define M1_U_DCB_UNDERVOLTAGE              (40.0F)
#define M1_U_DCB_OVERVOLTAGE               (346.4F)
#define M1_N_OVERSPEED                     (4950.0F)
#define M1_N_OVERLOAD                      (250.0F)
#define M1_N_MIN                           (300.0F)
#define M1_N_NOM                           (5000.0F)
#define M1_I_PH_NOM                        (0.9F)
#define M1_SCALAR_UQ_MIN                   (11.5F)
//DCB Voltage Filter                    
#define M1_UDCB_IIR_B0                     (0.030459027951F)
#define M1_UDCB_IIR_B1                     (0.030459027951F)
#define M1_UDCB_IIR_A1                     (0.939081944097F)
//Application counters                  
#define M1_CALIB_DURATION                  (200)
#define M1_FAULT_DURATION                  (5000)
#define M1_FREEWHEEL_DURATION              (2000)

//Current Loop Control                  
//----------------------------------------------------------------------
//Loop Bandwidth                        = 150 [Hz]
//Loop Attenuation                      
//Loop sample time                      
//Required D-axis current               = 0.3 [A]
//----------------------------------------------------------------------
//Current Controller Output Limit       
#define M1_CLOOP_LIMIT                     (0.5196154845414393F)
//Required D-axis current               
#define M1_ID_REQ                          (0.3F)
//D-axis Controller - Parallel type     
#define M1_D_KP_GAIN                       (118.82286120084694F)
#define M1_D_KI_GAIN                       (6.8274439856947575F)
//Q-axis Controller - Parallel type     
#define M1_Q_KP_GAIN                       (118.82286120084694F)
#define M1_Q_KI_GAIN                       (6.8274439856947575F)

//Speed Loop Control                    
//----------------------------------------------------------------------
//Loop Bandwidth                        = 15 [Hz]
//Loop Attenuation                      
//Loop sample time                      
//----------------------------------------------------------------------
//Speed Controller - Parallel type      
#define M1_SPEED_PI_PROP_GAIN              (0.001F)
#define M1_SPEED_PI_INTEG_GAIN             (0.000005F)
#define M1_SPEED_LOOP_HIGH_LIMIT           (0.9F)
#define M1_SPEED_LOOP_LOW_LIMIT            (-0.9F)

#define M1_SPEED_ID_MIN_STARTUP            (0.95F)

#define M1_SPEED_RAMP_UP                   (3.0F)
#define M1_SPEED_RAMP_DOWN                 (3.0F)

#define M1_SPEED_LOOP_CNTR                 (10)
#define M1_SPEED_LOOP_FREQ                 (1000)

#define M1_SPEED_IIR_B0                    (0.003131753958F)
#define M1_SPEED_IIR_B1                    (0.003131753958F)
#define M1_SPEED_IIR_A1                    (0.993736492083F)

//Sensorless Closed Loop Flux Observer  
//----------------------------------------------------------------------
#define M1_RFO_COMP_KP_GAIN                (25.0F)
#define M1_RFO_COMP_KI_GAIN                (0.01F)
#define M1_RFO_KR_INV_GAIN                 (1.0851063829787235F)
#define M1_RFO_LS_TOT_LEAK_GAIN            (0.08340425531914897F)
#define M1_RFO_PSI_RA1_GAIN                (0.995151077592515F)
#define M1_RFO_PSI_RB1_GAIN                (0.002278993531517996F)
#define M1_RFO_PSI_SA1_GAIN                (0.9981185907806752F)
#define M1_RFO_PSI_SA2_GAIN                (0.00009981185907806752F)
#define M1_RFO_RS_EST                      (26.06F)
#define M1_MRAS_KP_GAIN                    (32750.0F)
#define M1_MRAS_KI_GAIN                    (12500.0F)
#define M1_MRAS_PSI_RA1                    (0.995151077592515F)
#define M1_MRAS_PSI_RB1                    (0.002278993531517996F)
#define M1_MRAS_TS                         (0.0001F)

//Control Structure Module - Scalar Control
//----------------------------------------------------------------------
#define M1_SCALAR_VHZ_FACTOR_GAIN          (0.10981691073340778F)
#define M1_SCALAR_INTEG_GAIN               (0.000020943951F)
#define M1_SCALAR_RAMP_UP                  (0.3F)
#define M1_SCALAR_RAMP_DOWN                (0.3F)

//Motor Identification Module           
//----------------------------------------------------------------------
#define M1_CHAR_NUMBER_OF_POINTS_BASE      (6)
#define M1_CHAR_CURRENT_POINT_NUMBERS      (65)
#define M1_TIME_50MS                       (500)
#define M1_TIME_100MS                      (1000)
#define M1_TIME_300MS                      (3000)
#define M1_TIME_600MS                      (6000)
#define M1_TIME_1200MS                     (12000)
#define M1_TIME_2400MS                     (24000)
#define M1_K_I_50MA                        (0.05F)

#endif

//End of generated file                 
/**********************************************************************/
