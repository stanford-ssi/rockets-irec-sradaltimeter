#ifndef GPS_SENSOR_H
#define GPS_SENSOR_H

#include <Arduino.h>
#include "Flight_Configuration.h"

class Gps_Sensor {
 public:
  Gps_Sensor(HardwareSerial* _serial);
  bool begin();
  /*
   * reads a single char from the serial stream and, if it's '\n' parses the string
   * and returns true if the data has updated.
   */
  bool update();
  Gps_Data readPosition();

  static bool parseUblox(const char* str, Gps_Data* data);
  
 private:
  HardwareSerial* serial;
  char nmea_buffer[NMEA_BUFF_SIZE];
  int cur_buff_index;
  Gps_Data data;
  
};

#endif
