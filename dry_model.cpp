#include "dry_model.hpp"
#include <string>
#include <math.h>

float integr(float dt, float inp, float tm, float& out){  out += inp * dt / tm;  return out;};

float intert(float dt, float inp, float k, float tm, float& out){  out += (inp * k - out) * dt / tm;  return out; };

float rldiff(float dt, float dinp, float tdf, float tm, float& out){ out += dinp * tdf/tm - out * dt / tm;  return out;};

float saturate(float min, float max, float& val){  val = (val < min)?min:(val>max?max:val); return val; }

float my_rand(float min, float max){ return ( rand() * (max - min)) / RAND_MAX + min; }

float fApproxAirHmdToWoodHmd(float mTcam, float mAirHmdy) {return 6;}
float fApproxWoodHmdToAirHmd(float mTcam, float mWoodHmdy){return 100;}
float fApproxExtAirHdmToExtAirHdm(float extH, float extT, float intT) {return 10;}
float fMix(float val1, float val2, float kmix) {return (val2 * (1.0-kmix) + val1 * kmix);}
float fkMixAir(float inp) {return inp*0.5;};


DryMdl::DryMdl(float Tf, float Tv, float Tm) { mTf = Tf; mTv = Tv; mTm = Tm; mFi = 0; mVi = 0; mAc = 0; };
DryMdl::DryMdl() {  
  mTf = 0.5; mTv = 300.0/32767; mTm = 0.03; mFi = 0; mVi = 0; mAc = 0;

  mTclmx = 90; /// температура до которой максимально может нагрется камера
  mTmCal = 0.1; /// время нагрева каллорифера
  mTmTCamToTWood = 1.5; /// время нагрева доски от воздуха камеры
  mTmTCalToTCam = 0.5; /// время нагрева каммеры от каллорифекра
  mTmTWoodToTCam = 0.15; /// время нагрева воздуха камеры от доски 
  mTmWoodToAirDry = 0.5; /// время нагрева камеры от воздуха камеры
  mText = -10; ///  температура доски на улице
  mTcam = -10;
  mTcal = -10;
  mTWood = -10;

};

void DryMdl::setParam (float Tf, float Tv, float Tm) { mTf = Tf; mTv = Tv; mTm = Tm;}
/*
float DryMdl::updateMdl(float dt, float inp, float ul) {
  nFi = 1000;  nVi = 4000;
  mTms += dt;
  intert(dt, inp, 1, mTm, mAc);
  integr(dt, mAc + 2000*sin(mTms*425) +  3*ul*sin(mTms*125), mTv, mVi);
  saturate(-32767,32767, mVi);
  integr(dt, mVi, mTf, mFi);
  saturate(-32767,32767, mFi);
  return mFi;
};
*/
float DryMdl::updateMdl(float dt, float inp, float Uht) {
  // процесс нагрева каллорифера, при полной мощности котла (Uht = 1.0) стремится к mTclmx
  intert(dt, mTcam + (mTclmx - mTcam)*Uht, 1.0, mTmCal, mTcal);
  // каллорифер передает тепло в камеру с постойнной веремени mTmTCalToTCam
  intert(dt, mTcal, 1.0, mTmTCalToTCam, mTcam);
  // температура воздуха в камере также зависит от обмена теплом с доской
  intert(dt, mTWood, 1.0, mTmTWoodToTCam, mTcam);
  // температура воздуха в камере также зависит внешней температуры  и степени открытия заслонки 
  intert(dt, fMix(mText, mTcam, fkMixAir(inp) ), 1.0, mTmWoodToAirDry, mTcam);

  // температура доски в камере зависит температуры камеры постоянная времени занчительно зависит от влажности самой доски
  intert(dt, mTcam, 1.0, mTmTCamToTWood, mTWood);

  /* степень открытия определяется контроллером */
  /* смешение воздуха зависит от степени открытия заслонки */

  /* температура контура зависит от процесса смешения возхдуха и обмена теплом с древесиной и нагр. коллориферами */
  /* Коэффициент смешения зависит от степени открытия воздушной заслонки, 
    объема циркуляции воздуха и конструкции камеры */
  float Kopen = 0.1;
  /* Влажность воздуха зависит от коэффициента смешения с внешним воздухом 
  и процессом передачи влажности от древисины в воздух */
  float mSetAirHmdy = fApproxWoodHmdToAirHmd(mTcam, mWoodHmdy);
  mSetAirHmdy = mSetAirHmdy * (1 - Kopen) + fApproxExtAirHdmToExtAirHdm(mExtrAirHmdy, mTout, mTcam) * Kopen;
  intert(dt, mSetAirHmdy, 1.0, mTmWoodDry, mAirHmdy);
  //! функция пересчета в равновесную влажность, зависит от влажности воздуха и температуры в помещении
  mSetWoodDry = fApproxAirHmdToWoodHmd(mTcam, mAirHmdy); 
  intert(dt, mSetWoodDry, 1.0, mTmWoodDry, mWoodHmdy);





  return mFi;
};

char* DryMdl::print() {
  sprintf(msg, " %f %f %f", mTWood, mTcam, mTcal);
  return msg;
};

float DryMdl::getFi() {return mFi + my_rand(-nFi, nFi);}
float DryMdl::getVi() {return mVi + my_rand(-nVi, nVi);}
float DryMdl::getTr() {return mAc;}

