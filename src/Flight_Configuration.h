/*
This file contains flight configuration information for the IREC SRAD altimeter.
Author: John Dean
Date: 1-6-2017
*/

#ifndef FLIGHT_CONFIGURATION_H
#define FLIGHT_CONFIGURATION_H

#include <stdint.h>
#include <QuickGPS.h>

#include "Skybass_Rev1.h" // change for which rev the board is
//#define SITL_ON    //toggle this to turn on SITL testing
//#define _DEBUG_    //toggle this to put into debug mode
//#define _BUZZER_   //toggle this to enable buzzer


/* Frequency settings. Values are in Hz */
#define UPDATE_CLK_FREQ_HZ  200      //frequency of the update "clock"
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
#define BUZZ_TONE_MID       4500
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
#define NUM_EVENTS 7
typedef enum {
  EVENT_MAIN     =  0b00000001, //1
  EVENT_READ_BNO =  0b00000010, //2
  EVENT_READ_MMA =  0b00000100, //4
  EVENT_READ_BMP =  0b00001000, //8
  EVENT_READ_GPS =  0b00010000, //16
  EVENT_BUZZER   =  0b00100000, //32
  EVENT_FILTER   =  0b01000000, //64
} event_t;




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

typedef struct{
  float x;
  float y;
} Mma_Data;

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



enum loggers {LOG_BMP, LOG_MMA, LOG_BNO, LOG_EVENT, LOG_GPS};


//xbee communication

#define TX_START  0x40
#define TX_END    0xB0

// message is start byte + size + data + parity byte + end byte
const uint8_t XBEE_BUF_LENGTH = 1 + 1 + sizeof(long) + sizeof(Bmp_Data)
                              + sizeof(Mma_Data) + sizeof(Bno_Data)
                              + sizeof(Gps_Data) + 1 + 1;


#endif
