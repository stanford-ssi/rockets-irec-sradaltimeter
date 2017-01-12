/*
This class deals with interfacing with all sensors.

Author: John Dean
Date: 1-6-2017
*/
#ifndef FLIGHT_SENSORS_H
#define FLIGHT_SENSORS_H
#include <WProgram.h>
#include <ADC.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include <ADC.h>
#include "Salt_Rev0.h"
#include "Flight_Configuration.h"

class Flight_Sensors {
public:
  Flight_Sensors();
  bool initialize();

  //this function reads pressure data from the BMP280's and retruns a pointer to the struct
  bool* readESense();
  bool* readIsoSense();
  Bmp_Data* readPressure();
  float readVbat();

private:
  Adafruit_BMP280 bmp1;
  Adafruit_BMP280 bmp2;
  Adafruit_BNO055 bno;
};


class Flight_GPIO {

};

#endif
