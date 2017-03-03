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
  static void eventMain(void);
  static void eventAltimeter(void);
  static void eventAccel(void);


private:
  IntervalTimer main_loop_timer;
  IntervalTimer altimeter_timer;
  IntervalTimer accel_timer;
};

#endif
