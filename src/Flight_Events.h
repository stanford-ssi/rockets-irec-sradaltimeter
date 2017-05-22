/*
This file contains the Flight_Events class, which manages the schedualing
of taskes through the use of interrup timers and the events varaible.

Author: John Dean
Date: 1-8-2017
*/


#ifndef EVENTS_H
#define EVENTS_H

#include <WProgram.h>
#include "Flight_Configuration.h"



class Flight_Events{
public:
  static volatile uint8_t events; // this is the variable that contains the events bit flags

  void initialize();
  bool check(uint8_t EVENT);

  /* functions called by interrupt timers. they are static so
  that is is possible to pass the pointer to them to the interrupt
  timer object */
  static void updateClk(void);
  static uint8_t processor_busy;

private:
  IntervalTimer update_clk_timer;
  static volatile uint16_t main_precounter;
  static volatile uint16_t bmp_precounter;
  static volatile uint16_t bno_precounter;
  static volatile uint16_t mma_precounter;
  static volatile uint16_t gps_precounter;
  static volatile uint16_t buzzer_precounter;
  static volatile uint16_t filter_precounter;
};

#endif
