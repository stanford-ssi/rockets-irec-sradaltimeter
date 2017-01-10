/*
  This file contains the class for the altimeter for which everything
  else is a meber of.
*/
#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <WProgram.h>
#include <stdint.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include "Salt_Rev0.h"
#include "Flight_Sensors.h"
#include "Flight_Data.h"
#include "Flight_Events.h"

#include "Flight_Configuration.h"


class Altimeter{
public:
  /* member objects */
  Flight_Data flight_data;
  Flight_Sensors flight_sensors;
  Flight_Events flight_events;
  /* mebmer variables */
  uint16_t flight_state = STRTUP;
  uint8_t led_counter = 0;
  uint8_t buzzer_counter;
  uint8_t buzzer_freq_scaler;
  
  void manageEvents();
  void startup();



private:

  void mainUpdate();
  void manageBuzzer();
  void manageLEDs();
  void logData();


};



#endif
