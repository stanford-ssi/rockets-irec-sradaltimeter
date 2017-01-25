/*

Author: John Dean
Date: 1-6-2017
*/

#include "Flight_Data.h"

void Flight_Data::updateESense(byte esense_array){
  this->esense_array = esense_array;
}

void Flight_Data::updateIsoSense(byte iso_sense_array){
  this->iso_sense_array = iso_sense_array;
}

byte Flight_Data::getESense(){
  return this->esense_array;
}
byte Flight_Data::getIsoSense(){
  return this->iso_sense_array;
}

void Flight_Data::updateBMP(Bmp_Data* bmp_data){
  this->bmp_data.pressure1 = bmp_data->pressure1;
  this->bmp_data.pressure2 = bmp_data->pressure2;

  delete bmp_data;
}
