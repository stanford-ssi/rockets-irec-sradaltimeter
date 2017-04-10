#include "SITL.h"

/*Sensors*/


bool SITL::initialize(){
  bool sucessful = true;
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
  request(FESENSE);
  while(true){
    if(Serial.available()){
      byte read = Serial.read();
      return read;
    }
  }
}

byte SITL::readIsoSense(){
  request(FISOSENSE);
  while(true){
    if(Serial.available()){
      byte read = Serial.read();
      return read;
    }
  }
}

Bmp_Data SITL::readBMP(){
  Bmp_Data bmp_data;
  request(FBMP);
  int data_len = sizeof(Bmp_Data);
  while(true){
    if(Serial.available() == data_len){
      byte read[data_len];
      for(int i = 0;i < 8;i++){
        read[i]= Serial.read();
      }
      bmp_data.pressure1 = bytes2Float(read);
      bmp_data.pressure2 = bytes2Float(read+4);
      return bmp_data;
    }
  }
}

Mma_Data SITL::readMMA(){
  Mma_Data mma_data;
  request(FMMA);
  int data_len = sizeof(Bmp_Data);
  while(true){
    if(Serial.available() == data_len){
      byte read[data_len];
      for(int i = 0;i < 8;i++){
        read[i]= Serial.read();
      }
      mma_data.x = bytes2Float(read);
      mma_data.y = bytes2Float(read+4);
      return mma_data;
    }
  }
}

Gps_Data SITL::readGPS(){
  Gps_Data data;
  request(FGPS);
  int data_len = sizeof(Gps_Data);
  while(true){
    if(Serial.available() == data_len){
      byte read[data_len];
      for(int i = 0;i < 8;i++){
        read[i]= Serial.read();
      }
      data.lat = bytes2Float(read);
      data.lon = bytes2Float(read+4);
      data.alt = bytes2Float(read+8);
      data.lock = read[12];
      return data;
    }
  }
}


float SITL::readVbat(){
  int read = analogRead(VSENSE);
  float vbat = ((float)read)/1023.0 * 3.3 / (VBAT_RATIO);
  return vbat;
}

void SITL::request(byte select){
  float2Bytes(float (global_time),(requestMsg+1));
  requestMsg[0] = select;
  Serial.write(requestMsg,5);
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
