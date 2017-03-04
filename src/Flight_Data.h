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
#include "Salt_Rev0.h"
#include "Flight_Configuration.h"

const uint16_t DEFAULT_ARRAY_LENGTH  = 100;
const uint16_t D_STORE_FREQ    = 1;
const uint16_t D_SAMPLE_FREQ   = 30;

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
  t_type* getFullArray();
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

class Flight_Data {
public:
  Flight_Data() :
    bmp_array(BMP_FREQ, D_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    mma_array(D_SAMPLE_FREQ, D_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    bno_array(D_SAMPLE_FREQ, D_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    esense_array(D_SAMPLE_FREQ, D_STORE_FREQ, DEFAULT_ARRAY_LENGTH),
    isosense_array(D_SAMPLE_FREQ, D_STORE_FREQ, DEFAULT_ARRAY_LENGTH) {};

  bool initialize();
  byte getESense();
  byte getIsoSense();
  void updateESense(byte esense);
  void updateIsoSense(byte iso_sense);
  void updateBMP(Bmp_Data bmp_data);
  void updateMMA(Mma_Data mma_data);
  void writeBuffers();
  void printBuffers();

private:
  elapsedMicros global_time;
  elapsedMicros flight_time;
  Bmp_Data bmp_data;
  Mma_Data mma_data;
  Bno_Data bno_data;
  byte iso_sense;
  byte esense;

  /* ----- SD Card ----- */

  Circular_Array<Bmp_Data> bmp_array;
  Circular_Array<Mma_Data> mma_array;
  Circular_Array<Bno_Data> bno_array;
  Circular_Array<byte> esense_array;
  Circular_Array<byte> isosense_array;


};



#endif
