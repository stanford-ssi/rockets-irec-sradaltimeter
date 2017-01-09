/*
  This file contains the class for the altimeter for which everything
  else is a meber of.
*/
#ifndef ALTIMETER_H
#define ALTIMETER_H

#include "Flight_Configuration.h"
#include "Flight_Sensors.h"
#include "Flight_Data.h"
#include "Flight_Events.h"

class Altimeter{
public:
  Flight_Data flight_data;
  Flight_Sensors flight_sensors;
  Flight_Events flight_events;


private:


};



#endif
