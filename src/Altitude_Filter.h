/*
This file defines the class for the altitude kalman filter. Not implemented yet.

*/
#ifndef ALTITUDE_FILTER_H
#define ALTITUDE_FILTER_H

#include <WProgram.h>
#include <Eigen30.h>
#include <Eigen/Dense>
#include "Flight_Configuration.h"

//configuration options
#define VAR_BMP 0.5
#define VAR_BNO 0.5
#define VAR_MMA 0.5
using namespace Eigen;
class Altitude_Filter{
public:
  Altitude_Filter();
  void update(Bmp_Data bmp_data, Bno_Data bno_data, Mma_Data mma_data);
  float getAltitude();

private:

  Vector3f X;
  Vector2f Z;
  Matrix3f F;
  Matrix3f Q;
  Matrix3f P;
  Matrix<float, 3, 2> K;
  Matrix<float, 2, 3> H;
  Matrix2f R;

  void prefilter(Bmp_Data& bmp_data, Bno_Data& bno_data, Mma_Data& mma_data);
  void kalmanPredict();
  void kalmanUpdate();
};

#endif
