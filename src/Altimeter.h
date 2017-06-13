/*
  This file contains the class for the altimeter for which everything
  else is a meber of.
*/
#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <WProgram.h>
#include <stdint.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include "Flight_Sensors.h"
#include "Flight_Data.h"
#include "Flight_Events.h"
#include "SITL.h"
#include "Flight_Configuration.h"
#include "Logger.h"
#include "Altitude_Filter.h"


class Altimeter{
public:
  /* member objects */
  Altitude_Filter alt_filter;
  Flight_Data flight_data;
  #ifdef SITL_ON
    SITL flight_sensors;
  #else
    Flight_Sensors flight_sensors;
  #endif
  Flight_Events flight_events;

  Logger logger;

  /* member variables */
  uint16_t flight_state = STRTUP;   //flight state variable
  uint8_t led_counter = 0;          //counter used to blink the led
  uint8_t buzzer_counter;           //counter used to buzz the buzzer
  uint8_t buzzer_freq_scaler;       //counter used to act as a scaler on the buzzer frequency
  
  void manageEvents();
  void startup();


private:
  void mainUpdate();
  void manageBuzzer();
  void manageLEDs();
  void logData();
  void buzzInidicate(bool buzz);
  void buzzOff();
  void transmitXbee();
  uint8_t temp_counter = 0;
  uint8_t xbee_buf[XBEE_BUF_LENGTH];


};



#endif
