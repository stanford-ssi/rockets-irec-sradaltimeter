
#include "Flight_Events.h"
#include "Altimeter.h"
#include "Flight_Configuration.h"

extern Altimeter altimeter;

volatile uint8_t Flight_Events::events = 0b00000000; //need to declare the static variable
volatile uint16_t Flight_Events::main_precounter = 0;
volatile uint16_t Flight_Events::bmp_precounter = 0;
volatile uint16_t Flight_Events::bno_precounter = 0;
volatile uint16_t Flight_Events::mma_precounter = 0;
volatile uint16_t Flight_Events::gps_precounter = 0;
volatile uint16_t Flight_Events::buzzer_precounter = 0;
volatile uint16_t Flight_Events::filter_precounter = 0;
uint8_t Flight_Events::processor_busy = 0;

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
    //Serial.print("LC:");
    //Serial.println(processor_busy);
  }

  /* stuff that always should run*/
  //altimeter.transmitXbee();

  main_precounter++;
  if(main_precounter == UPDATE_CLK_FREQ_HZ/MAIN_FREQ){
    main_precounter = 0;
    events |= EVENT_MAIN;
  }

  bmp_precounter++;
  if(bmp_precounter == UPDATE_CLK_FREQ_HZ/BMP_FREQ){
    bmp_precounter = 0;
    events |= EVENT_READ_BMP;
  }

  mma_precounter++;
  if(mma_precounter == UPDATE_CLK_FREQ_HZ/MMA_FREQ){
    mma_precounter = 0;
    events |= EVENT_READ_MMA;
  }

  bno_precounter++;
  if(bno_precounter == UPDATE_CLK_FREQ_HZ/BNO_FREQ){
    bno_precounter = 0;
    events |= EVENT_READ_BNO;
  }

  gps_precounter++;
  if(gps_precounter == UPDATE_CLK_FREQ_HZ/GPS_FREQ){
    gps_precounter = 0;
    events |= EVENT_READ_GPS;
  }

  buzzer_precounter++;
  if(buzzer_precounter == UPDATE_CLK_FREQ_HZ/BUZZER_FREQ){
    buzzer_precounter = 0;
    #ifdef _BUZZER_
      events |= EVENT_BUZZER;
    #endif
  }

  filter_precounter++;
  if(filter_precounter == UPDATE_CLK_FREQ_HZ/FILTER_FREQ){
    filter_precounter = 0;
    events |= EVENT_FILTER;
  }
}
