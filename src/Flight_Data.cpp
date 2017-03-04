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
  this->esense = esense;
}

void Flight_Data::updateIsoSense(byte iso_sense){
  this->iso_sense = iso_sense;
}

byte Flight_Data::getESense(){
  return this->esense;
}

byte Flight_Data::getIsoSense(){
  return this->iso_sense;
}

void Flight_Data::updateBMP(Bmp_Data bmp_data){
  bmp_array.push(bmp_data);
}

void Flight_Data::updateMMA(Mma_Data mma_data){
  mma_array.push(mma_data);
}

void Flight_Data::writeBuffers(){
  // int i = millis();
  // test_buffer.push(i);
}

void Flight_Data::printBuffers(){
  // int* buffer = test_buffer.getFullArray();
  // for(int i = 0; i < ARRAYLENGTH; i++){
  //   Serial.print(buffer[i]);
  //   Serial.print(',');
  // }
  // Serial.println(' ');
  // delete buffer;
}

/* ----- Circular_Storage_Buffer ------ */

/*
This function gets returns the value from a given number of milliseconds ago
Need to add a check to make sure that the given time isn't too long ago
*/
template<class t_type> t_type Circular_Array<t_type>::getOld(uint32_t get_time){
   int16_t ind = static_cast<float>(sample_freq) * get_time / store_prescale / 1000;
   ind = head - ind;
   if(ind < 0) ind += array_length;
   return data_array[ind];
}

/*
Pushes a neew value to the array.
*/
template<class t_type> void Circular_Array<t_type>::push(t_type data){
  /*Serial.print("Write Count: ");
  Serial.print(write_count);
  if(write_count == 0) Serial.print(millis());
  Serial.print("  Head: ");
  Serial.println(head); */
  write_count++;
  if(write_count >= store_prescale){
    write_count = 0;
    head++;
    checkHead();
  }
  data_array[head] = data;
}

template<class t_type> t_type Circular_Array<t_type>::getLast(){
  return data_array[head];
}

template<class t_type> t_type* Circular_Array<t_type>::getFullArray(){
  t_type* full_array = new t_type[array_length];
  for(int i = 0; i <= head; i++){
    full_array[i] = data_array[head-i];
  }
  for(int i = head+1; i < array_length; i++){
    full_array[i] = data_array[(array_length+head)-i];
  }
  return full_array;
}

template<class t_type> void Circular_Array<t_type>::checkHead(){
  if(head >= array_length){
    head = 0;
  }
}
