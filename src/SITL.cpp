#include "SITL.h"

/*Sensors*/


bool SITL::initialize(){
  bool sucessful = true;
  //holds until connection with PC is established
  while(true){
    Serial.write(0x00);
    delay(100);
    if(Serial.available()){
      if(Serial.read() == FSTART){
        return sucessful;
      }
    }
  }
}

bool* SITL::readESense(){
  bool* esense_array = new bool[4];
  updateTimeRequest();
  request[0] = FESENSE;
  Serial.write(request,5);
    while(true){
      if(Serial.available()){
        byte read = Serial.read();
        esense_array[0] = 0b10000000 & read;
        esense_array[0] = 0b01000000 & read;
        esense_array[0] = 0b00100000 & read;
        esense_array[0] = 0b00010000 & read;
        return esense_array;
      }
    }
}

bool* SITL::readIsoSense(){
  bool* iso_sense_array = new bool[6];
  iso_sense_array[0] = digitalReadFast(ISO_SENSE_11);
  iso_sense_array[1] = digitalReadFast(ISO_SENSE_12);
  iso_sense_array[2] = digitalReadFast(ISO_SENSE_13);
  iso_sense_array[3] = digitalReadFast(ISO_SENSE_21);
  iso_sense_array[4] = digitalReadFast(ISO_SENSE_22);
  iso_sense_array[5] = digitalReadFast(ISO_SENSE_23);
  return iso_sense_array;
}

Bmp_Data* SITL::readPressure(){
  Bmp_Data* bmp_data = new Bmp_Data;
  updateTimeRequest();
  request[0] = FBMP;
  Serial.write(request,5);
  while(true){
    if(Serial.available() == 8){
      byte read[8];
      for(int i = 0;i < 8;i++){
        read[i]= Serial.read();
      }
      bmp_data->pressure1 = bytes2Float(read);
      bmp_data->pressure2 = bytes2Float(read+4);
      return bmp_data;
    }
  }
}

float SITL::readVbat(){
  int read = analogRead(VSENSE);
  float vbat = ((float)read)/1023.0 * 3.3 / (VBAT_RATIO);
  return vbat;
}

void SITL::updateTimeRequest(){
    float2Bytes(float (global_time),(request+1));
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
