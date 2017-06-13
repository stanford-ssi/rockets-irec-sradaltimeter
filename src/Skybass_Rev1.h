/*
This file contains all pin definitions and other board specific values for the IREC SRAD AlTIMETER REV1 "SKYBASS" PCB
Author: John Dean
Date: 1-6-2017
*/

#ifndef SKYBASS_H
#define SKYBASS_H

/* Pins */


#define LED_1         29
#define LED_2         30
#define LED_3         31
#define LED_4         32
#define BUZZER        23

#define BMP1_CS       17
#define BMP2_CS       16
#define SD_CS         28
#define MMA_CS        22
#define BNO_ADR       0x26  //this isn't the actual address

#define ARM_MMA_X     21
#define ARM_MMA_Y     20

//10 is an unused pin on this rev
#define ISO_SENSE_11  10
#define ISO_SENSE_12  10
#define ISO_SENSE_13  10
#define ISO_SENSE_21  10
#define ISO_SENSE_22  10
#define ISO_SENSE_23  10

#define TRIG_1        25
#define TRIG_2        24
#define TRIG_3        4
#define TRIG_4        3
#define ESENSE_1      26
#define ESENSE_2      6
#define ESENSE_3      5
#define ESENSE_4      2

#define VSENSE        A10

/* serial ports */
#define xbeeSerial Serial3
#define GPS_SERIAL Serial1

/* Values */

#define VBAT_RATIO    (10.0/15.0)
#define BUZZER_OFF    0


#endif
