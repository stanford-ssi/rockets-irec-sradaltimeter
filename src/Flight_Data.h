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

class Flight_Data {
public:

  byte getESense();
  byte getIsoSense();

  void updateESense(byte esense_array);
  void updateIsoSense(byte iso_sense_array);
  void updateBMP(Bmp_Data* bmp_data);

private:
  elapsedMillis global_time;
  elapsedMillis flight_time;
  Bmp_Data bmp_data;
  Mma_Data mma_data;
  Bno_Data bno_data;
  byte iso_sense_array;
  byte esense_array;

};

#endif
