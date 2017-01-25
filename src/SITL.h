#ifndef SITL_H
#define SITL_H

#include "WProgram.h"
#include "Flight_Configuration.h"
#include "Salt_Rev0.h"

#define FSTART       0xAA
#define FESENSE      0x01
#define FISOSENSE    0x02
#define FBNO         0x03
#define FMMA         0x04
#define FBMP         0x05

class SITL{
public:

  bool initialize();
  byte readESense();
  byte readIsoSense();
  Bmp_Data* readBMP();
  float readVbat();

private:
  void float2Bytes(float val, byte* bytes_array);
  float bytes2Float(byte* bytes_array);
  void request(byte select);
  byte requestMsg[5];
  elapsedMillis global_time;

};







#endif
