
#include "Flight_Events.h"

volatile uint8_t Flight_Events::events = 0b00000000; //need to declare the static variable

void Flight_Events::initialize(){
  main_loop_timer.begin(this->eventMain, 1000000 / UPDATE_FREQ_HZ);
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
