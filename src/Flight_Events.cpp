
#include "Flight_Events.h"

volatile uint8_t Flight_Events::events = 0b00000000; //need to declare the static variable

void Flight_Events::initialize(){
  main_loop_timer.begin(this->eventMain, 1000000 / UPDATE_FREQ_HZ);
  altimeter_timer.begin(this->eventAltimeter, 1000000 / 200);
  accel_timer.begin(this->eventAccel, 1000000 / 100);
}

bool Flight_Events::check(uint8_t EVENT){
  if(events & EVENT){
    events &= ~EVENT;
    return true;
  }
  return false;
}

void Flight_Events::eventMain(void){
	events |= EVENT_MAIN;
}

void Flight_Events::eventAltimeter(void){
	events |= EVENT_READ_BMP;
}

void Flight_Events::eventAccel(void){
	events |= EVENT_READ_MMA;
}
