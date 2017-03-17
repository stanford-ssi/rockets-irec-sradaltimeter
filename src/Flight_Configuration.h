/*
This file contains flight configuration information for the IREC SRAD altimeter.

Author: John Dean
Date: 1-6-2017
*/

#ifndef FLIGHT_CONFIGURATION_H
#define FLIGHT_CONFIGURATION_H

//#define SITL    //togle this to turn on SITL testing

/* Frequency settings. Values are in Hz */
#define UPDATE_CLK_FREQ_HZ  200      //frequency of the update "clock"
#define MAIN_FREQ           20      //frequency of the main update
#define BNO_FREQ            50
#define BMP_FREQ            50
#define MMA_FREQ            50
#define GPS_FREQ            20
#define BUZZER_FREQ         20
#define BEEP_FREQ_HZ        4        //frequency that the buzzer will beep at

/*  */

/*settings for indicating sensors*/
#define FSTART       0xAA
#define FESENSE      0x01
#define FISOSENSE    0x02
#define FBNO         0x03
#define FMMA         0x04
#define FBMP         0x05


/*tone settings for the buzzer */
#define BUZZ_TONE_HIGH      5000
#define BUZZ_TONE_MID       4000
#define BUZZ_TONE_LOW       3000

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
};


// some structures for handeling data
typedef struct{
  float lon;
  float lat;
  float alt;
  bool lock;
} Gps_Data;

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

#endif
