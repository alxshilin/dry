#ifndef __dryMdl_HPP
#define __dryMdl_HPP

float integr(float dt, float inp, float tm, float& out);
float intert(float dt, float inp, float k, float tm, float& out);
float rldiff(float dt, float dinp, float tdf, float tm, float& out);
float saturate(float min, float max, float& val);
float my_rand(float min, float max);


class DryMdl {
  float mTms = 0;
  char msg[32];
  float nFi;
  float nVi;

  float mExtrAirHmdy; /// влажность на улице
  float mTout;        /// температура на улице
  float mTcam;        /// температура в камере

  float mWoodHmdy;    /// ткущая влажность доски
  float mAirHmdy;     /// ткущая влажность djple[f]
  float mSetWoodDry;  /// влажность доски к которой она стремится, вычисляется из равновесной влажности
protected:
  float mTmWoodDry;   /// Постоянная времени процесса сушки пиломатериала  
  float mTmAir;       /// Постоянная времени перемешивания воздуха  
  float mTmWoodToAir; /// Постоянная времени передачи влаги в воздух в камере  

  float mTf;  /// Время интергирования от скорости до значения угла гиросокпа, и определяется его параметрами.
  float mTv;  /// ВРемя интергирования от приложеной силы на скорость, зависит от массы коптера и мощности двигателя
  float mTm;  /// время инреции частотного преобразователя двигателя 
  float mFi;  /// значение угла гиросокпа
  float mVi;  /// скорость изменения угла гиросокпа
  float mAc;  /// ускорение, которое пропорционально силе тяги

  float mTclmx; /// температура до которой максимальноможет нагрется камера
  float mTmCal; /// время нагрева каллорифера
  float mTmTCamToTWood; /// время нагрева доски от воздуха камеры
  float mTmTCalToTCam; /// время нагрева каммеры от каллорифекра
  float mTmTWoodToTCam; /// время нагрева воздуха камеры от доски 
  float mTmWoodToAirDry; /// время нагрева камеры от воздуха камеры
  float mText; ///  температура доски на улице

  float mTcal; /// температура каллорифера
  float mTWood; /// температура доски
  
public:
  DryMdl(float Tf, float Tv, float Tm);
  DryMdl();
  
  void setParam (float Tf, float Tv, float Tm);
  float getFi();
  float getVi();
  float getTr();
  //float updateMdl(float inp, float dt, float ul);
  //float updateMdl(float inp, float dt);
  float updateMdl(float dt, float inp, float Uht);
  char* print();
};

#endif /* __dryMdl_HPP */