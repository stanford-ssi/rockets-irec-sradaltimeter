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

void Flight_Data::writeBuffers(){
  int i = millis();
  test_buffer.push(i);
}

void Flight_Data::printBuffers(){
  int* buffer = test_buffer.getFullArray();
  for(int i = 0; i < ARRAYLENGTH; i++){
    Serial.print(buffer[i]);
    Serial.print(',');
  }
  Serial.println(' ');
  //delete buffer;
}

//-----Circular_Storage_Buffer


template<class t_type> void Circular_Buffer<t_type>::push(t_type data){
  head++;
  checkHead();
  data_array[head] = data;
}

template<class t_type> t_type Circular_Buffer<t_type>::getLast(){
  return data_array[head];
}

template<class t_type> t_type* Circular_Buffer<t_type>::getFullArray(){
  t_type* full_array = new t_type[ARRAYLENGTH];
  for(int i = 0; i <= head; i++){
    full_array[i] = data_array[head-i];
  }
  for(int i = head+1; i < ARRAYLENGTH; i++){
    full_array[i] = data_array[(ARRAYLENGTH+head)-i];
  }
  return full_array;
}


template<class t_type> void Circular_Buffer<t_type>::checkHead(){
  if(head >= ARRAYLENGTH){
    head = 0;
  }
}
