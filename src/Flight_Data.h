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
#include <SDfat.h>
#include "Salt_Rev0.h"
#include "Flight_Configuration.h"

uint16_t D_ARRAY_LENGTH  = 100;
uint16_t D_STORE_FREQ    = 1;
uint16_t D_SAMPLE_FREQ   = 30;

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
  Circular_Array(uint16_t sample_freq, uint16_t store_freq, uint16_t array_length);
  ~Circular_Array();
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
    bmp_buf(D_SAMPLE_FREQ, D_STORE_FREQ, D_ARRAY_LENGTH),
    mma_buf(D_SAMPLE_FREQ, D_STORE_FREQ, D_ARRAY_LENGTH),
    bno_buf(D_SAMPLE_FREQ, D_STORE_FREQ, D_ARRAY_LENGTH),
    esense_buf(D_SAMPLE_FREQ, D_STORE_FREQ, D_ARRAY_LENGTH),
    isosense_buf(D_SAMPLE_FREQ, D_STORE_FREQ, D_ARRAY_LENGTH) {}

  bool initialize();
  byte getESense();
  byte getIsoSense();
  void updateESense(byte esense_array);
  void updateIsoSense(byte iso_sense_array);
  void updateBMP(Bmp_Data* bmp_data);
  void writeBuffers();
  void printBuffers();

  /* ----- SD Card ----- */
  void dataLog(byte sensor);
  File newDataFile(File dir);
  void printDirectory(File dir, int numTabs);

private:
  elapsedMillis global_time;
  elapsedMillis flight_time;
  Bmp_Data bmp_data;
  Mma_Data mma_data;
  Bno_Data bno_data;
  byte iso_sense_array;
  byte esense_array;
  Circular_Array<Bmp_Data> bmp_buf;
  Circular_Array<Mma_Data> mma_buf;
  Circular_Array<Bno_Data> bno_buf;
  Circular_Array<byte> esense_buf;
  Circular_Array<byte> isosense_buf;

  /* ----- SD Card ----- */
  SdFat sd;
  SdFile data_file;
  uint32_t bgnBlock, endBlock;



};



#endif
