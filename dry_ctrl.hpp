#ifndef __DryCtrl_HPP
#define __DryCtrl_HPP

#include "dry_model.hpp"

class DryCtrl : public DryMdl {
  float loldVi, uI, lTr, uM;
  float Kpf, Kdf, Kp, Kd, mTmi;
  void UdateKalman(float Fi, float Vi);
  void setCtrlParam();
public:
  DryCtrl(float Tf, float Tv, float Tm, float Tmi);
  DryCtrl();
  float updateCtrl(float dt, float setFi, float Fi, float Vi);
  float GetUi();
};

#endif /* __DryCtrl_HPP */