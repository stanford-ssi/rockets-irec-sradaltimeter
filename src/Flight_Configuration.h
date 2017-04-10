/*
This file contains flight configuration information for the IREC SRAD altimeter.
Author: John Dean
Date: 1-6-2017
*/

#ifndef FLIGHT_CONFIGURATION_H
#define FLIGHT_CONFIGURATION_H

//#define SITL_ON    //togle this to turn on SITL testing

#include <stdint.h>
#include <QuickGPS.h>

//#define SITL    //togle this to turn on SITL testing


/* Frequency settings. Values are in Hz */
#define UPDATE_CLK_FREQ_HZ  200      //frequency of the update "clock"
#define MAIN_FREQ           20      //frequency of the main update
#define BNO_FREQ            50
#define BMP_FREQ            50
#define MMA_FREQ            50
#define GPS_FREQ            20
#define BUZZER_FREQ         20
#define FILTER_FREQ         50
#define BEEP_FREQ_HZ        4        //frequency that the buzzer will beep at

/*  */

/*settings for indicating sensors*/
#define FSTART       0xAA
enum {
  FESENSE    =  0x01,
  FISOSENSE  =  0x02,
  FBNO       =  0x03,
  FMMA       =  0x04,
  FBMP       =  0x05,
  FGPS       =  0x06,
};



/*tone settings for the buzzer */
#define BUZZ_TONE_HIGH      5000
#define BUZZ_TONE_MID       4000
#define BUZZ_TONE_LOW       3000

//GPS serial port
#define GPS_SERIAL Serial2


//enumeration of flight states
enum {
  STRTUP,
  IDLE,
  ARMED,
  PWRD_FLGHT,
  COAST,
  DESCNT_DROG,
  DESCNT_MAIN,
  PRE_RCVRY,
  RCVRED,
};

//enumeration of flight events
enum {
  EVENT_MAIN     =  0b00000001,
  EVENT_READ_BNO =  0b00000010,
  EVENT_READ_MMA =  0b00000100,
  EVENT_READ_BMP =  0b00001000,
  EVENT_READ_GPS =  0b00010000,
  EVENT_BUZZER   =  0b00100000,
  EVENT_FILTER   =  0b01000000,
};


// some structures for handeling data
typedef QuickGPS::Data Gps_Data;

typedef struct{
  float pressure1;
  float pressure2;
} Bmp_Data;

typedef struct{
  float x;
  float y;
} Mma_Data;

typedef struct{
  float x;
  float y;
  float z;
} Bno_Data;

typedef struct{
  long event;
} Event_Data;

enum loggers {LOG_BMP, LOG_MMA, LOG_BNO, LOG_EVENT, LOG_GPS};


//xbee communication
#define xbeeSerial Serial3
#define TX_START  0x40
#define TX_END    0xB0

// message is start byte + size + data + parity byte + end byte
const uint8_t XBEE_BUF_LENGTH = 1 + 1 + sizeof(long) + sizeof(Bmp_Data)
                              + sizeof(Mma_Data) + sizeof(Bno_Data)
                              + sizeof(Gps_Data) + 1 + 1;


#endif
