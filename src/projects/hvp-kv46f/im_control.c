/******************************************************************************
| includes                          
|----------------------------------------------------------------------------*/
#include "im_control.h"

/******************************************************************************
| constants                         
|----------------------------------------------------------------------------*/

/******************************************************************************
| macros                          
|----------------------------------------------------------------------------*/

/******************************************************************************
| types                         
|----------------------------------------------------------------------------*/

/******************************************************************************
| functions                         
|----------------------------------------------------------------------------*/
void MCS_ACIMFocCtrlCurrentA1(MCS_ACIM_FOC_A1_T *psFocACIM)
{
  double theta;
  
  /* 转速磁链PI初始化 */
  GFLIB_CtrlPIpAWInit_FLT_FCi(0,psFocACIM->sRFO.sFluxPiParamsGFLIB); 
  GFLIB_CtrlPIpAWInit_FLT_FCi(0,psFocACIM->sSpeed.sSpeedPiParamsGFLIB);
  
  /* 转子磁链观测 */
  GMCLIB_Clark_FLT(psFocACIM->sIABC, psFocACIM->sIAlBe); 
  GMCLIB_Park_FLT(psFocACIM->sIAlBe, psFocACIM->sSCFOC, psFocACIM->sIDQ); 
  psFocACIM->sRFO.sFlux = RotFluxObserve(sIDQ->fltD, lamdar);
  
  /* 转子位置角估测 */
  theta = RotPosition(psFocACIM->sRFO.sFlux, psFocACIM->sIDQ.fltQ);
  psFocACIM->sSCFOC.fltSin = GFLIB_Sin_FLT(theta); 
  psFocACIM->sSCFOC.fltCos = GFLIB_Cos_FLT(theta); 
  
  /* 电流给定值生成 */
  GFLIB_CtrlPIpAW_FLT_FC(psFocACIM->sRFO.fltErrorFlux,                          // 转速PI
                         psFocACIM->sRFO.bFluxPISatFlag,
                         psFocACIM->sRFO.sFluxPiParamsGFLIB);                   
  GFLIB_CtrlPIpAW_FLT_FC(psFocACIM->sSpeed.fltErrorSpeed,                       // 磁链PI
                         psFocACIM->sSpeed.bSpeedPISatFlag,
                         psFocACIM->sSpeed.sSpeedPiParamsGFLIB);                
  psFocACIM->sIDQReq.fltD = psFocACIM->sRFO.sFluxPiParamsGFLIB->fltIAccK_1;     // ism给定值
  psFocACIM->sIDQReq.fltQ = psFocACIM->sSpeed.sSpeedPiParamsGFLIB->fltIAccK_1 
                            * M1_L_R / (M1_MOTOR_PP * M1_L_M) / lamda;          // ist给定值
  GMCLIB_ParkInv_FLT(sIDQReq, sSCFOC, sIAlBeReq);                               // 电流给定值PARK逆变换
  GMCLIB_ClarkInv_FLT(sIAlBeReq, sIABCReq);                                     // 电流给定值CLARK逆变换
  
  /* 电流滞环输出控制信号 */
  if (sIABCReq.fltA > sIABC.fltA)
    // A相上桥臂（死区？）
  else
    // A相下桥臂
    
  if (sIABCReq.fltB > sIABC.fltB)
    // B相上桥臂（死区？）
  else
    // B相下桥臂
    
  if (sIABCReq.fltB > sIABC.fltB)
    // C相上桥臂（死区？）
  else
    // C相下桥臂
}

double RotFluxObserve(double ism, double lamdar)
{  
  lamdar = (1 - M1_SAMPLE_TIME / M1_T_R) * lamdar + M1_L_M * M1_SAMPLE_TIME / M1_T_R * ism;
  
  return lamdar;
}

double SlipFreq(double lamdar, double ist)
{
  double wr = 0;
  
  if(lamdar > 1e-5)
    wr = ist * M1_L_M / M1_T_R / lamdar;
  else
    wr = 0;
  
  return wr;
}

double integrator(double sum, double current)
{
  sum += current * M1_SAMPLE_TIME;
  return sum;
}