/*
This file contains all pin definitions and other board specific values for the IREC SRAD AlTIMETER REV0 PCB

Author: John Dean
Date: 1-6-2017
*/

#ifndef SALT_REV0_H
#define SALT_REV0_H

/* Pins */


#define LED_1         29
#define LED_2         30
#define LED_3         31
#define LED_4         32
#define BUZZER        23

#define BMP1_CS       9
#define BMP2_CS       10
#define SD_CS         28
#define MMA_CS        2
#define BNO_ADR       0x26  //this isn't the actual address

#define ARM_MMA_X     3
#define ARM_MMA_Y     4

#define ISO_SENSE_11  6
#define ISO_SENSE_12  5
#define ISO_SENSE_13  27
#define ISO_SENSE_21  26
#define ISO_SENSE_22  25
#define ISO_SENSE_23  24

#define TRIG_1        20
#define TRIG_2        21
#define TRIG_3        15
#define TRIG_4        14
#define ESENSE_1      33
#define ESENSE_2      22
#define ESENSE_3      17
#define ESENSE_4      16

#define VSENSE        A10

/* Values */

#define VBAT_RATIO    (10.0/15.0)


#endif
