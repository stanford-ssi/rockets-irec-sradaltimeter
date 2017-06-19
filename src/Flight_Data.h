/*
This is the header file for the FlightData class, which is used to
to store all of the data for the flight. The class for reading sensors
is separate, and this is done to easily support software-in-the-loop and
hardware-in-the-loop testing.

Author: John Dean
Date: 1-6-2017
*/
#ifndef FLIGHT_DATA_H
#define FLIGHT_DATA_H

#include <WProgram.h>
#include "Flight_Configuration.h"



#define BLOCK_COUNT 256000

/*
This class is a curicular storage buffer used for storing both the most recent recorded
data value, and a history. It can take any type. It must be initialized with:
1. the frequency that the data is sampled at
2. the frequency that the data will be stored at
3. the length of the array
*/
template <class t_type> class Circular_Array{
public:
  Circular_Array(uint16_t sample_freq, uint16_t store_freq, uint16_t array_length)
  {
    this->sample_freq = sample_freq;
    this->store_freq = store_freq;
    this->array_length = array_length;
    this->store_prescale = sample_freq/store_freq;
    data_array = new t_type[array_length];
    head = 0;
    write_count = 0;
  };
  ~Circular_Array() {
    delete [] data_array;
  };
  void push(t_type data);
  t_type getLast();
  bool getFullArray(t_type* full_array, int len);
  t_type getOld(uint32_t get_time);

private:
  uint16_t sample_freq;
  uint16_t store_freq;
  uint16_t store_prescale;
  uint16_t array_length;
  uint16_t log_count;
  uint16_t head;
  uint16_t write_count;
  void checkHead();
  t_type* data_array;
};

/* ------- TEMPLATE CLASS DEFINITIONS ------- */

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

template<class t_type> bool Circular_Array<t_type>::getFullArray(t_type* full_array, int len){
  for(int i = 0; i <= head; i++){
    if(i == len) return false;
    full_array[i] = data_array[head-i];
  }
  for(int i = head+1; i < array_length; i++){
    if(i == len) return false;
    full_array[i] = data_array[(array_length+head)-i];
  }
  return true;
}

template<class t_type> void Circular_Array<t_type>::checkHead(){
  if(head >= array_length){
    head = 0;
  }
}

/* ----- FLIGHT DATA CLASS ----- */

class Flight_Data {
public:
  Flight_Data() :
    bmp_array(BMP_FREQ, DEFAULT_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    mma_array(MMA_FREQ, DEFAULT_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    bno_array(BNO_FREQ, DEFAULT_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    gps_array(GPS_FREQ, DEFAULT_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    esense_array(MAIN_FREQ, DEFAULT_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    isosense_array(MAIN_FREQ, DEFAULT_STORE_FREQ, DEFAULT_ARRAY_LENGTH) {};

  bool initialize();
  void updateESense(byte esense);
  void updateIsoSense(byte iso_sense);
  void updateBMP(Bmp_Data bmp_data);
  void updateMMA(Mma_Data mma_data);
  void updateBNO(Bno_Data bno_data);
  void updateGPS(Gps_Data gps_data);
  byte getESense();
  byte getIsoSense();
  Bmp_Data getBMPdata();
  Mma_Data getMMAdata();
  Bno_Data getBNOdata();
  Gps_Data getGPSdata();
  long getGlobaltime();

  Circular_Array<Bmp_Data> bmp_array;
  Circular_Array<Mma_Data> mma_array;
  Circular_Array<Bno_Data> bno_array;
  Circular_Array<Gps_Data> gps_array;
  Circular_Array<byte> esense_array;
  Circular_Array<byte> isosense_array;

private:

  elapsedMicros global_time;
  elapsedMicros flight_time;
  Bmp_Data bmp_data;
  Mma_Data mma_data;
  Bno_Data bno_data;
  byte iso_sense;
  byte esense;

};





#endif
