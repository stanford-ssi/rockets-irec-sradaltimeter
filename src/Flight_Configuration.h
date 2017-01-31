/*
This file contains flight configuration information for the IREC SRAD altimeter.

Author: John Dean
Date: 1-6-2017
*/

#ifndef FLIGHT_CONFIGURATION_H
#define FLIGHT_CONFIGURATION_H

//#define SITL    //togle this to turn on SITL testing


#define UPDATE_FREQ_HZ      30       //frequency of the main update
#define BEEP_FREQ_HZ        4        //frequency that the buzzer will beep at


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
};


// some structures for handeling data
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

#endif
