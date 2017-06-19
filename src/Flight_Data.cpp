/*

Author: John Dean
Date: 1-6-2017
*/

#include "Flight_Data.h"

 /*Flight_Data::Flight_Data() {

 }*/

bool Flight_Data::initialize(){
  bool success = true;
  return success;
}
void Flight_Data::updateESense(byte esense){
  esense_array.push(esense);
}

void Flight_Data::updateIsoSense(byte iso_sense){
  isosense_array.push(iso_sense);
}

void Flight_Data::updateBMP(Bmp_Data bmp_data){
  bmp_array.push(bmp_data);
}

void Flight_Data::updateMMA(Mma_Data mma_data){
  mma_array.push(mma_data);
}

void Flight_Data::updateBNO(Bno_Data bno_data){
  bno_array.push(bno_data);
}

void Flight_Data::updateGPS(Gps_Data gps_data){
  gps_array.push(gps_data);
}





byte Flight_Data::getESense(){
  return this->esense;
}

byte Flight_Data::getIsoSense(){
  return this->iso_sense;
}

long Flight_Data::getGlobaltime(){
  return this->global_time;
}

Bmp_Data Flight_Data::getBMPdata(){
  return bmp_array.getLast();
}
Mma_Data Flight_Data::getMMAdata(){
  return mma_array.getLast();
}
Bno_Data Flight_Data::getBNOdata(){
  return bno_array.getLast();
}
Gps_Data Flight_Data::getGPSdata(){
  return gps_array.getLast();
}




/* ----- Circular_Storage_Buffer ------ */
