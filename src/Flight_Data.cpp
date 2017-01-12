/*

Author: John Dean
Date: 1-6-2017
*/

#include "Flight_Data.h"

void Flight_Data::updateESense(bool* esense_array){
  this->esense_array[0] = esense_array[0];
  this->esense_array[1] = esense_array[1];
  this->esense_array[2] = esense_array[2];
  this->esense_array[3] = esense_array[3];
  delete [] esense_array;
}

void Flight_Data::updateIsoSense(bool* iso_sense_array){
  this->iso_sense_array[0] = iso_sense_array[0];
  this->iso_sense_array[1] = iso_sense_array[1];
  this->iso_sense_array[2] = iso_sense_array[2];
  this->iso_sense_array[3] = iso_sense_array[3];
  this->iso_sense_array[4] = iso_sense_array[4];
  this->iso_sense_array[5] = iso_sense_array[5];
  delete [] iso_sense_array;
}

bool* Flight_Data::getESense(){
  return this->esense_array;
}
bool* Flight_Data::getIsoSense(){
  return this->iso_sense_array;
}

void Flight_Data::updatePressure(Bmp_Data* bmp_data){
  this->bmp_data.pressure1 = bmp_data->pressure1;
  this->bmp_data.pressure2 = bmp_data->pressure2;

  delete bmp_data;
}
