#include "SITL.h"


/*Sensors*/


bool SITL::initialize(){
  bool sucessful = true;
  requestMsg[0] = FSTART;
  //holds until connection with PC is established
  while(true){
    Serial.write(FSTART);
    delay(100);
    if(Serial.available()){
      if(Serial.read() == FSTART){
        return sucessful;
      }
    }
  }
}

byte SITL::readESense(){
  return 0;
}

byte SITL::readIsoSense(){
  return 0;
}

Bmp_Data SITL::readBMP(){
  Bmp_Data bmp;
  request(LOG_BMP);
  getData(&bmp);
  return bmp;
}

Mma_Data SITL::readMMA(){
  Mma_Data mma;
  request(LOG_MMA);
  getData(&mma);
  return mma;
}

Gps_Data SITL::readGPS(){
  Gps_Data gps;
  request(LOG_GPS);
  getData(&gps);
  return gps;
}

Bno_Data SITL::readBNO(){
  Bno_Data bno;
  request(LOG_BNO);
  getData(&bno);
  return bno;
}


float SITL::readVbat(){
  float vbat;
  request(LOG_VBAT);
  getData(&vbat);
  return vbat;
}

void SITL::request(byte select){
  float2Bytes(float (global_time),(requestMsg+2));
  requestMsg[1] = select;
  Serial.write(requestMsg,6);
}

void SITL::float2Bytes(float val, byte* bytes_array){
  //Create funion of shared memory space
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}

float SITL::bytes2Float(byte* bytes_array){
  float f;
  memcpy(&f, &bytes_array, 4);
  return f;
}
