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

  bool* getESense();
  bool* getIsoSense();

  void updateESense(bool* esense_array);
  void updateIsoSense(bool* iso_sense_array);
  void updatePressure(Bmp_Data* bmp_data);

private:
  uint32_t time;
  uint32_t liftoff_time;
  Bmp_Data bmp_data;
  Mma_Data mma_data;
  Bno_Data bno_data;
  bool iso_sense_array[6];
  bool esense_array[4];

};

#endif
