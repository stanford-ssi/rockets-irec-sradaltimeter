/*
This file contains flight configuration information for the IREC SRAD altimeter.
Author: John Dean
Date: 1-6-2017
*/

#ifndef FLIGHT_CONFIGURATION_H
#define FLIGHT_CONFIGURATION_H

#include <stdint.h>
#include <QuickGPS.h>

#include "Skybass_Rev2.h" // change for which rev the board is
//#define SITL_ON    //toggle this to turn on SITL testing
//#define _DEBUG_    //toggle this to put into debug mode
#define _BUZZER_   //toggle this to enable buzzer
#define _LOG_      //toggle this to enable logging

/* Frequency settings. Values are in Hz */
#define UPDATE_CLK_FREQ_HZ  100      //frequency of the update "clock"
#define MAIN_FREQ           20      //frequency of the main update
#define BNO_FREQ            50
#define BMP_FREQ            50
#define MMA_FREQ            50
#define GPS_FREQ            1
#define BUZZER_FREQ         20
#define FILTER_FREQ         50
#define BEEP_FREQ_HZ        4        //frequency that the buzzer will beep at

/*setting for history buffers*/
#define DEFAULT_ARRAY_LENGTH    50
#define DEFAULT_STORE_FREQ      10

/* state transition values */
#define LAUNCH_ANGLE_AVG_MIN 75
#define LAUNCH_ANGLE_STD_MAX 1.0
#define LIFTOFF_DV_MIN 50
#define LIFTOFF_ALT_MIN 100
#define RECOVERY_MAIN_ALT 1500        // Altitude to deplot main at, in m
#define RECOVERY_MAIN_MIN_DELAY 10  // Minumum time after appogee for main to delpoy
#define RECOVERY_DROGUE_TIME 2      // Time delay after appo for Drogue
#define RECOVERY_SEP_TIME 2          // Time BEFORE appo for Sep
#define RECOVERY_SEP_BACKUP_TIME 1   // Time delay after appo for Sep
#define MIN_FLIGHT_TIME 5         // Minimum time from liftoff to appogee, in seconds

/*settings for indicating sensors*/
#define FSTART       0xAA


/*tone settings for the buzzer */
#define BUZZ_TONE_HIGH      5000
#define BUZZ_TONE_MID       4500
#define BUZZ_TONE_LOW       3000




//enumeration of flight states
enum {
  STRTUP,
  IDLE,
  ARMED,
  FLIGHT,
  DESCNT,
  PRE_RCVRY,
  RCVRED,
};

//enumeration of flight events
#define NUM_EVENTS 7
typedef enum {
  EVENT_MAIN     =  0b00000001, //1
  EVENT_FILTER   =  0b00000010, //2
  EVENT_READ_MMA =  0b00000100, //4
  EVENT_READ_BMP =  0b00001000, //8
  EVENT_READ_GPS =  0b00010000, //16
  EVENT_BUZZER   =  0b00100000, //32
  EVENT_READ_BNO =  0b01000000, //64
} event_t;

// enumeration of event
enum {
  MAIN,
  DROGUE,
  SEP,
  SEP_BACKUP,
};

//pins for ematch channels
#define NUM_EMATCHES 4
const int ematch_pins[4] = {TRIG_1,TRIG_2,TRIG_3,TRIG_4};

// some structures for handeling data
typedef QuickGPS::Data Gps_Data;
/*
typedef struct{
  uint64_t time;
  float lon;
  float lat;
  float alt;
  bool lock;
} Gps_Data;
*/
typedef struct{
  float pressure1;
  float temp1;
  float pressure2;
  float temp2;
} Bmp_Data;

const Bmp_Data bmp_default = {100000,50,100000,50};

typedef struct{
  float x;
  float y;
} Mma_Data;

const Mma_Data mma_default = {0,0};

typedef struct{
  struct{
    float x;
    float y;
    float z;
  } lin_a;
  struct{
    float w;
    float x;
    float y;
    float z;
  } quat;
  struct{
    float x;
    float y;
    float z;
  } euler;
  struct{
    float x;
    float y;
    float z;
  } gyro;
  struct{
    float x;
    float y;
    float z;
  } rot_a;
} Bno_Data;

typedef struct{
  long event;
} Event_Data;



enum loggers {LOG_BMP, LOG_MMA, LOG_BNO, LOG_GPS, LOG_VBAT, LOG_EVENT};


//xbee communication

#define TX_START  0x40
#define TX_END    0xB0

// message is start byte + size + data + parity byte + end byte
const uint8_t XBEE_BUF_LENGTH = 1 + 1 + sizeof(long) + sizeof(Bmp_Data)
                              + sizeof(Mma_Data) + sizeof(Bno_Data)
                              + sizeof(Gps_Data) + sizeof(float) + 1 + 1;


#endif
