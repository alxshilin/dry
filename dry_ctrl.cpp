#include "dry_ctrl.hpp"
#include <string>
#include <math.h>

DryCtrl::DryCtrl(float Tf, float Tv, float Tm, float Tmi){
  mTmi = Tmi;
  setParam(Tf, Tv, Tm);
  setCtrlParam();
}

DryCtrl::DryCtrl(){
  mTmi = 100.0 / 1000;
  mTf *= 1.0; mTv *= 0.5; mTm *= 5; // mTf in (0.1 .. 2.0); mTv in (0.1 .. 2); mTm in (0.7 ... 30)
  setCtrlParam();
}

void DryCtrl::setCtrlParam(){
  
  Kd  = mTmi / ( mTm * 2 );
  Kp  = mTv * Kd / ( 2.0 * mTm * 2);
  Kpf = mTf / ( 3.125 * 2 * mTm );
  Kdf = 1.25 * mTm * Kpf / mTf;
}

#define __KlFmn 0.0042
#define __KlFmx 0.52
#define __KlVmn 0.0042
#define __KlVmx 0.52
float lineTrans(float x, float x1, float x2, float y1, float y2){
  return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
}
void DryCtrl::UdateKalman(float Fi, float Vi){
  float uI2 = ((uI/100)*(uI/100));
  float kKFi = lineTrans(uI2, 0 , 100 , __KlFmn, __KlFmx);
  float kKVi = lineTrans(uI2, 0 , 100 , __KlVmn, __KlVmx);
  mFi += (Fi - mFi) * kKFi;
  mVi += (Vi - mVi) * kKVi;
}

float DryCtrl::updateCtrl(float dt, float setFi, float Fi, float Vi){
  UdateKalman(Fi, Vi);
  float erFi = setFi - mFi;
  float setVi = erFi * Kpf - mVi * Kdf;
  lTr = getTr();
  uI =  (setVi - mVi) * Kp - lTr * Kd;
  integr(dt, uI, mTmi, uM);
  //updateMdl(dt, uM);
  return uM;
}

float DryCtrl::GetUi() { return uI; }
