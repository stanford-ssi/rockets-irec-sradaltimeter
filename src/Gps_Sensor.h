#ifndef GPS_SENSOR_H
#define GPS_SENSOR_H

#include <Arduino.h>

#define NMEA_BUFF_SIZE 512

typedef struct{
  float latitude;
  float longitude;
} Gps_Pos;


class Gps_Sensor {
 public:
  Gps_Sensor(HardwareSerial* _serial);
  bool begin();
  /*
   * reads a single char from the serial stream and, if it's '\n' parses the string
   * and returns true if the data has updated.
   */
  bool update();
  Gps_Pos* readPosition();

 private:
  HardwareSerial* serial;
  float latitude;
  float longitude;
  int buf_index;
  char nmea_buffer[NMEA_BUFF_SIZE];
};

#endif
