#include "Gps_Sensor.h"

Gps_Sensor::Gps_Sensor(HardwareSerial* _serial)
  : serial(_serial),
    latitude(0.0f),
    longitude(0.0f),
    buf_index(0) {}

bool Gps_Sensor::begin() {
  serial->begin(9600);
  return true;
}

bool Gps_Sensor::update() {

  bool lat_real = false;
  bool log_real = false;
  
  while (serial->available()) {
    //We want to have a max so we don't parse a too-long string. Better to just fail
    //then take forever
    if (buf_index >= NMEA_BUFF_SIZE) {
      buf_index = 0;
    }
    
    char in = serial->read();
    if (in == '\n') {
      nmea_buffer[buf_index++] = '\0';
      buf_index = 0;
      char* data = strtok(nmea_buffer, ",");

      //have we recieved the latest latitude and longitude values?
      bool just_parsed_ll = false;
      float tmp_ll = 0.0;

      //super janky NMEA parser that just gets latitude and longitude
      while (data != NULL) {
	int len = strlen(data);
	if (len == 10 || len == 11) {
	  char deg[4];
	  char min[9];
	  if (len == 10) {
	    strncpy(deg, data, 2);
	    deg[2] = '\0';
	    strcpy(min, data+2);
	  } else {
	    strncpy(deg, data, 3);
	    deg[3] = '\0';
	    strcpy(min, data+3);
	  }
	  tmp_ll = atoi(deg);
	  tmp_ll += atof(min)/60.0;
	  just_parsed_ll = true;
	} else {
	  if (len == 1 && just_parsed_ll) {
	    switch (data[0]) {
	    case 'W':
	      tmp_ll = -tmp_ll;
	    case 'E':
	      longitude = tmp_ll;
	      log_real = true;
	      break;
	    case 'S':
	      tmp_ll = -tmp_ll;
	    case 'N':
	      latitude = tmp_ll;
	      lat_real = true;
	      break;
	    }
	  }
	  just_parsed_ll = false;
	}
	data = strtok(NULL, ",");
      }
      
    } else {
      nmea_buffer[buf_index++] = in;
    }
  }
  return lat_real && log_real;
}

Gps_Data* Gps_Sensor::readPosition() {
  Gps_Data* ret = new Gps_Data;
  ret->latitude = latitude;
  ret->longitude = longitude;
  return ret;
}
