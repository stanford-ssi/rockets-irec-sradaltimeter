
#include "Flight_Events.h"


volatile uint8_t Flight_Events::events = 0b00000000; //need to declare the static variable
volatile uint16_t Flight_Events::main_precounter = 0;
volatile uint16_t Flight_Events::bmp_precounter = 0;
volatile uint16_t Flight_Events::bno_precounter = 0;
volatile uint16_t Flight_Events::mma_precounter = 0;
volatile uint16_t Flight_Events::gps_precounter = 0;
bool Flight_Events::processor_busy = 0;

void Flight_Events::initialize(){
  update_clk_timer.begin(this->updateClk, 1000000 / UPDATE_CLK_FREQ_HZ);
}

bool Flight_Events::check(uint8_t EVENT){
  if(events & EVENT){
    events &= ~EVENT;
    return true;
  }
  return false;
}

void Flight_Events::updateClk(void){
  if(processor_busy){
    Serial.println("WARNING: Loop took more than 5ms to execute");
  }

  main_precounter++;
  if(main_precounter == UPDATE_CLK_FREQ_HZ/MAIN_FREQ){
    main_precounter = 0;
    events |= EVENT_MAIN;
  }

  bmp_precounter++;
  if(bmp_precounter == UPDATE_CLK_FREQ_HZ/MAIN_FREQ){
    bmp_precounter = 0;
    events |= EVENT_READ_BMP;
  }

  mma_precounter++;
  if(mma_precounter == UPDATE_CLK_FREQ_HZ/MAIN_FREQ){
    mma_precounter = 0;
    events |= EVENT_READ_MMA;
  }

  bno_precounter++;
  if(bno_precounter == UPDATE_CLK_FREQ_HZ/MAIN_FREQ){
    bno_precounter = 0;
    events |= EVENT_READ_BNO;
  }

  gps_precounter++;
  if(gps_precounter == UPDATE_CLK_FREQ_HZ/MAIN_FREQ){
    gps_precounter = 0;
    events |= EVENT_READ_GPS;
  }
}
