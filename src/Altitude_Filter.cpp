#include "Altitude_Filter.h"


Altitude_Filter::Altitude_Filter(){
  X << 0,
       0,
       0;

  Z << 0,
       0;

  F << 1, 2, 0,
       0, 1, 2,
       0, 0, 1;

  H << 1, 0, 0,
       0, 0, 1;


  Q << 1, 0, 0,
       0, 1, 0,
       0, 0, 1;

  P << 1, 0, 0,
       0, 1, 0,
       0, 0, 1;

  R << 1, 0,
       0, 1;

}

void Altitude_Filter::update(Bmp_Data bmp_data, Bno_Data bno_data, Mma_Data mma_data){
  kalmanPredict();
  prefilter(bmp_data, bno_data, mma_data);
  kalmanUpdate();
}

float Altitude_Filter::getAltitude(){
  return X(0);
}

void Altitude_Filter::kalmanPredict(){
  X = F * X;
  P = F * P * F.transpose() + Q;
}

void Altitude_Filter::kalmanUpdate(){
  K = P * H.transpose() * (H * (P * H.transpose()) + R).inverse();
  X = X + K * (Z - H * X);
  P = (Matrix3f::Identity() - K * H) * P;
}

void Altitude_Filter::prefilter(Bmp_Data& bmp_data, Bno_Data& bno_data, Mma_Data& mma_data){
  Z(0) = (bmp_data.pressure1 + bmp_data.pressure2)/2;
  Z(1) = mma_data.y;
}
