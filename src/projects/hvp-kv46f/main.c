#include "main.h"
#include "app_init.h"
#include "math.h"

#define pi 3.1415926535898
#define digit 100000
#define period 4625  // 半周期时钟数
#define M 0.98  // 调制度

int period_count = 0;  // 载波周期数
int Tinv[3] = {0, 0, 0};  // 三相对应比较值
int last[3];  // 上周期Tinv值(for test)
double Dm = 0, Dn = 0, D0 = 0;  // 占空比

double roundn(double);  // 截断小数点后位数

void main(void)
{
    /* disable all interrupts before peripherals are initialized */
    __disable_irq();
    
    /* init application ports */  
    InitPorts();  
    PORT_WR_PCR_MUX(PORTB, 22, 1);                                              
    GPIO_SET_PDDR(PTB, 1<<22);

    /* initialize peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();      
    
    //Init_PIT();

    /* enable interrupts  */
    __enable_irq();

    /* infinite loop */
    while(1)
    {
    }
}

void PWMA_RELOAD0_IRQHandler(void)
{
    double Angle = 0;
    double theta = 0;
    int sector = 0;
    
    Angle = fmod((10 * pi * (period_count / 1000.0)), (2 * pi));
    theta = fmod(Angle,1/3.0 * pi);
    sector = floor( Angle / (1/3.0 * pi)) + 1;
    Dm = M * sin(1/3.0 * pi - theta) / 2.0;
    Dn = M * sin(theta) / 2.0;
    D0 = (0.5 - Dm - Dn) / 2.0;
    Dm = roundn(Dm);
    Dn = roundn(Dn);
    D0 = roundn(D0);
    if (D0 < 0) D0 = 0;
    
    switch (sector)
    {
    case 1:
      Tinv[0] = period - floor(period * (D0));
      Tinv[1] = period - floor(period * (D0 + Dm));
      Tinv[2] = period - floor(period * (D0 + Dm + Dn));
      break;
    case 2:
      Tinv[0] = period - floor(period * (D0 + Dn));
      Tinv[1] = period - floor(period * (D0));
      Tinv[2] = period - floor(period * (D0 + Dm + Dn));
      break;
    case 3:
      Tinv[0] = period - floor(period * (D0 + Dm + Dn));
      Tinv[1] = period - floor(period * (D0));
      Tinv[2] = period - floor(period * (D0 + Dm));
      break;
    case 4:
      Tinv[0] = period - floor(period * (D0 + Dm + Dn));
      Tinv[1] = period - floor(period * (D0 + Dn));
      Tinv[2] = period - floor(period * (D0));
      break;
    case 5:
      Tinv[0] = period - floor(period * (D0 + Dm));
      Tinv[1] = period - floor(period * (D0 + Dm + Dn));
      Tinv[2] = period - floor(period * (D0));
      break;
    case 6:
      Tinv[0] = period - floor(period * (D0));
      Tinv[1] = period - floor(period * (D0 + Dm + Dn));
      Tinv[2] = period - floor(period * (D0 + Dn));
    }
    
    PWM_WR_VAL2(PWMA, 0, -Tinv[0]);
    PWM_WR_VAL2(PWMA, 1, -Tinv[1]);
    PWM_WR_VAL2(PWMA, 2, -Tinv[2]);
    
    PWM_WR_VAL3(PWMA, 0, Tinv[0]);
    PWM_WR_VAL3(PWMA, 1, Tinv[1]);
    PWM_WR_VAL3(PWMA, 2, Tinv[2]);
    
    period_count++;
    if (period_count > 1000) 
    {
      period_count = 0;
    }

    last[0] = Tinv[0];
    last[1] = Tinv[1];
    last[2] = Tinv[2];
    
    PWM_WR_STS_RF(PWMA, 0, TRUE);
    /* start PWMs (set load OK flags and run) */
    PWM_WR_MCTRL_LDOK(PWMA, TRUE);
 }

void PWMA_RERR_IRQHandler(void)
{
  GPIO_WR_PSOR(PTB, 1<<22);
}

void PIT0_IRQHandler(void)
{
  PIT_WR_TFLG_TIF(PIT, 0, 1);
  GPIO_WR_PTOR(PTD, 1<<0);
  GPIO_WR_PTOR(PTB, 1<<22);
}
double roundn(double input)
{
  double temp;
  temp = input * digit;
  temp = floor(temp);
  temp = temp / digit;
  return temp;
}